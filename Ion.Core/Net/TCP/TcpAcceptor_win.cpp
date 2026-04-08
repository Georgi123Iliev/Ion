#include "pch.h"

#define WIN32_LEAN_AND_MEAN  // Say this...

#include <windows.h>         // And now we can include that.
#include <winsock2.h>        // And this.
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <utility>
#include <stop_token>
#include <functional>

#include "Utility/Descriptor.hpp"
#include "TcpAcceptor.hpp"
#include "Net/Exceptions/NetworkException.hpp"


namespace Ion::Net::TCP
{

	


	class TcpAcceptor::TcpAcceptorImpl
	{

		using WinSock = Ion::Utility::Descriptor<SOCKET, ::closesocket, INVALID_SOCKET>;

		

	public:

		
		TcpAcceptorImpl(std::stop_token&& token)
		{

			const int yes = 1;


			ADDRINFOA hints = {};  //zero out struct
			PADDRINFOA temp_res;

			hints.ai_family = AF_UNSPEC; //IPv4 / IPv6
			hints.ai_socktype = SOCK_STREAM; //TCP(like) socket
			hints.ai_flags = AI_PASSIVE; // Fill ip

			auto getaddrinfoResult = getaddrinfo(
				m_nodeName,
				m_serviceName,
				&hints,
				&temp_res
			);

			if (0 != getaddrinfoResult)
			{
				throw NetworkException(getaddrinfoResult, std::system_category()
					,"DNS resolution");
			}

			auto deleter = [](PADDRINFOA p) noexcept {::freeaddrinfo(p); };

			std::unique_ptr<ADDRINFOA, decltype(deleter)> res(temp_res, deleter);

			auto s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

			if (INVALID_SOCKET == s)
			{
				throw NetworkException(::WSAGetLastError(),std::system_category(),"Socket creation");
			}

			WinSock ws{ s };

			// Windows specific: Prevent port hijacking. 
			// Unlike Linux, SO_REUSEADDR on Windows allows other apps to steal bound port
			// SO_EXCLUSIVEADDRUSE does not so it is safer here.
			auto setsockoptResult = setsockopt(ws.get(), SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&yes, sizeof(yes));

			if (0 != setsockoptResult)
			{
				throw NetworkException(::WSAGetLastError(), std::system_category(), "setsockopt EXCLUSIVEADDRUSE");
			}




			auto bindResult = bind(ws.get(), res->ai_addr, res->ai_addrlen);

			if (0 != bindResult)
			{
				throw NetworkException(::WSAGetLastError(), std::system_category(), "bind");
			}

			auto listenResult = listen(ws.get(), 10);

			if (0 != listenResult)
			{
				throw NetworkException(::WSAGetLastError(), std::system_category(), "listen");
			}

			m_socket = std::move(ws);


			m_stopCallback.emplace(std::move(token), [&socket = m_socket, &interrupted = m_interrupted]() {

				//on windows closesocket is the correct way to interrupt a blocking accept call
				// it is very important to use the method to close here as it avoids a double close down the line.
				// ::closesocket most definetly should not be used directly
				std::ignore = socket.close();

				//Set a flag that we have been interrupted
				interrupted = true;
			});

			


			
		}

		AcceptResult implAccept() noexcept
		{
			socklen_t addr_size;
			struct sockaddr_storage their_addr = {};
			addr_size = sizeof(their_addr);
			

			SOCKET new_s = ::accept(m_socket.get(), (struct sockaddr*)&their_addr, &addr_size);




			if (new_s == INVALID_SOCKET)
			{
				return std::unexpected(std::error_code(::WSAGetLastError(), std::system_category()));
			}


			auto connectionHandle = Net::Types::OpaqueHandle(new_s);

			return connectionHandle;

		}

		[[nodiscard]]
		bool isInterrupted() const noexcept
		{
			return m_interrupted;
		}

		

	private:
		//TODO: pass in as params
		PCSTR m_nodeName = "0.0.0.0";
		PCSTR m_serviceName = "8080";

		/*
		* m_socket and m_interrupted must outlive the m_stopCallback as it relies on them.
		*/
		std::atomic<bool> m_interrupted{ false };

		WinSock m_socket;
		
		//Optional - defer initialization
		//move_only_function - type erasure to allow a labda capture
		std::optional<std::stop_callback<std::move_only_function<void()>>> m_stopCallback;

		
		

	};


	TcpAcceptor::TcpAcceptor(const NetworkEnvironment& env)
		: m_env(&env),impl(std::make_unique<TcpAcceptorImpl>(env.aquireStopToken()))
	{

	}

	TcpAcceptor::~TcpAcceptor() = default;

	std::expected<TcpConnection, std::error_code> TcpAcceptor::accept() noexcept
	{
		auto res = impl->implAccept();

		if (!res)
			return std::unexpected(res.error());


		//fulfilling noexceptness
		try
		{
			return TcpConnection(res.value(), *m_env);
		}
		catch (std::bad_alloc)
		{
			return std::unexpected(std::make_error_code(std::errc::not_enough_memory));
		}
		
	}

	bool TcpAcceptor::isInterrupted() const noexcept
	{
		return impl->isInterrupted();
	}

	
	
}
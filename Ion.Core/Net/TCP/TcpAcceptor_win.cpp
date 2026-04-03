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


			ADDRINFOA hints;
			PADDRINFOA temp_res;

			memset(&hints, 0, sizeof(hints)); //zero out struct
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
				throw NetworkException("Setting up socket environment in windows failed. getaddrinfo"
					, std::error_code(getaddrinfoResult, std::system_category()));
			}

			auto deleter = [](PADDRINFOA p) noexcept {::freeaddrinfo(p); };

			std::unique_ptr<ADDRINFOA, decltype(deleter)> res(temp_res, deleter);

			auto s = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

			if (INVALID_SOCKET == s)
			{
				throw NetworkException("Setting up socket environment in windows failed. socket"
					, std::error_code(WSAGetLastError(), std::system_category()));
			}

			WinSock ws{ s };

			auto setsockoptResult = setsockopt(ws.get(), SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(yes));

			if (0 != setsockoptResult)
			{
				throw NetworkException("Setting up socket environment in windows failed. setsockopt"
					, std::error_code(WSAGetLastError(), std::system_category()));
			}




			auto bindResult = bind(ws.get(), res->ai_addr, res->ai_addrlen);

			if (0 != bindResult)
			{
				throw NetworkException("Setting up socket environment in windows failed. bind"
					, std::error_code(WSAGetLastError(), std::system_category()));
			}

			auto listenResult = listen(ws.get(), 10);

			if (0 != listenResult)
			{
				throw NetworkException("Setting up socket environment in windows failed. listen"
					, std::error_code(WSAGetLastError(), std::system_category()));
			}

			m_socket = std::move(ws);


			m_stopCallback.emplace(std::move(token), [&socket = m_socket, &interrupted = m_interrupted]() {

				//on windows closesocket is the correct way to interrupt a blocking accept call
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
				return std::unexpected(std::error_code(WSAGetLastError(), std::system_category()));
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

		

		auto utilityFormException() const {
			return NetworkException("Setting up socket environment in windows failed. listen",std::error_code(WSAGetLastError(), std::system_category()));
		}
		
		

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

		return TcpConnection(res.value(), *m_env);
		
	}

	bool TcpAcceptor::isInterrupted() const noexcept
	{
		return impl->isInterrupted();
	}

	
	
}
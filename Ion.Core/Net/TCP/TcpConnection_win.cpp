#include "pch.h"

#include <expected>
#include <limits>

#include <winsock2.h>
#include <ws2tcpip.h>  
#define WIN32_LEAN_AND_MEAN  // Say this...

#include <windows.h>         // And now we can include that.
#include <winsock2.h>        // And this.

#pragma comment(lib, "ws2_32.lib")


#include "TcpConnection.hpp"
#include "Utility/Descriptor.hpp"

namespace Ion::Net::TCP
{

	using RecvResult_t = std::expected<std::span<std::byte>, std::error_code>;
	using SendResult_t = std::expected<std::span<const std::byte>, std::error_code>;
	

	class TcpConnection::TcpConnectionImpl
	{



		using WinSock = Ion::Utility::Descriptor<SOCKET, closesocket, INVALID_SOCKET>;

	public:
		TcpConnectionImpl(SOCKET s)
			: m_ws(s)
		{

		}

		auto getHandle() const noexcept
		{
			return m_ws.get();
		}
		
	
		RecvResult_t recv(std::span<std::byte> buffer)
		{

#ifdef max
#undef max
#endif // max

			if (buffer.size() > static_cast<size_t>(std::numeric_limits<int>::max()))
			{
				//
			}



			//avoid narrowing
			//reinterpret cast fine here???
			auto bytesCount = ::recv(m_ws.get(), reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);

			if (SOCKET_ERROR == bytesCount)
			{
				return std::unexpected(std::error_code(WSAGetLastError(), std::system_category()));
			}

			
			auto receivedData = buffer.subspan(0, bytesCount);

			return receivedData;
		}

		SendResult_t send(std::span<const std::byte> buffer)
		{

			


			//avoid narrowing
			auto bytesCount = ::send(m_ws.get(), reinterpret_cast<const char*>(buffer.data()), buffer.size(), 0);

			if (SOCKET_ERROR == bytesCount)
			{
				return std::unexpected(std::error_code(WSAGetLastError(), std::system_category()));
			}

			
			auto remainingToSendData = buffer.subspan(bytesCount);

			return remainingToSendData;
		}


	private:
		WinSock m_ws;

		
	};



	TcpConnection::TcpConnection(TcpConnection&& other) = default;
	TcpConnection& TcpConnection::operator=(TcpConnection&& other) = default;

	Net::Types::OpaqueHandle TcpConnection::getHandle() const noexcept
	{
		return m_impl->getHandle();
	}

	TcpConnection::TcpConnection(Net::Types::OpaqueHandle opaqueHandle, const NetworkEnvironment& env)
		:m_env(&env),m_impl(std::make_unique<TcpConnectionImpl>(opaqueHandle))
	{
	}

	TcpConnection::~TcpConnection() = default;

	RecvResult_t TcpConnection::recv(std::span<std::byte> buffer)
	{
		return m_impl->recv(buffer);
	}

	SendResult_t TcpConnection::send(std::span<const std::byte> buffer)
	{
		return m_impl->send(buffer);
	}

}
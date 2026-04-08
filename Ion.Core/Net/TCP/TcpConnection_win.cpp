#include "pch.h"

#include <expected>
#include <limits>

#define NOMINMAX



#define WIN32_LEAN_AND_MEAN  // Say this...

#include <windows.h>         // And now we can include that.
#include <winsock2.h>        // And this.
#include <mswsock.h>
#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "ws2_32.lib")


#include "TcpConnection.hpp"
#include "Utility/Descriptor.hpp"

namespace Ion::Net::TCP
{

	using RecvResult_t = std::expected<std::span<std::byte>, std::error_code>;
	using SendResult_t = std::expected<std::span<const std::byte>, std::error_code>;
	

	class TcpConnection::TcpConnectionImpl
	{



		using WinSock = Ion::Utility::Descriptor<SOCKET, ::closesocket, INVALID_SOCKET>;

	public:
		TcpConnectionImpl(SOCKET s) noexcept
			: m_socket(s)
		{

		}

		auto getHandle() const noexcept
		{
			return m_socket.get();
		}
		
	
		RecvResult_t recv(std::span<std::byte> buffer)
		{


			if (buffer.size() > std::numeric_limits<int>::max())
			{
				return std::unexpected(std::make_error_code(std::errc::value_too_large));
			}

			// std::byte* -> char* legitimate for socket I/O

			auto bytesCount = ::recv(m_socket.get(), reinterpret_cast<char*>(buffer.data()), buffer.size(), 0);

			if (SOCKET_ERROR == bytesCount)
			{
				return std::unexpected(std::error_code(::WSAGetLastError(), std::system_category()));
			}

			if (bytesCount == 0) {
				return std::unexpected(std::make_error_code(std::errc::connection_aborted));
			}

			
			auto receivedData = buffer.subspan(0, bytesCount);

			return receivedData;
		}

		SendResult_t send(std::span<const std::byte> buffer)
		{

			


			//avoid narrowing

			if (buffer.size() > std::numeric_limits<int>::max())
			{
				return std::unexpected(std::make_error_code(std::errc::value_too_large));
			}

			int buffer_size = static_cast<int>(buffer.size());


			// std::byte* -> char* legitimate for socket I/O

			auto bytesCount = ::send(m_socket.get(), reinterpret_cast<const char*>(buffer.data()), buffer_size, 0);

			if (SOCKET_ERROR == bytesCount)
			{
				return std::unexpected(std::error_code(::WSAGetLastError(), std::system_category()));
			}

			
			auto remainingToSendData = buffer.subspan(bytesCount);

			return remainingToSendData;
		}

		[[nodiscard]]
		std::expected<void,std::error_code> sendfile(fs::path&& path, std::span< const std::byte> head, std::span< const std::byte> tail)
		{
			
			auto handle = ::CreateFileW(
				path.c_str(),
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL, //lpSecurityAttributes
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);

			if (handle == INVALID_HANDLE_VALUE)
			{
				return std::unexpected(std::error_code(::GetLastError(),std::system_category()));
			}

			Utility::Descriptor<HANDLE,::CloseHandle, INVALID_HANDLE_VALUE> fileHandle(handle);

			TRANSMIT_FILE_BUFFERS transmitFileBuffers
			{
				.Head = const_cast<std::byte*>(head.data()),
				.HeadLength = static_cast<DWORD>(head.size()),
				.Tail = const_cast<std::byte*>(tail.data()),
				.TailLength = static_cast<DWORD>(tail.size())
			};


			auto transmitResult = ::TransmitFile(
				m_socket.get(),
				fileHandle.get(),
				0, //0 -> transmit entire file
				0, //default number of bytes per send
				NULL, //not overlapped
				&transmitFileBuffers,
				NULL

			);

			if (transmitResult == FALSE)
			{
				return std::unexpected(std::error_code(::GetLastError(), std::system_category()));
			}

			return {};
		}



	private:
		WinSock m_socket;

		
	};



	TcpConnection::TcpConnection(TcpConnection&& other) noexcept = default;
	TcpConnection& TcpConnection::operator=(TcpConnection&& other) noexcept = default;

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
	
	std::expected<void, std::error_code>  TcpConnection::sendfile(fs::path path, std::span< const std::byte> head, std::span< const std::byte> tail)
	{
		return m_impl->sendfile(std::move(path),head,tail);
	}

}
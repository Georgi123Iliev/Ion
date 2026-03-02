#pragma once
#include<expected>
#include <system_error>
#include <memory>
#include<span>

#include "Net/Types.hpp"
#include "Net/NetworkEnvironment.hpp"

namespace Ion::Net::TCP
{
	

	class TcpConnection
	{
		
	public:

		

		

		TcpConnection(Net::Types::OpaqueHandle opaqueHandle, const NetworkEnvironment& env);


		TcpConnection(TcpConnection&& other);
		TcpConnection& operator=(TcpConnection&& other);
	
		[[nodiscard]] Net::Types::OpaqueHandle getHandle() const noexcept;

		

		
		[[nodiscard]] std::expected<std::span<std::byte>, std::error_code> recv(std::span<std::byte> buffer);

		[[nodiscard]] std::expected<std::span<const std::byte>, std::error_code> send(std::span< const std::byte> buffer);

		~TcpConnection();
	private:

		
		const NetworkEnvironment* m_env;
		class TcpConnectionImpl;
		std::unique_ptr<TcpConnectionImpl> m_impl;

	};
}


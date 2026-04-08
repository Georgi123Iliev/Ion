#pragma once
#include<expected>
#include <system_error>
#include <memory>
#include <span>
#include <filesystem>

#include "Net/Types.hpp"
#include "Net/NetworkEnvironment.hpp"

namespace Ion::Net::TCP
{
	namespace fs = std::filesystem;

	class TcpConnection
	{
		
	public:

		

		

		TcpConnection(Net::Types::OpaqueHandle opaqueHandle, const NetworkEnvironment& env);


		TcpConnection(TcpConnection&& other) noexcept;
		TcpConnection& operator=(TcpConnection&& other) noexcept;
	
		[[nodiscard]] Net::Types::OpaqueHandle getHandle() const noexcept;
		
		[[nodiscard]] std::expected<std::span<std::byte>, std::error_code> recv(std::span<std::byte> buffer);

		[[nodiscard]] std::expected<std::span<const std::byte>, std::error_code> send(std::span< const std::byte> buffer);

		[[nodiscard]] std::expected<void, std::error_code> sendfile(fs::path path, std::span< const std::byte> head, std::span< const std::byte> tail);

		~TcpConnection();
	private:

		
		const NetworkEnvironment* m_env;
		class TcpConnectionImpl;
		std::unique_ptr<TcpConnectionImpl> m_impl;

	};
}


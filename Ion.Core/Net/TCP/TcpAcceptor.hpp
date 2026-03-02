#pragma once
#include <memory>
#include <expected>
#include <system_error>

#include "Net/Types.hpp"
#include "TcpConnection.hpp"
#include "Net/NetworkEnvironment.hpp"

namespace Ion::Net::TCP
{

	class TcpAcceptor
	{
		using AcceptResult = std::expected<Net::Types::OpaqueHandle, std::error_code>;
	public:
		TcpAcceptor(const NetworkEnvironment& env);
		~TcpAcceptor();

		[[nodiscard]]
		std::expected<TcpConnection, std::error_code> accept() noexcept;


	private:

		class TcpAcceptorImpl;
		const NetworkEnvironment* m_env;
		std::unique_ptr<TcpAcceptorImpl> impl;


	};
}

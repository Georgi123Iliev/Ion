#pragma once
#include <memory>
#include <expected>
#include <system_error>

#include "TcpConnection.hpp"

class TcpAcceptor
{

	TcpAcceptor();
	~TcpAcceptor();

	// accept

	std::expected<TcpConnection,std::error_code> accept() noexcept;


private:

	class TcpAcceptorImpl;

	std::unique_ptr<TcpAcceptorImpl> impl;

	
};


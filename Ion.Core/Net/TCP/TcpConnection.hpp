#pragma once
#include <cstdarg>
#include <memory>
class TcpConnection
{

public:
	using opaqueHandle_t = std::uintptr_t;

	explicit TcpConnection(opaqueHandle_t opaqueHandle);

	//send(std::span<char>);

	//recv

	~TcpConnection();
private:

	
	
	class TcpConnectionImpl;
	std::unique_ptr<TcpConnectionImpl> impl;

};


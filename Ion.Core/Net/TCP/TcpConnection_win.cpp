#include "pch.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#define WIN32_LEAN_AND_MEAN  // Say this...

#include <windows.h>         // And now we can include that.
#include <winsock2.h>        // And this.

#pragma comment(lib, "ws2_32.lib")
#include "TcpConnection.hpp"
#include "Utility/Descriptor.hpp"



TcpConnection::TcpConnection(opaqueHandle_t opaqueHandle)
	:impl(std::make_unique<TcpConnectionImpl>(opaqueHandle))
{
}

TcpConnection::~TcpConnection() = default;


class TcpConnection::TcpConnectionImpl
{
	using WinSock = Ion::Utility::Descriptor<SOCKET, closesocket, INVALID_SOCKET>;

public:
	TcpConnectionImpl(SOCKET s)
		: ws(s)
	{

	}
private:
	WinSock ws;
};
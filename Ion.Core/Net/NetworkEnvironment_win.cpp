#include "pch.h"
#include "NetworkEnvironment.hpp"
#include "Net/Exceptions/NetworkException.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>
#define WIN32_LEAN_AND_MEAN  // Say this...

#include <windows.h>         // And now we can include that.
#include <winsock2.h>        // And this.

#pragma comment(lib, "ws2_32.lib")

#include<stdexcept>


class NetworkEnvironment::EnvironmentImpl
{
public:

	EnvironmentImpl()
	{
		using namespace Ion::Net;

		//WSAStartup params
		int wsaResult;
		WORD wVersionRequested = MAKEWORD(2, 2); //version 2.2 of winsockets
		WSADATA wsaData;

		wsaResult = WSAStartup(
			wVersionRequested,
			&wsaData
		);

		if (0 != wsaResult)
		{
			throw NetworkException("Setting up socket environment in windows failed. WSAStartup"
				, std::error_code(wsaResult, std::system_category()));
		}
	}

	~EnvironmentImpl()
	{
		WSACleanup();
	}
};

NetworkEnvironment::NetworkEnvironment()
{
	if (m_exists)
	{
		throw(std::logic_error("Only one environment can exist at a time"));
	}

	m_impl = std::make_unique<EnvironmentImpl>();
	m_exists.store(true);
}

NetworkEnvironment::~NetworkEnvironment() = default;
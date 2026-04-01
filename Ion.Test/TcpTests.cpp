#include "pch.h"


#include "Net/TCP/TcpAcceptor.hpp"
#include "Threading/ThreadPool.hpp"
#include "Parsing/ParsingWrapper.hpp"
#include "Utility/Conversions.hpp"

using namespace Ion::Net::TCP;
using namespace Ion::Threading;
using namespace Ion::Net;

TEST(TcpTests, GetConnection) {

	/*
	NetworkEnvironment env;

	TcpAcceptor acc(env);

	auto receiverRes = acc.accept();

	if (!receiverRes)
	{
		FAIL() << "accepting connection failed" << receiverRes.error();
	}

	std::string message = "Hello";

	auto& receiver = receiverRes.value();

	auto f = [conn = std::move(receiver), &message]() mutable {

		constexpr int size = 1024;
		std::byte arr[size];

		while (true)
		{

			auto readBytesRes = conn.recv(arr);

			if (!readBytesRes)
			{
				FAIL() << "recv failed" << readBytesRes.error();
			}

			auto readBytes = readBytesRes.value();

			if (readBytes.size() == 0)
				return;


			parse(toStringView(readBytes));

			


			//ASSERT_EQ(memcmp(readBytes.data(), message.c_str(), readBytes.size()), 0);

		}
		};
		

	ThreadPool tr;


	tr.dispatch(std::move(f));
	
	

	*/


}
#include <unordered_map>

#include "Net/HTTP/HttpServer.hpp"
#include "Net/NetworkEnvironment.hpp"
#include "Net/HTTP/Router.hpp"
#include "Net/HTTP/RouterBuilder.hpp"
#include "Utility/Conversions.hpp"

int main()
{

	using namespace Ion::Net::HTTP;
	using namespace Ion::Net::TCP;

	HttpHandler handler = [](TcpConnection& conn,
		const HttpHead& httpHead,
		std::span<const std::byte> spillover) -> std::span<const std::byte> {
			
			std::string_view sv = R"(HTTP/1.1 200 OK
				Content - Type: text / html
				Content - Length : 44

				<html><body><h1>Hello World!</h1></body></html>)";
			
			std::ignore = conn.send(Ion::Utility::toByteSpan(sv));


			
			return spillover;
		
		
		
		};



	RouterBuilder rb;

	//rb.add(...)

	rb.addStaticRoute(HttpMethod::GET, Path{ "/notAFile" }, handler);

	Ion::Net::NetworkEnvironment env;
	Ion::Net::HTTP::HttpServer server(env,rb.consume());

	server.actionLoop();
}
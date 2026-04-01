#include "pch.h"
#include "Router.hpp"

#include "Utility/Conversions.hpp"

std::span<const std::byte> Ion::Net::HTTP::notFound(TCP::TcpConnection& conn, const HttpHead& httpHead, std::span<const std::byte> spillover) {

	std::string_view notFoundResponse = "HTTP/1.1 404 Not Found\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: 13\r\n"
    "Connection: close\r\n"
    "\r\n"
        "404 Not Found";

    auto responseBytes = Utility::toByteSpan(notFoundResponse);


    std::ignore = conn.send(responseBytes);

	return spillover;
}

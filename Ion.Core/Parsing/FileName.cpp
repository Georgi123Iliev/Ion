#include "pch.h"

#include<string_view>
#include<span>

#include "picohttpparser.h"


struct ParseResult {

    int result;
    std::string_view method;
    std::string_view path;
    int minorVersion;
};

auto parse(std::string_view msg, std::span<phr_header> headers, size_t& lastLen)
{
    const char* method_ptr;
    size_t method_len;

    const char* path_ptr;
    size_t path_len;

    struct phr_header* headers_internal = headers.data();
    size_t num_headers = headers.size();

    int minor_version;

    int result = phr_parse_request(msg.data(), msg.length(), &method_ptr, &method_len, &path_ptr, &path_len, &minor_version, headers_internal, &num_headers, lastLen);

    ParseResult pr =
    {
        .result = result,
        .method = {method_ptr,method_len},
        .path = {path_ptr,path_len},
        .minorVersion = minor_version

    };

    return pr;
}

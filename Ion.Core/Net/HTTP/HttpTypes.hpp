#pragma once

#include<string_view>
#include<span>
#include<array>

#include<algorithm>
#include<ranges>

#include "Parsing/ParsingWrapper.hpp"

#include "Net/Constants.hpp"

namespace Ion::Net::HTTP
{


    

    struct HttpMethod
    {

        static const HttpMethod GET;
        static const HttpMethod POST;
 
       explicit HttpMethod(std::string_view method)
            : method(method)
        {

        }

        



        bool operator==(const HttpMethod& other) const
        {
            return method == other.method;
        }

        std::string_view method;
    };

     inline const auto HttpMethod::GET = HttpMethod{ "GET" };
     inline const auto HttpMethod::POST = HttpMethod{ "POST" };


    struct Path
    {
    
        Path(std::string_view path)
            : path(path)
        {

        }

        bool operator==(const Path& other) const
        {
            return path == other.path;
        }


        std::string_view path;
    };

    

    struct HttpHead
    {

        struct header
        {
            std::string_view name;
            std::string_view value;
        };

        HttpMethod method;
        Path path;
        int minorVersion;
        std::string_view entireHead;
      


        explicit HttpHead(const Ion::Parsing::ParseResult& pr)
            :method(pr.method), path(pr.path),
            minorVersion(pr.minorVersion), entireHead(pr.wholeMsg.substr(0, pr.result)),
            m_raw_headers(pr.headers)
        {
            
                     
        }

        auto headers() const
        {
            auto to_internal = [](const auto& h) {
                return header
                {
                    .name = {h.name, h.name_len},
                    .value = {h.value, h.value_len}
                };
            };

            return m_raw_headers | std::views::transform(to_internal);
        }

    private:
        
        
        std::span<phr_header> m_raw_headers;

    };


    template<typename T, typename keyT, typename valT>
    concept is_map_like =
        requires(T t, keyT k)
    {
        { t.find(k) != t.end() } -> std::same_as<bool>;
        { std::remove_reference_t<decltype(t.find(k)->second)>() } -> std::same_as<valT>;
    };






}




//Hasher methods
namespace std
{


    template<>
    struct std::hash<Ion::Net::HTTP::HttpMethod>
    {
        std::size_t operator()(const Ion::Net::HTTP::HttpMethod& m) const noexcept
        {
            return std::hash<std::string_view>{}(m.method);
        }
    };

    template<>
    struct std::hash<Ion::Net::HTTP::Path>
    {
        std::size_t operator()(const Ion::Net::HTTP::Path& p) const noexcept
        {
            return std::hash<std::string_view>{}(p.path);
        }
    };

}
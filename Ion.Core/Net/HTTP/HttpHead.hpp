#pragma once
#include "pch.h"

#include<string_view>
#include<span>
#include<array>

#include<algorithm>
#include<ranges>

#include "Parsing/ParsingWrapper.hpp"

#include "Net/Constants.hpp"

namespace Ion::Net::HTTP
{

    struct header
    {
        std::string_view name;
        std::string_view value;
    };

    struct HttpHead
    {


        std::string_view method;
        std::string_view path;
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
                return header{
                    .name = {h.name, h.name_len},
                    .value = {h.value, h.value_len}
                };
                };

            return m_raw_headers | std::views::transform(to_internal);
        }

    private:
        
        
        std::span<phr_header> m_raw_headers;

    };
}
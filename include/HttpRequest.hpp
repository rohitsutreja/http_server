#pragma once

#include "HttpMethod.h"

#include <string>
#include <optional>
#include <unordered_map>

struct HttpRequest
{
    HttpMethod method{HttpMethod::UNKNOWN};
    std::string url{};
    std::string version{};
    std::unordered_map<std::string, std::string> headers{};
    std::string body{};

    std::optional<std::string> get_header(const std::string header_name)
    {
        const auto header_it{headers.find(header_name)};
        if (header_it == headers.end())
        {
            return std::nullopt;
        }

        return headers.at(header_name);
    }
};

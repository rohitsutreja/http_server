#pragma once

#include "HttpMethod.hpp"

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
    std::unordered_map<std::string, std::string> params{};

    std::optional<std::string> get_header(const std::string &header_name)
    {
        const auto header_it{headers.find(header_name)};
        if (header_it == headers.end())
        {
            return std::nullopt;
        }

        return headers.at(header_name);
    }

    std::optional<std::string> get_param(const std::string &param_name)
    {
        const auto param_it{params.find(param_name)};
        if (param_it == params.end())
        {
            return std::nullopt;
        }

        return params.at(param_name);
    }
};

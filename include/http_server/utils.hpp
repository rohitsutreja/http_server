#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <functional>

#include "http_server/HttpRequest.hpp"
#include "http_server/HttpResponse.hpp"

namespace http_server
{

    namespace details
    {
        inline std::vector<std::string> split(const std::string &str, char delimiter)
        {
            std::stringstream ss(str);
            std::string item;
            std::vector<std::string> tokens;
            while (std::getline(ss, item, delimiter))
            {
                if (!item.empty())
                {
                    {
                        tokens.push_back(item);
                    }
                }
            }
            return tokens;
        };

    }
}
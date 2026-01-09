#include "HttpMethod.hpp"

#include <unordered_map>
#include <optional>

struct HttpResponse
{
    int status_code = 200;
    std::string status_message = "OK";
    std::unordered_map<std::string, std::string> headers{};
    std::string body{};

    std::string to_string()
    {
        std::string response{"HTTP/1.1 " + std::to_string(status_code) + " " + status_message + "\r\n"};
        headers.emplace("Content-Length", std::to_string(body.size()));
        for (const auto &pair : headers)
        {
            response += pair.first;
            response += ": ";
            response += pair.second;
            response += "\r\n";
        }

        response += "\r\n";

        response += body;

        return response;
    }
};

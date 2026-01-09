#include <cstdint>
#include <string_view>
#include <string>

enum class HttpMethod : uint8_t
{
    GET,
    POST,
    PUT,
    DELETE,
    HEAD,
    OPTIONS,
    UNKNOWN
};

inline HttpMethod string_to_method(std::string_view s)
{
    if (s == "GET")
        return HttpMethod::GET;
    if (s == "POST")
        return HttpMethod::POST;
    if (s == "PUT")
        return HttpMethod::PUT;
    if (s == "DELETE")
        return HttpMethod::DELETE;
    if (s == "HEAD")
        return HttpMethod::HEAD;
    if (s == "OPTIONS")
        return HttpMethod::OPTIONS;
    return HttpMethod::UNKNOWN;
}

inline std::string method_to_string(HttpMethod m)
{
    switch (m)
    {
    case HttpMethod::GET:
        return "GET";
    case HttpMethod::POST:
        return "POST";
    case HttpMethod::PUT:
        return "PUT";
    case HttpMethod::DELETE:
        return "DELETE";
    case HttpMethod::HEAD:
        return "HEAD";
    case HttpMethod::OPTIONS:
        return "OPTIONS";
    default:
        return "UNKNOWN";
    }
}
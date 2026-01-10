#include <optional>
#include <string_view>

#include "HttpRequest.hpp"

namespace http_server
{
    namespace details
    {

        inline std::optional<HttpRequest> parse(std::string_view raw_request)
        {
            HttpRequest req{};

            size_t line_end = raw_request.find("\r\n");
            if (line_end == std::string_view::npos)
            {
                return std::nullopt;
            }

            // 2. Isolate the first line
            auto first_line = raw_request.substr(0, line_end);

            // 3. Move the master cursor past the first line
            raw_request.remove_prefix(line_end + 2);

            // --- PARSE METHOD ---
            size_t method_end = first_line.find(' ');
            if (method_end == std::string_view::npos)
                return std::nullopt;

            req.method = string_to_method(first_line.substr(0, method_end));

            // Chop off method
            first_line.remove_prefix(method_end + 1);

            // --- PARSE URI ---
            size_t uri_end = first_line.find(' ');
            if (uri_end == std::string_view::npos)
                return std::nullopt;

            req.url = std::string(first_line.substr(0, uri_end));

            // Chop off URI
            first_line.remove_prefix(uri_end + 1);

            // --- PARSE VERSION ---

            // Whatever is left is the version (e.g., "HTTP/1.1")
            req.version = std::string(first_line);

            while (true)
            {
                // 1. Check for End of Headers (Empty Line)
                if (raw_request.starts_with("\r\n"))
                {
                    // Remove Empty Line,  Only body will remain.
                    raw_request.remove_prefix(2);
                    break;
                }

                auto header_line_end = raw_request.find("\r\n");
                if (header_line_end == std::string_view::npos)
                {
                    return std::nullopt;
                }

                // Isolate the header line
                auto header_line = raw_request.substr(0, header_line_end);

                // Chop off this header line from the req
                raw_request.remove_prefix(header_line_end + 2);

                // Split Key: Value
                auto semicolon_pos = header_line.find(":");
                if (semicolon_pos == std::string_view::npos)
                {
                    return std::nullopt;
                }

                auto header_key = header_line.substr(0, semicolon_pos);
                auto header_value = header_line.substr(semicolon_pos + 1);

                if (!header_value.empty() && header_value.front() == ' ')
                {
                    header_value.remove_prefix(1);
                }

                req.headers.emplace(std::string(header_key), std::string(header_value));
            }

            req.body = std::string{raw_request};

            return req;
        }
    }

}
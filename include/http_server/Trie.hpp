#include <string>
#include <unordered_map>
#include <functional>
#include <memory>

#include "utils.hpp"
#include "http_server/HttpRequest.hpp"
#include "http_server/HttpResponse.hpp"

namespace http_server
{
    namespace details
    {
        struct TrieNode
        {
            std::unordered_map<std::string, std::unique_ptr<TrieNode>> children;
            std::unique_ptr<TrieNode> param_child = nullptr;
            std::string param_name;

            std::unordered_map<HttpMethod, HttpHandler> method_handlers;

            bool is_end = false;
        };
    } // namespace details

} // namespace http_server

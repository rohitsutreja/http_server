#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "HttpMethod.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Trie.hpp"
#include "utils.hpp"

namespace http_server
{
    class Router
    {
    public:
        Router() : _root{std::make_unique<details::TrieNode>()} {}

        HttpResponse route(HttpRequest &req);

        void add_route_handler(HttpMethod method, const std::string &path, const HttpHandler &handler);

    private:
        std::unique_ptr<details::TrieNode> _root;
    };
}
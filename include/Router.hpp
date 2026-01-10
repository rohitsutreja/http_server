#pragma once

#include <map>
#include <string>
#include <assert.h>
#include <functional>

#include "HttpMethod.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

namespace http_server
{

    using HttpHandler = std::function<HttpResponse(const HttpRequest &)>;

    class Router
    {
    public:
        Router() = default;

        HttpResponse route(const HttpRequest &req)
        {
            {

                Route route{req.method, req.url};

                auto it = _routes.find(route);

                if (it != _routes.end())
                {
                    return it->second(req);
                }

                HttpResponse res;
                res.status_code = 404;
                res.body = "<h1>404 Not Found</h1><p>The router could not match this path.</p>";
                res.headers["Content-Type"] = "text/html";
                res.headers["Content-Length"] = std::to_string(res.body.size());
                return res;
            }
        }
        void Get(std::string path, const HttpHandler &handler)
        {
            _add(HttpMethod::GET, path, handler);
        }

        void Put(std::string path, const HttpHandler &handler)
        {
            _add(HttpMethod::PUT, path, handler);
        }

        void Patch(std::string path, const HttpHandler &handler)
        {
            _add(HttpMethod::PATCH, path, handler);
        }

        void Post(std::string path, const HttpHandler &handler)
        {
            _add(HttpMethod::POST, path, handler);
        }

        void Delete(std::string path, const HttpHandler &handler)
        {
            _add(HttpMethod::DELETE, path, handler);
        }

    private:
        using Route = std::pair<HttpMethod, std::string /* path */>;

        void _add(HttpMethod method, std::string path, const HttpHandler &handler)
        {
            assert(method != HttpMethod::UNKNOWN);

            Route route{method, path};
            _routes.emplace(route, handler);
        }

        std::map<Route, HttpHandler> _routes;
    };
}
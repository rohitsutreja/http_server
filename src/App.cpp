#include "http_server/App.hpp"

namespace http_server
{

    void App::get(const std::string &path, const HttpHandler &handler)
    {
        router.add_route_handler(HttpMethod::GET, path, handler);
    }

    void App::post(const std::string &path, const HttpHandler &handler)
    {
        router.add_route_handler(HttpMethod::POST, path, handler);
    }

    void App::put(const std::string &path, const HttpHandler &handler)
    {
        router.add_route_handler(HttpMethod::PUT, path, handler);
    }

    void App::patch(const std::string &path, const HttpHandler &handler)
    {
        router.add_route_handler(HttpMethod::PATCH, path, handler);
    }

    void App::delete_(const std::string &path, const HttpHandler &handler)
    {
        router.add_route_handler(HttpMethod::DELETE, path, handler);
    }

    void App::start()
    {
        server.set_handler([this](HttpRequest &req) -> HttpResponse
                           { return router.route(req); });
        server.run();
    }

}
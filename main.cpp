#include <iostream>

#include "Server.hpp"
#include "Router.hpp"

int main()
{
    try
    {
        http_server::Router router{};

        router.Get("/", [](const HttpRequest &)
                   {
            HttpResponse res{};
            res.status_code = 200;
            res.status_message = "OK";
            res.body = "<h1>Hello World</h1>";

            res.headers.emplace("Content-Type", "text/html");
            res.headers.emplace("Connection", "close");

            return res; });

        router.Get("/home", [](const HttpRequest &)
                   {
            HttpResponse res{};
            res.status_code = 200;
            res.status_message = "OK";
            res.body = "<h1>Home!</h1>";

            res.headers.emplace("Content-Type", "text/html");
            res.headers.emplace("Connection", "close");

            return res; });

        http_server::Server server{8000};

        server.set_handler([&router](const HttpRequest &req) -> HttpResponse
                           { return router.route(req); });

        server.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Fatal Error] " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
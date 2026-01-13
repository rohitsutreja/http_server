#include <iostream>

#include "http_server/App.hpp"

int main()
{
    try
    {
        http_server::App app{8000};

        app.get("/", [](const http_server::HttpRequest &)
                {
            http_server::HttpResponse res{};
            res.status_code = 200;
            res.status_message = "OK";
            res.body = "<h1>Hello World</h1>";

            res.headers.emplace("Content-Type", "text/html");
            res.headers.emplace("Connection", "close");

            return res; });

        app.get("/home", [](const http_server::HttpRequest &)
                {
             http_server::HttpResponse res{};
            res.status_code = 200;
            res.status_message = "OK";
            res.body = "<h1>Home!</h1>";

            res.headers.emplace("Content-Type", "text/html");
            res.headers.emplace("Connection", "close");

            return res; });

        app.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Fatal Error] " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
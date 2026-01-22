#include <iostream>
#include <chrono>
#include "http_server/App.hpp"

using namespace http_server;

int main()
{
    try
    {
        App app{8000};

        app.use([](HttpRequest &req, NextFunction next)
        {
            auto start = std::chrono::high_resolution_clock::now();
            std::cout << "[LOG] IN: " << req.url << std::endl;

            HttpResponse res = next(req);

            auto end = std::chrono::high_resolution_clock::now();
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            std::cout << "[LOG] OUT: " << res.status_code << " (" << ms << "ms)" << std::endl;
            return res; 
        });

        app.use([](HttpRequest &req, NextFunction next)
        {
            if (req.url == "/admin") {
                return HttpResponse(401, "Unauthorized Access!");
            }
            return next(req); 
        });

        app.get("/", [](const HttpRequest &)
        {
            HttpResponse res;
            res.body = "<h1>Welcome to C++ HTTP Server!</h1>";
            res.status_code = 200;
            res.status_message = "OK";
            res.headers["Content-Type"] = "text/html";
            return res; 
        });

        app.get("/admin", [](const HttpRequest &)
        { 
            return HttpResponse(200, "Welcome, Admin."); 
        });

        app.start();
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Fatal] " << e.what() << std::endl;
    }
    return 0;
}
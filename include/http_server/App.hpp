#include "http_server/Server.hpp"
#include "http_server/Router.hpp"
#include "http_server/HttpRequest.hpp"

namespace http_server
{
    using NextFunction = std::function<HttpResponse(HttpRequest &)>;
    using Middleware = std::function<HttpResponse(HttpRequest &, NextFunction)>;

    class App
    {
    public:
        App(uint16_t port) : _server(port) {}

        void use(const Middleware &middleware);

        void get(const std::string &path, const HttpHandler &handler);
        void post(const std::string &path, const HttpHandler &handler);
        void put(const std::string &path, const HttpHandler &handler);
        void patch(const std::string &path, const HttpHandler &handler);
        void delete_(const std::string &path, const HttpHandler &handler);

        void start();

    private:
        HttpResponse _dispatch(int index, HttpRequest &req, NextFunction finalHandler);

        std::vector<Middleware> _middlewares;

        Server _server;
        Router _router;
    };
}
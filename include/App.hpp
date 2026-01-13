#include "Server.hpp"
#include "Router.hpp"

namespace http_server
{
    class App
    {
    public:
        App(uint16_t port) : server(port) {}

        void get(const std::string &path, const HttpHandler &handler);
        void post(const std::string &path, const HttpHandler &handler);
        void put(const std::string &path, const HttpHandler &handler);
        void patch(const std::string &path, const HttpHandler &handler);
        void delete_(const std::string &path, const HttpHandler &handler);

        void start();

    private:
        Server server;
        Router router;
    };
}
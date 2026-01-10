#pragma once

#include "HttpParser.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpMethod.hpp"
#include "Socket.hpp"
#include "Epoll.hpp"
#include "Status.hpp"
#include "Router.hpp"

#include <sys/socket.h>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <stdexcept>
#include <iostream>
#include <span>
#include <unordered_map>
#include <cstdint>
#include <array>

namespace http_server
{

    class Server
    {
    public:
        explicit Server(uint16_t port);
        void run();

        void set_handler(const HttpHandler &);

    private:
        void _handle_accept();

        void _handle_client_data(int fd);

        uint16_t _port;
        bool _running{};

        details::Socket _server_socket{};
        details::Epoll _epoll_instance{};

        HttpHandler _handler;

        std::unordered_map<int, details::Socket> _clients{};
    };

}
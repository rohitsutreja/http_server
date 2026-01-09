#pragma once

#include "Socket.hpp"
#include "Epoll.hpp"
#include "Status.hpp"

#include <sys/socket.h>
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

class Server
{
public:
    explicit Server(uint16_t port);
    void run();

private:
    void _handle_accept();

    void _handle_client_data(int fd);

    uint16_t _port;
    bool _running{};

    Socket _server_socket{};
    Epoll _epoll_instance{};

    std::unordered_map<int, Socket> _clients{};
};
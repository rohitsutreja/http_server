#pragma once

#include "Status.hpp"

#include <string>
#include <optional>
#include <string_view>
#include <cstdint>
#include <span>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <stdexcept>
#include <iostream>

class Socket
{
public:
    Socket();

    void bind(uint16_t port);
    void listen();

    std::optional<Socket> accept();

    IoResult read(std::span<char> buffer);
    IoResult write(std::string_view data);

    ~Socket();

    Socket(const Socket &) = delete;
    Socket &operator=(const Socket &) = delete;

    Socket(Socket &&other);
    Socket &operator=(Socket &&other);

    int get_fd() const { return _fd; }

private:
    explicit Socket(int fd);

    int _fd{-1};
};
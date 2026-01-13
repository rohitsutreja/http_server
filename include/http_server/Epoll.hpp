#pragma once

#include "Socket.hpp"

#include <sys/epoll.h>
#include <cstdint>
#include <span>
#include <vector>

namespace http_server
{
    namespace details
    {
        class Epoll
        {
        public:
            Epoll();
            ~Epoll();

            Epoll(const Epoll &) = delete;
            Epoll &operator=(const Epoll &) = delete;

            Status add(int fd, uint32_t events);
            Status add(const Socket &socket, uint32_t events);
            int wait(std::span<epoll_event> events, int timeout = -1);

        private:
            int _fd{-1};
        };
    }
}
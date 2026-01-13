#include "http_server/Epoll.hpp"

#include <sys/epoll.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <cerrno>

namespace http_server
{
    namespace details
    {
        Epoll::Epoll()
        {
            _fd = epoll_create1(0);
            if (_fd < 0)
            {
                throw std::runtime_error(std::strerror(errno));
            }
        }

        Epoll::~Epoll()
        {
            if (_fd >= 0)
            {
                close(_fd);
            }
        }

        Status Epoll::add(int fd, uint32_t events)
        {
            struct epoll_event event{};
            event.events = events;
            event.data.fd = fd;

            return {.code = epoll_ctl(_fd, EPOLL_CTL_ADD, fd, &event)};
        }

        Status Epoll::add(const Socket &socket, uint32_t events)
        {
            return add(socket.get_fd(), events);
        }

        int Epoll::wait(std::span<epoll_event> events, int timeout)
        {
            return epoll_wait(_fd, events.data(), static_cast<int>(events.size()), timeout);
        }

    }

} // namespace http_server
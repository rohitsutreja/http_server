#include "Socket.hpp"

namespace http_server
{
    namespace details
    {

        Socket::Socket()
        {
            auto fd{socket(AF_INET, SOCK_STREAM, 0)};
            if (fd < 0)
            {
                throw std::runtime_error{std::strerror(errno)};
            }

            int opt{1};
            if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
            {
                std::cerr << "Warning: setsockopt(SO_REUSEADDR) failed\n";
            }

            _fd = fd;
        }

        void Socket::bind(uint16_t port)
        {
            struct sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = INADDR_ANY;

            if (::bind(_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
            {
                throw std::runtime_error(std::strerror(errno));
            }
        }

        void Socket::listen()
        {
            if (::listen(_fd, 10000) < 0)
            {
                throw std::runtime_error{std::strerror(errno)};
            };
        }

        std::optional<Socket> Socket::accept()
        {
            struct sockaddr_in addr{};
            socklen_t len{sizeof(addr)};

            int new_fd = ::accept(_fd, (struct sockaddr *)&addr, &len);

            if (new_fd < 0)
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    return std::nullopt;
                }

                if (errno == EMFILE || errno == ENFILE)
                {
                    std::cerr << "[Server] Error: Too many open files. Rejecting connection.\n";
                    return std::nullopt;
                }

                std::cerr << "[Server] Accept error: " << std::strerror(errno) << "\n";
                return std::nullopt;
            }

            return Socket{new_fd};
        }

        IoResult Socket::read(std::span<char> buffer)
        {
            const auto bytes{::read(_fd, buffer.data(), buffer.size())};

            if (bytes < 0)
            {
                return {.bytes = 0, .status = Status::FromErrno()};
            }

            return {.bytes = static_cast<int>(bytes), .status = Status::Success()};
        }

        IoResult Socket::write(std::string_view data)
        {
            const auto wrote_bytes{::send(_fd, data.data(), data.size(), MSG_NOSIGNAL)};

            if (wrote_bytes < 0)
            {
                return {.bytes = 0, .status = Status::FromErrno()};
            }

            return {.bytes = static_cast<int>(wrote_bytes), .status = Status::Success()};
        }

        Socket::Socket(Socket &&other)
        {
            _fd = other._fd;
            other._fd = -1;
        }

        Socket &Socket::operator=(Socket &&other)
        {
            if (this == &other)
            {
                return *this;
            }

            if (_fd >= 0)
            {
                close(_fd);
            }

            _fd = other._fd;
            other._fd = -1;

            return *this;
        }

        Socket::~Socket()
        {
            if (_fd >= 0)
            {
                close(_fd);
            }
        }

        Socket::Socket(int fd) : _fd(fd) {}
    }
}
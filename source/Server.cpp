#include "Server.hpp"

#include <cstring>
#include <iostream>

Server::Server(uint16_t port) : _port(port)
{
    _server_socket.bind(_port);
    _server_socket.listen();

    const auto status{_epoll_instance.add(_server_socket, EPOLLIN)};

    if (status.failed())
    {
        throw std::runtime_error(status.message());
    }

    std::cout << "[Server] Listening on port " << _port << "..." << std::endl;
}

void Server::run()
{
    _running = true;
    std::array<epoll_event, 64> events{};

    while (_running)
    {
        const auto n{_epoll_instance.wait(events)};

        if (n < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            std::cerr << "Epoll error: " << strerror(errno) << std::endl;
            break;
        }

        for (int i{}; i < n; ++i)
        {
            const auto event_fd{events[i].data.fd};

            if (event_fd == _server_socket.get_fd())
            {
                _handle_accept();
            }
            else
            {
                _handle_client_data(event_fd);
            }
        }
    }
}

void Server::_handle_accept()
{
    auto client{_server_socket.accept()};

    if (!client)
    {
        return;
    }

    const auto status{_epoll_instance.add(*client, EPOLLIN)};

    if (status.failed())
    {
        std::cerr << "[Server] Failed to add client to epoll: " << status.message() << "\n";
        return;
    }

    const auto client_fd{client->get_fd()};
    _clients.emplace(client_fd, std::move(*client));

    std::cout << "[Server] Client " << client_fd << " connected." << std::endl;
}

void Server::_handle_client_data(int fd)
{
    if (_clients.find(fd) == _clients.end())
    {
        return;
    }

    auto &client = _clients.at(fd);

    std::array<char, 4096> buffer{};

    const auto result{client.read(buffer)};

    if (result.retry())
    {
        return;
    }

    if (!result.ok() || result.bytes == 0)
    {
        std::cerr << "[Server] Client " << fd << " disconnected.\n";
        _clients.erase(fd);
    }
    else
    {
        std::string_view msg(buffer.data(), result.bytes);

        std::cout << "[Server] Received (" << fd << "): " << msg << '\n';
        client.write("Echo: ");
        client.write(msg);
    }
}
#include <iostream>

#include "Server.hpp"

int main()
{
    try
    {
        Server server{8000};
        server.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Fatal Error] " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
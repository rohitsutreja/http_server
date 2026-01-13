cmake_minimum_required(VERSION 3.10)
project(http_server_framework)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(LIB_SOURCES
    src/App.cpp
    src/Epoll.cpp
    src/Router.cpp
    src/Server.cpp
    src/Socket.cpp
)

add_library(http_server_lib STATIC ${LIB_SOURCES})

target_include_directories(http_server_lib PRIVATE src)

target_include_directories(http_server_lib PUBLIC include)

target_compile_options(http_server_lib PRIVATE -Wall -Wextra -Wpedantic)

add_executable(demo main.cpp)
target_link_libraries(demo PRIVATE http_server_lib)
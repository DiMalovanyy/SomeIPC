cmake_minimum_required(VERSION 3.5)

project(sockets LANGUAGES C)

add_executable(sockets_client sockets_client.c sockets_utils.h)
add_executable(sockets_server sockets_server.c sockets_utils.h)

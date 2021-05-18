cmake_minimum_required(VERSION 3.5)

project(Task4 LANGUAGES C)

#client
add_executable(CLIENT client.c fifo_utils.h)
add_executable(SERVER server.c fifo_utils.h)

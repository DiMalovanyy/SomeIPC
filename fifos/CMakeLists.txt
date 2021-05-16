cmake_minimum_required(VERSION 3.5)



Project(fifos LANGUAGES C)

set(SIMPLE simple_fifos)

add_executable(${SIMPLE}_client simple_fifos_client.c fifos_utils.h)
add_executable(${SIMPLE}_server simple_fifos_server.c fifos_utils.h)

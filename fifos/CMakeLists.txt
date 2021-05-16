cmake_minimum_required(VERSION 3.5)



Project(fifos LANGUAGES C)

set(SIMPLE simple_fifos)
set(UNIQUE unique_fifos)

add_executable(${SIMPLE}_client ${SIMPLE}_client.c fifos_utils.h)
add_executable(${SIMPLE}_server ${SIMPLE}_server.c fifos_utils.h)

add_executable(${UNIQUE}_client ${UNIQUE}_client.c fifos_utils.h)
add_executable(${UNIQUE}_server ${UNIQUE}_server.c fifos_utils.h)

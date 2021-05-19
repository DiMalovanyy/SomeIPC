cmake_minimum_required(VERSION 3.5)

project(IPC VERSION 0.0.1 LANGUAGES C)

add_subdirectory(fifos)
add_subdirectory(task4)
add_subdirectory(sockets)

add_executable(PIPE_SYNC pipe_sync.c)

add_executable(PIPE_LS_WC pipe_ls_wc.c)

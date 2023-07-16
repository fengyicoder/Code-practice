cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(test LANGUAGES CXX)

aux_source_directory(.. native_source)
aux_source_directory(../../memory memory_source)

set(CMAKE_CXX_STANDARD 17)

set(SRCS
    echo_server.cpp
    ../../log/pr.cpp
    ../../log/log.cpp
    ../../threadpool/thread_pool.cpp
)
list(APPEND SRCS ${native_source})
list(APPEND SRCS ${memory_source})

set(INCS
    ../
    ../../log
    ../../threadpool
)
include_directories(${INCS})
add_executable(echo_server ${SRCS})
target_link_libraries(echo_server pthread)



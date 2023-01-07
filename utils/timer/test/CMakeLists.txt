cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(test LANGUAGES CXX)

set(SRCS
    test_timer.cpp
    ../../log/pr.cpp
    ../../log/log.cpp
)
set(INCS
    ../
    ../../log
    ../../threadpool
)

set(CMAKE_CXX_STANDARD 17)

include_directories(${INCS})
add_executable(timer_test ${SRCS})
target_link_libraries(timer_test pthread)
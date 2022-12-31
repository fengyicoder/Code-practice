cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(test LANGUAGES CXX)

set(SRCS 
    test_threadpool.cpp
    ../thread_pool.cpp
    ../../log/pr.cpp 
    ../../log/log.cpp
)

set(INCS
    ../
    ../../log
)

set(CMAKE_CXX_STANDARD 17)

include_directories(${INCS})
add_executable(threadpool_test ${SRCS})
target_link_libraries(threadpool_test pthread)
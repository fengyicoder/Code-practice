cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(test LANGUAGES CXX)

include_directories(${PROJECT_SOURCE_DIR}/..)

set(SRCS test_pr.cpp ../pr.cpp)
add_executable(pr_test ${SRCS})
target_link_libraries(pr_test pthread)

set(SRCS test_q.cpp ../pr.cpp)
add_executable(q_test ${SRCS})
target_link_libraries(q_test pthread)

set(SRCS test_log_level.cpp ../pr.cpp ../log.cpp)
add_executable(log_level_test ${SRCS})
target_link_libraries(log_level_test pthread)

set(SRCS test_log_async.cpp ../pr.cpp ../log.cpp)
add_executable(log_level_async ${SRCS})
target_link_libraries(log_level_async pthread)

set(SRCS test_log_sync.cpp ../pr.cpp ../log.cpp)
add_executable(log_level_sync ${SRCS})
target_link_libraries(log_level_sync pthread)


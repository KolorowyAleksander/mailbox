cmake_minimum_required(VERSION 3.5)
project(queues)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory(client)
add_subdirectory(broker)
add_subdirectory(client-example)

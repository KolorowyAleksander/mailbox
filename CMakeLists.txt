cmake_minimum_required(VERSION 3.3)
project(queues)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_subdirectory(broker)
add_subdirectory(client)

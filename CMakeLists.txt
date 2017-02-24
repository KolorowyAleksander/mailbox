cmake_minimum_required(VERSION 3.3)
project(queues)

set (CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS}-pthread")

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

include_directories(${PROJECT_INCLUDE_DIR})

add_subdirectory(postman)
add_subdirectory(mailbox)
add_subdirectory(client-example)

cmake_minimum_required(VERSION 3.5)
project(client)

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set (CLIENT_SOURCES ${PROJECT_SOURCE_DIR}/main.cpp)

include_directories("${PROJECT_BINARY_DIR}")
add_executable(CLIENT ${CLIENT_SOURCES})

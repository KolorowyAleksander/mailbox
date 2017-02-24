cmake_minimum_required(VERSION 3.3)
project(client-example)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CLIENT_EXAMPLE_SOURCES ${PROJECT_SOURCE_DIR}/main.cpp)

include_directories("${PROJECT_BINARY_DIR}")
include_directories(${POSTMAN_INCLUDE_DIRS})

add_executable(CLIENT-EXAMPLE ${CLIENT_EXAMPLE_SOURCES})
target_link_libraries(CLIENT-EXAMPLE postman)

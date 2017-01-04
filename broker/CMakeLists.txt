cmake_minimum_required(VERSION 3.5)
project(broker)

set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set (BROKER_SOURCES ${PROJECT_SOURCE_DIR}/main.cpp)

include_directories(${CLIENT_INCLUDE_DIRS})
include_directories("${PROJECT_BINARY_DIR}")


add_executable(BROKER ${BROKER_SOURCES})
target_link_libraries(BROKER client)

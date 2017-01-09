cmake_minimum_required(VERSION 3.5)
project(postman)

set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set (POSTMAN_SOURCES ${PROJECT_SOURCE_DIR}/PostmanConnectionException.cpp
                     ${PROJECT_SOURCE_DIR}/PostmanException.cpp
                     ${PROJECT_SOURCE_DIR}/Connection.cpp)

include_directories(${PROJECT_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})
include_directories("${PROJECT_BINARY_DIR}")

add_library(${PROJECT_NAME} SHARED ${POSTMAN_SOURCES})

set(POSTMAN_INCLUDE_DIRS ${PROJECT_INCLUDE_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

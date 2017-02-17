cmake_minimum_required(VERSION 3.5)
project(mailbox)

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(MAILBOX_SOURCES ${PROJECT_SOURCE_DIR}/main.cpp
                    ${PROJECT_SOURCE_DIR}/SimpleLogger.cpp
                    ${PROJECT_SOURCE_DIR}/ConnectionReciever.cpp
                    ${PROJECT_SOURCE_DIR}/QueueManager.cpp
                    ${PROJECT_SOURCE_DIR}/Queue.cpp)

include_directories(${PROJECT_SOURCE_DIR})
include_directories(${POSTMAN_INCLUDE_DIRS})
include_directories("${PROJECT_BINARY_DIR}")


add_executable(MAILBOX ${MAILBOX_SOURCES})
target_link_libraries(MAILBOX postman)

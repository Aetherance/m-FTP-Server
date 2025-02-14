cmake_minimum_required(VERSION 3.28)

project(m-FTP-server)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

file(GLOB SERVER ${CMAKE_SOURCE_DIR}/server/*.cc)

file(GLOB CLIENT ${CMAKE_SOURCE_DIR}/client/*.cc)

add_executable(server ${SERVER})

add_executable(client ${CLIENT})
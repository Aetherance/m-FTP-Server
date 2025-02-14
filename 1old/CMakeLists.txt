# 指定 CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 设置工程名称
project(MyProject)

# 添加可执行文件
add_executable(client client.cpp)
add_executable(server server.cpp)
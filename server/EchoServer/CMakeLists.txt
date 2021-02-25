# cmake 最低版本要求
cmake_minimum_required(VERSION 3.6)

# 工程名称
project(EchoServer)

# 指定搜索路径
include_directories(../../bases/)

# 设置
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-g")

aux_source_directory(../../bases/ SOURCES)

add_executable(${PROJECT_NAME} EchoServer.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} -lpthread)

cmake_minimum_required (VERSION 3.12)
project (cppjson_test)

set(CMAKE_CXX_STANDARD 11)

add_executable(cppjson_test_ch02
    cppjson.cpp
    cppjsonTest.cpp
)

# 添加头文件目录
target_include_directories(cppjson_test_ch02 PRIVATE include)
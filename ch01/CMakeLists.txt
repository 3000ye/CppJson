cmake_minimum_required (VERSION 3.12)
project (cppjson_test)

set(CMAKE_CXX_STANDARD 11)

add_executable(cppjson_test_ch01
    cppjson.cpp
    test.cpp
)

# 添加头文件目录
target_include_directories(cppjson_test_ch01 PRIVATE include)
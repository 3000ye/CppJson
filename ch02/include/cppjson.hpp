#pragma once
#include <string>

// json 中有 6 种数据类型，将 true 和 false 分开就是 7 种
// 声明一个枚举类型类
enum class cppjsonType {
    CPPJSON_NULL,
    CPPJSON_TRUE,
    CPPJSON_FALSE,
    CPPJSON_NUMBER,
    CPPJSON_STRING,
    CPPJSON_ARRAY,
    CPPJSON_OBJECT
};

// 解析 json 的返回状态码
enum class cppjsonParseCode {
    OK,
    EXPECT_VALUE,
    INVALID_VALUE,
    ROOT_NOT_SINGULAR
};

// json 是树形结构，每个节点用 cppjson_value 结构体表示
typedef struct {
    double number;
    cppjsonType type;
} cppjson_value;

// 解析 json
cppjsonParseCode cppjson_parse(cppjson_value* v, const std::string json);
// 获取数据类型
cppjsonType cppjson_get_type(const cppjson_value* v);
// 获取数字型数据
double cppjson_get_number(const cppjson_value* v);
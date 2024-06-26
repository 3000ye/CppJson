#include "include/cppjson.hpp"
#include <assert.h>
#include <stdexcept>
#include <string>
#include <iostream>


typedef struct {
    std::string json;
} cppjson_context;

// 删除空白符
static void cppjson_parse_whitespace(cppjson_context* s) {
    const std::string str = s->json;

    int i = 0;
    while (str[i] == ' ' or str[i] == '\t' or str[i] == '\n' or str[i] == '\r') { i ++; }

    s->json = str.substr(i, str.size());
}

// 解析 ws2 之后是否还有非空值
static cppjsonParseCode cppjson_parse_root_not_singular(cppjson_context* s) {
    // 删除空白符
    cppjson_parse_whitespace(s);

    // 判断是否还有非空值
    if (s->json[0] != '\0') return cppjsonParseCode::ROOT_NOT_SINGULAR;
    else return cppjsonParseCode::OK;
}

// 解析 null 值
static cppjsonParseCode cppjson_parse_null(cppjson_context* s, cppjson_value* v) {
    const std::string str = s->json;

    auto head = str.substr(0, 4);
    if (head != "null") return cppjsonParseCode::INVALID_VALUE;
    else {
        s->json = str.substr(4, str.size());
        v->type = cppjsonType::CPPJSON_NULL;
        return cppjsonParseCode::OK;
    }
}

// 解析 true 值
static cppjsonParseCode cppjson_parse_true(cppjson_context* s, cppjson_value* v) {
    const std::string str = s->json;

    auto head = str.substr(0, 4);
    if (head != "true") return cppjsonParseCode::INVALID_VALUE;
    else {
        s->json = str.substr(4, str.size());
        v->type = cppjsonType::CPPJSON_TRUE;
        return cppjsonParseCode::OK;
    }
}

// 解析 false 值
static cppjsonParseCode cppjson_parse_false(cppjson_context* s, cppjson_value* v) {
    const std::string str = s->json;

    auto head = str.substr(0, 5);
    if (head != "false") return cppjsonParseCode::INVALID_VALUE;
    else {
        s->json = str.substr(5, str.size());
        v->type = cppjsonType::CPPJSON_FALSE;
        return cppjsonParseCode::OK;
    }
}

// 解析 number 值
static cppjsonParseCode cppjson_parse_number(cppjson_context* s, cppjson_value* v) {
    const std::string str = s->json;
    double number;

    std::cout << "number = " << str << std::endl;

    // 检查是否存在 正号
    if (str[0] == '+') return cppjsonParseCode::INVALID_VALUE;

    try { // 将 string 转为 double 类型
        number = std::stod(str);
    }
    catch (const std::invalid_argument& e) { // 如果无法正常转换，则返回 INVALID_VALUE
        fprintf(stderr, "%s:%d, Invalid argument: %s\n", __FILE__, __LINE__, str.c_str());
        return cppjsonParseCode::INVALID_VALUE;
    }
    catch (const std::out_of_range& e) { // 如果指数过大，返回 INFTY_NUMBER
        fprintf(stderr, "%s:%d, Infty number: %s\n", __FILE__, __LINE__, str.c_str());
        return cppjsonParseCode::INFTY_NUMBER;
    }
    
    v->type = cppjsonType::CPPJSON_NUMBER;
    v->number = number;

    int i = (str[0] == '-' ? 0 : -1);
    while (str[++i]) {
        std::string sub = str.substr(0, i + 1);
        double temp = std::stod(sub);
        if (temp == number) {
            s->json = str.substr(i + 1, str.size());
            break;
        }
    }

    return cppjsonParseCode::OK;
}

// 解析 value
static cppjsonParseCode cppjson_parse_value(cppjson_context* s, cppjson_value* v) {
    const std::string str = s->json;

    switch (str[0]) {
        case 'n': return cppjson_parse_null(s, v);
        case 't': return cppjson_parse_true(s, v);
        case 'f': return cppjson_parse_false(s, v);
        default: return cppjson_parse_number(s, v);
        case '\0': return cppjsonParseCode::EXPECT_VALUE;
    }
}

// json 解析函数：ws1 value ws2
cppjsonParseCode cppjson_parse(cppjson_value *v, const std::string json) {
    cppjson_context s; s.json = json;
    assert(v != NULL); v->type = cppjsonType::CPPJSON_NULL;

    // 分别解析 ws1 value ws2
    cppjson_parse_whitespace(&s);
    auto ret = cppjson_parse_value(&s, v);
    return ret == cppjsonParseCode::OK ? cppjson_parse_root_not_singular(&s) : ret;
}

// 返回 json 值的类型
cppjsonType cppjson_get_type(const cppjson_value *v) {
    assert(v != NULL);
    return v->type;
}
// 返回 number 型数据的值
double cppjson_get_number(const cppjson_value *v) {
    assert(v != NULL and v->type == cppjsonType::CPPJSON_NUMBER);
    return v->number;
}
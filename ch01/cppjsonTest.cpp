#include "include/cppjson.hpp"
#include <cstdio>
#include <stdio.h>
#include <typeinfo>

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

// 测试宏接口
// flag 表示测试点是否通过，如果未通过则打印异常信息
#define EXPECT_BASE(flag, expect, actual) \
    do {\
        test_count ++;\
        if (flag) test_pass ++;\
        else {\
            fprintf(stderr, "%s:%d, expect = %s(%d), actual = %s(%d)\n", __FILE__, __LINE__, typeid(expect).name(), static_cast<int>(expect), typeid(actual).name(), static_cast<int>(actual));\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_TYPE(expect, actual) EXPECT_BASE((expect) == (actual), expect, actual)


//
static void test_parse_null() {
    cppjson_value v;
    v.type = cppjsonType::CPPJSON_FALSE;

    EXPECT_TYPE(cppjsonParseCode::OK, cppjson_parse(&v, "null"));
    EXPECT_TYPE(cppjsonType::CPPJSON_NULL, cppjson_get_type(&v));
}

// 
static void test_parse_expect_value() {
    cppjson_value v;
    v.type = cppjsonType::CPPJSON_FALSE;

    EXPECT_TYPE(cppjsonParseCode::EXPECT_VALUE, cppjson_parse(&v, ""));
    EXPECT_TYPE(cppjsonType::CPPJSON_NULL, cppjson_get_type(&v));

    v.type = cppjsonType::CPPJSON_FALSE;
    EXPECT_TYPE(cppjsonParseCode::EXPECT_VALUE, cppjson_parse(&v, " "));
    EXPECT_TYPE(cppjsonType::CPPJSON_NULL, cppjson_get_type(&v));
}

//
static void test_parse_invalid_value() {
    cppjson_value v;
    v.type = cppjsonType::CPPJSON_FALSE;

    EXPECT_TYPE(cppjsonParseCode::INVALID_VALUE, cppjson_parse(&v, "nul"));
    EXPECT_TYPE(cppjsonType::CPPJSON_NULL, cppjson_get_type(&v));

    v.type = cppjsonType::CPPJSON_FALSE;
    EXPECT_TYPE(cppjsonParseCode::INVALID_VALUE, cppjson_parse(&v, "?"));
    EXPECT_TYPE(cppjsonType::CPPJSON_NULL, cppjson_get_type(&v));
}

//
static void test_parse_root_not_singular() {
    cppjson_value v;
    v.type = cppjsonType::CPPJSON_FALSE;

    EXPECT_TYPE(cppjsonParseCode::ROOT_NOT_SINGULAR, cppjson_parse(&v, "null x"));
    EXPECT_TYPE(cppjsonType::CPPJSON_NULL, cppjson_get_type(&v));
}

static void test_parse() {
    test_parse_null();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}

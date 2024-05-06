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

// TEST_ERROR 测试宏：异常用例
#define TEST_ERROR(error, json)\
    do {\
        cppjson_value v;\
        v.type = cppjsonType::CPPJSON_FALSE;\
        EXPECT_TYPE(error, cppjson_parse(&v, json));\
        EXPECT_TYPE(cppjsonType::CPPJSON_NULL, cppjson_get_type(&v));\
    } while(0)

// CPPJSON_NUMBER 测试宏：正常用例
#define TEST_NUMBER(expect, json)\
    do {\
        cppjson_value v;\
        EXPECT_TYPE(cppjsonParseCode::OK, cppjson_parse(&v, json));\
        EXPECT_TYPE(cppjsonType::CPPJSON_NUMBER, cppjson_get_type(&v));\
        EXPECT_TYPE(expect, cppjson_get_number(&v));\
    } while (0)


// 解析 null 测试
static void test_parse_null() {
    cppjson_value v;
    v.type = cppjsonType::CPPJSON_FALSE;

    EXPECT_TYPE(cppjsonParseCode::OK, cppjson_parse(&v, "null"));
    EXPECT_TYPE(cppjsonType::CPPJSON_NULL, cppjson_get_type(&v));
}

// 解析 true 测试
static void test_parse_true() {
    cppjson_value v;
    v.type = cppjsonType::CPPJSON_FALSE;

    EXPECT_TYPE(cppjsonParseCode::OK, cppjson_parse(&v, "true"));
    EXPECT_TYPE(cppjsonType::CPPJSON_TRUE, cppjson_get_type(&v));
}

// 解析 false 测试
static void test_parse_false() {
    cppjson_value v;
    v.type = cppjsonType::CPPJSON_TRUE;

    EXPECT_TYPE(cppjsonParseCode::OK, cppjson_parse(&v, "false"));
    EXPECT_TYPE(cppjsonType::CPPJSON_FALSE, cppjson_get_type(&v));
}

// 解析 number 测试
static void test_parse_number() {
    TEST_NUMBER(0.2, "0.2");
    TEST_NUMBER(0.0, "-0");
    TEST_NUMBER(0.20, "0.20");
    TEST_NUMBER(0.0, "-0.0");
    TEST_NUMBER(1.0, "1");
    TEST_NUMBER(-1.0, "-1");
    TEST_NUMBER(1.5, "1.5");
    TEST_NUMBER(-1.5, "-1.5");
    TEST_NUMBER(3.1416, "3.1416");
    TEST_NUMBER(1E10, "1E10");
    TEST_NUMBER(1e10, "1e10");
    TEST_NUMBER(1E+10, "1E+10");
    TEST_NUMBER(1E-10, "1E-10");
    TEST_NUMBER(-1E10, "-1E10");
    TEST_NUMBER(-1e10, "-1e10");
    TEST_NUMBER(-1E+10, "-1E+10");
    TEST_NUMBER(-1E-10, "-1E-10");
    TEST_NUMBER(1.234E+10, "1.234E+10");
    TEST_NUMBER(1.234E-10, "1.234E-10");
    TEST_NUMBER(0.0, "1e-10000"); /* must underflow */
}

// 解析 只有空白值情况
static void test_parse_expect_value() {
    TEST_ERROR(cppjsonParseCode::EXPECT_VALUE, "");
    TEST_ERROR(cppjsonParseCode::EXPECT_VALUE, " ");
}

// 解析 空白之后还有多余字符 情况
static void test_parse_root_not_singular() {
    TEST_ERROR(cppjsonParseCode::ROOT_NOT_SINGULAR, "null x");

    #if 1
        // 0 之后只能是：'.', 'E', 'e' 或 空
        TEST_ERROR(cppjsonParseCode::ROOT_NOT_SINGULAR, "0123");
        TEST_ERROR(cppjsonParseCode::ROOT_NOT_SINGULAR, "0x0");
        TEST_ERROR(cppjsonParseCode::ROOT_NOT_SINGULAR, "0x123");
    #endif
}

// 解析 异常 情况
static void test_parse_invalid_value() {
    TEST_ERROR(cppjsonParseCode::INVALID_VALUE, "nul");
    TEST_ERROR(cppjsonParseCode::INVALID_VALUE, "?");

    #if 1
        // 正号是非法的
        TEST_ERROR(cppjsonParseCode::INVALID_VALUE, "+0");
        TEST_ERROR(cppjsonParseCode::INVALID_VALUE, "+1");
        TEST_ERROR(cppjsonParseCode::INVALID_VALUE, "+0.2");
        // 小数点之前至少要有一个数字
        TEST_ERROR(cppjsonParseCode::INVALID_VALUE, ".123");
        // 小数点之后至少要有一个数字
        TEST_ERROR(cppjsonParseCode::INVALID_VALUE, "1.");
        // 特定关键词非法
        TEST_ERROR(cppjsonParseCode::INVALID_VALUE, "inf");
        TEST_ERROR(cppjsonParseCode::INVALID_VALUE, "INF");
        TEST_ERROR(cppjsonParseCode::INVALID_VALUE, "nan");
        TEST_ERROR(cppjsonParseCode::INVALID_VALUE, "NAN");
    #endif
}

static void test_parse() {
    // test_parse_null();
    // test_parse_true();
    // test_parse_false();
    test_parse_number();
    // test_parse_expect_value();
    // test_parse_invalid_value();
    // test_parse_root_not_singular();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}

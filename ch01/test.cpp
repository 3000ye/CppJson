#include "include/cppjson.hpp"
#include <iostream>
#include <typeinfo>

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;


#define EXPECT_BASE(flag, expect, actual) \
    do {\
        test_count ++;\
        if (flag) test_pass ++;\
        else {\
            std::cerr << __FILE__ << ": " << __LINE__ << " expect: " << typeid(expect).name() << " actual: " << typeid(actual).name() << std::endl;\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_TYPE(expect, actual) EXPECT_BASE((expect) == (actual), expect, actual)


// 解析 null 测试
static void test_parse_null() {
    cppjson_value v;
    v.type = cppjsonType::CPPJSON_FALSE;

    EXPECT_TYPE(cppjsonType::CPPJSON_FALSE, cppjson_get_type(&v));
    EXPECT_TYPE(cppjsonParseCode::OK, cppjson_parse(&v, "null"));
}

static void test_parse() {
    test_parse_null();
}

int main() {
    test_parse();
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    return main_ret;
}

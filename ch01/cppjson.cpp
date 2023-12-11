#include "include/cppjson.hpp"
#include <assert.h>


typedef struct {
    std::string json;
} cppjson_context;

// 检测空白符
static void cppjson_parse_whitespace(cppjson_context* s) {
    const std::string str = s->json;

    int i = 0;
    while (str[i] == ' ' or str[i] == '\t' or str[i] == '\n' or str[i] == '\r') { i ++; }

    s->json = str.substr(i, str.size());
}

// 检测 null 值
// static cppjsonParseCode cppjson_parse_null(cppjson_context* s, cppjson_value* v) {
//     const std::string str = s->json;

//     int i = 0;
//     if ()
// }

static cppjsonParseCode cppjson_parse_value(cppjson_context* s, cppjson_value* v) {
    return cppjsonParseCode::OK;
}

cppjsonParseCode cppjson_parse(cppjson_value *v, const std::string json) {
    // cppjson_context s;
    // assert(v != NULL);
    // s.json = json;
    // v->type = cppjsonType::CPPJSON_NULL;
    // cppjson_parse_whitespace(&s);

    // return cppjson_parse_value(&s, v);
    return cppjsonParseCode::OK;
}

cppjsonType cppjson_get_type(const cppjson_value *v) {
    return v->type;
}

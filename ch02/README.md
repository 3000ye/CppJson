# `CppJson` 第二章节：解析数字

* Castor
* 2023-12-29

本章节内容：

1. [初探重构](#1-初探重构)
2. [JSON 数字语法](#2-json-数字语法)
3. [数字表示方式](#3-数字表示方式)
4. [单元测试](#4-单元测试)
5. [十进制转换至二进制](#5-十进制转换至二进制)
6. [总结与练习](#6-总结与练习)
7. [参考](#7-参考)
8. [常见问题](#8-常见问题)

## 1. 初探重构

在讨论解析数字执勤啊，我们再补充 TDD 中的一个步骤——重构（refactoring）：

> 在不改变代码外在行为的情况下，对代码作出修改，以改进程序的内部结构。

在 TDD 的过程中，我们的目标是编写代码去通过测试。但由于这个目标的引导性太强，我们可能会忽略正确性以外的软件品质。在通过测试之后，代码的正确性得以保证，我们就应该审视现时的代码，看看有没有地方可以改进，而同时能维持测试顺利通过。我们可以安心地做各种修改，因为我们有单元测试，可以判断代码在修改后是否影响原来的行为。

那么，哪里要作出修改？Beck 和 Fowler 认为程序员要培养一种判断能力，找出程序中的坏味道。

例如，在第一单元的练习中，可能大部分人都会复制 `cppjson_parse_null()` 的代码，作一些修改，成为 `cppjson_parse_true()` 和 `cppjson_parse_false()`。如果我们再审视这 3 个函数，它们非常相似。这违反编程中常说的 DRY（don't repeat yourself）原则。本单元的第一个练习题，就是尝试合并这 3 个函数。

另外，我们也可能发现，单元测试代码也有很重复的代码，例如 `test_parse_invalid_value()` 中我们每次测试一个不合法的 JSON 值，都有 4 行相似的代码。我们可以把它用宏的方式把它们简化：

```cpp
// TEST_ERROR 测试宏：异常用例
#define TEST_ERROR(error, json)\
    do {\
        cppjson_value v;\
        v.type = cppjsonType::CPPJSON_FALSE;\
        EXPECT_TYPE(error, cppjson_parse(&v, json));\
        EXPECT_TYPE(cppjsonType::CPPJSON_NULL, cppjson_get_type(&v));\
    } while(0)

// 解析 只有空白值情况
static void test_parse_expect_value() {
    TEST_ERROR(cppjsonParseCode::EXPECT_VALUE, "");
    TEST_ERROR(cppjsonParseCode::EXPECT_VALUE, " ");
}
```

最后，我希望指出，软件的架构难以用单一标准评分，重构时要考虑平衡各种软件品质。例如上述把 3 个函数合并后，优点是减少重复的代码，维护较容易，但缺点可能是带来性能的少量影响。
#ifndef GTEST_H
#define GTEST_H

#include <my_string.h>

//class my_string_test: public ::testing

TEST(Ctor, DefaultCtor) {
    my_string str;
    EXPECT_EQ(str._size, size_t(0));
    EXPECT_EQ(str._is_heap, false);
    EXPECT_EQ(str._data, &str._stack[0]);
    EXPECT_EQ(*str._data, '\0');

    //EXPECT_EQ(str[0], '\0');
    //EXPECT_EQ(str.size(), size_t(0));
}
TEST(Ctor, CtorAllocStack) {                        // my_string(const char* newData)
    my_string str("123");
    EXPECT_EQ(str._size, size_t(3));
    EXPECT_EQ(str._is_heap, false);
    EXPECT_EQ(str._data, &str._stack[0]);
    EXPECT_EQ(str._data[str._size], '\0');
    EXPECT_STREQ(str._data, "123");
}
TEST(Ctor, CtorAllocHeap) {                         // my_string(const char* newData)
    my_string str("0123456789123456");
    EXPECT_EQ(str._size, size_t(16));
    EXPECT_EQ(str._capacity, size_t(16));
    EXPECT_EQ(str._is_heap, true);
    EXPECT_NE(str._data, &str._stack[0]);   //UB?
    EXPECT_EQ(str._data[str._size], '\0');
    EXPECT_STREQ(str._data, "0123456789123456");
}
TEST(Ctor, CtorStrAllocStack) {                     // my_string(const my_string& str)
    my_string str("123");
    my_string str2(str);

    EXPECT_EQ(str2._size, size_t(3));
    EXPECT_EQ(str2._is_heap, false);
    EXPECT_EQ(str2._data, &str2._stack[0]);
    EXPECT_EQ(str2._data[str2._size], '\0');
    EXPECT_STREQ(str2._data, str._data);
}
TEST(Ctor, CtorStrAllocHeap) {                      // my_string(const my_string& str)
    my_string str("0123456789123456");
    my_string str2(str);

    EXPECT_EQ(str2._size, size_t(16));
    EXPECT_EQ(str2._capacity, size_t(16));
    EXPECT_EQ(str2._is_heap, true);
    EXPECT_NE(str2._data, &str2._stack[0]);   //UB?
    EXPECT_NE(&str._data, &str2._data);
    EXPECT_EQ(str2._data[str2._size], '\0');
    EXPECT_STREQ(str2._data, str._data);
}

///
TEST(operator_equal, StackToStack) {                // my_string& operator=(const char* newData)
    my_string str("123456");
    str = "|||";

    EXPECT_EQ(str._size, size_t(3));
    EXPECT_EQ(str._is_heap, false);
    EXPECT_EQ(str._data, &str._stack[0]);
    EXPECT_EQ(str._data[str._size], '\0');
}
TEST(operator_equal, StackToHeap) {
    my_string str("123456");
    str = "|||||||||||||||||||||||||";

    EXPECT_EQ(str._size, size_t(25));
    EXPECT_EQ(str._capacity, size_t(25));
    EXPECT_EQ(str._is_heap, true);
    EXPECT_NE(str._data, &str._stack[0]);   //UB?
    EXPECT_EQ(str._data[str._size], '\0');
}
TEST(operator_equal, HeapToStack) {
    my_string str("123456789|123456789|12345");
    str = "|||||";

    EXPECT_EQ(str._size, size_t(5));
    EXPECT_EQ(str._is_heap, false);
    EXPECT_EQ(str._data, &str._stack[0]);
    EXPECT_EQ(str._data[str._size], '\0');
}
TEST(operator_equal, HeapToHeapEnoughMemory) {
    my_string str("123456789|123456789|12345");
    str = "123456789|123456789|";

    EXPECT_EQ(str._size, size_t(20));
    EXPECT_EQ(str._capacity, size_t(25));
    EXPECT_EQ(str._is_heap, true);
    EXPECT_NE(str._data, &str._stack[0]);   //UB?
    EXPECT_EQ(str._data[str._size], '\0');
}
TEST(operator_equal, HeapToHeapNotEnoughMemory) {
    my_string str("123456789|123456789|12345");
    str = "123456789|123456789|123456789|";

    EXPECT_EQ(str._size, size_t(30));
    EXPECT_EQ(str._capacity, size_t(30));
    EXPECT_EQ(str._is_heap, true);
    EXPECT_NE(str._data, &str._stack[0]);   //UB?
    EXPECT_EQ(str._data[str._size], '\0');
}

///
TEST(operator_equal_str, StackToStack) {            // my_string& operator=(const my_string& str)
    my_string first("123");
    my_string second("123456");
    second = first;

    EXPECT_EQ(second._size, size_t(3));
    EXPECT_EQ(second._is_heap, false);
    EXPECT_EQ(second._data, &second._stack[0]);
    EXPECT_EQ(second._data[second._size], '\0');
    EXPECT_STREQ(second._data, first._data);
}
TEST(operator_equal_str, StackToHeap) {
    my_string first("123456789|123456789|");
    my_string second("123");
    second = first;

    EXPECT_EQ(second._size, size_t(20));
    EXPECT_EQ(second._capacity, size_t(20));
    EXPECT_EQ(second._is_heap, true);
    EXPECT_NE(second._data, &second._stack[0]);   //UB?
    EXPECT_EQ(second._data[second._size], '\0');
    EXPECT_STREQ(second._data, first._data);
}
TEST(operator_equal_str, HeapToStack) {
    my_string first("123");
    my_string second("123456789|123456789|");
    second = first;

    EXPECT_EQ(second._size, size_t(3));
    EXPECT_EQ(second._is_heap, false);
    EXPECT_EQ(second._data, &second._stack[0]);
    EXPECT_EQ(second._data[second._size], '\0');
    EXPECT_STREQ(second._data, first._data);
}
TEST(operator_equal_str, HeapToHeapEnoughMemory) {
    my_string first("123456789|123456789|");
    my_string second("123456789|123456789|123456789|");
    second = first;

    EXPECT_EQ(second._size, size_t(20));
    EXPECT_EQ(second._capacity, size_t(30));
    EXPECT_EQ(second._is_heap, true);
    EXPECT_NE(second._data, &second._stack[0]);   //UB?
    EXPECT_EQ(second._data[second._size], '\0');
    EXPECT_NE(&second._data, &first._data);
    EXPECT_STREQ(second._data, first._data);
}
TEST(operator_equal_str, HeapToHeapNotEnoughMemory) {
    my_string first("123456789|123456789|123456789|");
    my_string second("123456789|123456789|");
    second = first;

    EXPECT_EQ(second._size, size_t(30));
    EXPECT_EQ(second._capacity, size_t(30));
    EXPECT_EQ(second._is_heap, true);
    EXPECT_NE(second._data, &second._stack[0]);   //UB?
    EXPECT_EQ(second._data[second._size], '\0');
    EXPECT_NE(&second._data, &first._data);
    EXPECT_STREQ(second._data, first._data);
}

///
TEST(operator_equal_rvalue, StackToStack) {             // my_string& operator=(my_string&& str)
    my_string first("123456");
    my_string second("123");
    second = std::move(first);

    EXPECT_EQ(first._size, size_t(0));
    EXPECT_EQ(first._is_heap, false);
    EXPECT_EQ(first._data, &first._stack[0]);
    EXPECT_EQ(first._data[first._size], '\0');

    EXPECT_EQ(second._size, size_t(6));
    EXPECT_EQ(second._is_heap, false);
    EXPECT_EQ(second._data, &second._stack[0]);
    EXPECT_EQ(second._data[second._size], '\0');
    EXPECT_NE(&second._data, &first._data);
}
TEST(operator_equal_rvalue, StackToHeap) {
    my_string first("123456789|123456789|");
    my_string second("123");
    second = std::move(first);

    EXPECT_EQ(first._size, size_t(0));
    EXPECT_EQ(first._is_heap, false);
    EXPECT_EQ(first._data, &first._stack[0]);
    EXPECT_EQ(first._data[first._size], '\0');

    EXPECT_EQ(second._size, size_t(20));
    EXPECT_EQ(second._capacity, size_t(20));
    EXPECT_EQ(second._is_heap, true);
    EXPECT_NE(second._data, &second._stack[0]);   //UB?
    EXPECT_EQ(second._data[second._size], '\0');
    EXPECT_NE(&second._data, &first._data);
}
TEST(operator_equal_rvalue, HeapToStack) {
    my_string first("123");
    my_string second("123456789|123456789|");
    second = std::move(first);

    EXPECT_EQ(first._size, size_t(0));
    EXPECT_EQ(first._is_heap, false);
    EXPECT_EQ(first._data, &first._stack[0]);
    EXPECT_EQ(first._data[first._size], '\0');

    EXPECT_EQ(second._size, size_t(3));
    EXPECT_EQ(second._is_heap, false);
    EXPECT_EQ(second._data, &second._stack[0]);
    EXPECT_EQ(second._data[second._size], '\0');
    EXPECT_NE(&second._data, &first._data);
}
TEST(operator_equal_rvalue, HeapToHeapEnoughMemory) {
    my_string first("123456789|123456789|");
    my_string second("123456789|123456789|123456789|");
    second = std::move(first);

    EXPECT_EQ(first._size, size_t(0));
    EXPECT_EQ(first._is_heap, false);
    EXPECT_EQ(first._data, &first._stack[0]);
    EXPECT_EQ(first._data[first._size], '\0');

    EXPECT_EQ(second._size, size_t(20));
    EXPECT_EQ(second._capacity, size_t(20));
    EXPECT_EQ(second._is_heap, true);
    EXPECT_NE(second._data, &second._stack[0]);   //UB?
    EXPECT_EQ(second._data[second._size], '\0');
    EXPECT_NE(&second._data, &first._data);
}
TEST(operator_equal_rvalue, HeapToHeapNotEnoughMemory) {
    my_string first("123456789|123456789|123456789|");
    my_string second("123456789|123456789|");
    second = std::move(first);

    EXPECT_EQ(first._size, size_t(0));
    EXPECT_EQ(first._is_heap, false);
    EXPECT_EQ(first._data, &first._stack[0]);
    EXPECT_EQ(first._data[first._size], '\0');

    EXPECT_EQ(second._size, size_t(30));
    EXPECT_EQ(second._capacity, size_t(30));
    EXPECT_EQ(second._is_heap, true);
    EXPECT_NE(second._data, &second._stack[0]);   //UB?
    EXPECT_EQ(second._data[second._size], '\0');
    EXPECT_NE(&second._data, &first._data);
}

///
TEST(comparison_operators, operator_less) {
    my_string first("123");
    my_string second("12345678901234");
    bool comparison_stack_to_stack_less = first < second;
    EXPECT_TRUE(comparison_stack_to_stack_less);
    ///
    first = "12345";
    second = "12345";
    bool comparison_stack_to_stack_equal = first < second;
    EXPECT_FALSE(comparison_stack_to_stack_equal);
    ///
    first = "12345";
    second = "123";
    bool comparison_stack_to_stack_more = first < second;
    EXPECT_FALSE(comparison_stack_to_stack_more);


    first = "123";
    second = "123456789|123456789|";
    bool comparison_stack_to_heap_less = first < second;
    EXPECT_TRUE(comparison_stack_to_heap_less);
    ///
    bool comparison_heap_to_stack_more = second < first;
    EXPECT_FALSE(comparison_heap_to_stack_more);


    first = "123456789|123456789|";
    second = "123456789|123456789|123456789|";
    bool comparison_heap_to_heap_less = first < second;
    EXPECT_TRUE(comparison_heap_to_heap_less);
    ///
    bool comparison_heap_to_heap_more = second < first;
    EXPECT_FALSE(comparison_heap_to_heap_more);
    ///
    first = "123456789|123456789|";
    second = "123456789|123456789|";
    bool comparison_heap_to_heap_equal = second < first;
    EXPECT_FALSE(comparison_heap_to_heap_equal);
}
TEST(comparison_operators, operator_more) {
    my_string first("123");
    my_string second("12345678901234");
    bool comparison_stack_to_stack_less = first > second;
    EXPECT_FALSE(comparison_stack_to_stack_less);
    ///
    first = "12345";
    second = "12345";
    bool comparison_stack_to_stack_equal = first > second;
    EXPECT_FALSE(comparison_stack_to_stack_equal);
    ///
    first = "12345";
    second = "123";
    bool comparison_stack_to_stack_more = first > second;
    EXPECT_TRUE(comparison_stack_to_stack_more);


    first = "123";
    second = "123456789|123456789|";
    bool comparison_stack_to_heap_less = first > second;
    EXPECT_FALSE(comparison_stack_to_heap_less);
    ///
    bool comparison_heap_to_stack_more = second > first;
    EXPECT_TRUE(comparison_heap_to_stack_more);


    first = "123456789|123456789|";
    second = "123456789|123456789|123456789|";
    bool comparison_heap_to_heap_less = first > second;
    EXPECT_FALSE(comparison_heap_to_heap_less);
    ///
    bool comparison_heap_to_heap_more = second > first;
    EXPECT_TRUE(comparison_heap_to_heap_more);
    ///
    first = "123456789|123456789|";
    second = "123456789|123456789|";
    bool comparison_heap_to_heap_equal = first > second;
    EXPECT_FALSE(comparison_heap_to_heap_equal);
}
TEST(comparison_operators, operator_less_or_equal) {
    my_string first("12345");
    my_string second("123456");
    bool comparison_less = first <= second;
    EXPECT_TRUE(comparison_less);

    bool comparison_more = second <= first;
    EXPECT_FALSE(comparison_more);

    first = "12345";
    second = "12345";
    bool comparison_equal_1 = first <= second;
    EXPECT_TRUE(comparison_equal_1);

    bool comparison_equal_2 = second <= first;
    EXPECT_TRUE(comparison_equal_2);
}
TEST(comparison_operators, operator_more_or_equal) {
    my_string first("12345");
    my_string second("123456");
    bool comparison_less = first >= second;
    EXPECT_FALSE(comparison_less);

    bool comparison_more = second >= first;
    EXPECT_TRUE(comparison_more);

    first = "12345";
    second = "12345";
    bool comparison_equal_1 = first >= second;
    EXPECT_TRUE(comparison_equal_1);

    bool comparison_equal_2 = second >= first;
    EXPECT_TRUE(comparison_equal_2);
}
TEST(comparison_operators, operator_equal) {
    my_string first("123");
    my_string second("123456789");

    bool comparison_equal_1 = first == second;
    EXPECT_FALSE(comparison_equal_1);

    bool comparison_equal_2 = second == first;
    EXPECT_FALSE(comparison_equal_2);

    first = "12345";
    second = "12345";
    bool comparison_equal_3 = first == second;
    EXPECT_TRUE(comparison_equal_3);

    bool comparison_equal_4 = second == first;
    EXPECT_TRUE(comparison_equal_4);
}
TEST(comparison_operators, operator_not_equal) {
    my_string first("123");
    my_string second("123456789");

    bool comparison_equal_1 = first != second;
    EXPECT_TRUE(comparison_equal_1);

    bool comparison_equal_2 = second != first;
    EXPECT_TRUE(comparison_equal_2);

    first = "12345";
    second = "12345";
    bool comparison_equal_3 = first != second;
    EXPECT_FALSE(comparison_equal_3);

    bool comparison_equal_4 = second != first;
    EXPECT_FALSE(comparison_equal_4);
}

///
TEST(other_operators, operator_square_brackets) {
    my_string str("123");
    EXPECT_EQ(str[0], '1');
    EXPECT_EQ(str[1], '2');
    EXPECT_EQ(str[2], '3');
    EXPECT_EQ(str[3], '\0');  // UB?
}
TEST(other_operators, operator_addition) {
    my_string stack_1("12345|");
    my_string stack_2("67890|");
    my_string result_1;
    result_1 = stack_1 + stack_2;

    EXPECT_EQ(stack_1, "12345|");
    EXPECT_EQ(stack_2, "67890|");
    EXPECT_EQ(result_1, "12345|67890|");
    ///
    my_string heap_1("1234567890|1234567890|");
    my_string result_2 = heap_1 + stack_1;

    EXPECT_EQ(heap_1, "1234567890|1234567890|");
    EXPECT_EQ(stack_1, "12345|");
    EXPECT_EQ(result_2, "1234567890|1234567890|12345|");
}
/*
TEST(comparison_operators, operator_more) {
    my_string first("123");
    my_string second("123456789|123456789|");
    bool comparison = first > second;
    EXPECT_FALSE(comparison);

    first = "1";
    second = "123333333333333333333333333333333333";
    bool comparison2 = first > second;
    EXPECT_FALSE(comparison2);

    first = "12345";
    second = "123";
    bool comparison3 = first > second;
    EXPECT_TRUE(comparison3);
}*/


#endif // GTEST_H

#include <iostream>

#define VECTOR_OPEN_SOURCE
#include "src/my_vector.hpp"
#undef VECTOR_OPEN_SOURCE

#include "test_framework/test_framework.hpp"

using namespace cxx;
using namespace tests_framework;

// my_vector<T> tests//
void copy_constructor_test() {
    my_vector<int> source_vector(3);
    source_vector.resize(3);

    source_vector[0] = 1;
    source_vector[1] = 2;
    source_vector[2] = 3;

    my_vector<int> other_vector(source_vector);
    Assert(source_vector, other_vector, __LINE__);

    my_vector<char*> source_vector2;
    my_vector<char*> other_vector2(source_vector2);
}

void push_back_test() {
    my_vector<char> vector;

    vector.push_back(2);
    Assert(vector.array_[0], 2, __LINE__);
    Assert(vector.size_, 1, __LINE__);


    vector.push_back(3);
    Assert(vector.array_[0], 2, __LINE__);
    Assert(vector.array_[1], 3, __LINE__);
    Assert(vector.size_, 2, __LINE__);
}

void resize_test() {
    my_vector<float> vector(3);

    vector.resize(5);
    Assert(vector.size_, 5, __LINE__);
    Assert(vector.capacity_, 5, __LINE__);

    vector.resize(1);
    Assert(vector.size_, 1, __LINE__);
    Assert(vector.capacity_, 5, __LINE__);
}

void assigment_operator_test() {
    my_vector<int> source_vector;
    my_vector<int> vector(3);

    vector[0] = 1;
    vector[1] = 2;
    vector[2] = 3;

    source_vector = vector;

    Assert(source_vector.size_, vector.size_, __LINE__);
    Assert(source_vector.capacity_, vector. capacity_, __LINE__);

    Assert(source_vector[0], 1, __LINE__);
    Assert(source_vector[1], 2, __LINE__);
    Assert(source_vector[2], 3, __LINE__);
}

//my_vector<bool> tests//
void bool_copy_constructor_test() {
    my_vector<bool> source_vector(3);
    source_vector.resize(3);

    source_vector[0] = true;
    source_vector[1] = false;
    source_vector[2] = true;

    my_vector<bool> other_vector(source_vector);
    Assert(source_vector, other_vector, __LINE__);

}

void bool_push_back_test() {
    my_vector<bool> vector;
    vector.push_back(true);
    vector.push_back(false);
    vector.push_back(true);
    vector.push_back(true);

    vector.push_back(true);
    vector.push_back(false);
    vector.push_back(true);
    vector.push_back(false);

    Assert(8, vector.size_, __LINE__);
    Assert(vector.array_[0], 0xBAu, __LINE__);
}

void bool_resize_test() {
    my_vector<bool> vector(3);

    vector.resize(5);

    Assert(vector.size_, 5, __LINE__);
    Assert(vector.chrsize_, 1, __LINE__);

    vector.resize(1);

    Assert(vector.size_, 1, __LINE__);
    Assert(vector.chrsize_, 1, __LINE__);
}



int main() {
    vector_with_tests vectorWithTests(7);

    vectorWithTests[0] = {copy_constructor_test, "copy CTOR test"};
    vectorWithTests[1] = {push_back_test, "push back test"};
    vectorWithTests[2] = {resize_test, "resize test"};
    vectorWithTests[3] = {bool_copy_constructor_test, "bool vec copy CTOR test"};
    vectorWithTests[4] = {bool_push_back_test, "bool vec push back test"};
    vectorWithTests[5] = {assigment_operator_test, "assigment operator test"};
    vectorWithTests[6] = {bool_resize_test, "bool vec reisze test"};

    test_framework run_tests;
    run_tests(vectorWithTests);

    return 0;
}

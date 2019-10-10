#pragma once

#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <math.h>

enum REG {
#define DEF_REG(register, code, command) register = code,
#include "../lib/DEF_REG.h"
#undef DEF_REG
};

#define REG_AX registers_[0]
#define REG_BX registers_[1]
#define REG_CX registers_[2]
#define REG_DX registers_[3]
#define REG_OX registers_[4]
#define REG_EB registers_[5]
#define REG_ES registers_[6]


const char NOTEXE_FILE[] =  "../assembly/CPU.notexe";
const int BASE_EB = 511;
const int BASE_STACK_CAP = 512;
const int RAM_SIZE = 1024;
const int REG_SIZE = 7;
const int ERRCODE = -123;


class cpu {
    int ram_[RAM_SIZE];
    int registers_[REG_SIZE];
    std::vector<int> commands_buf_;
    int start_point_;
    std::string exename_;

//APPARAT STACK
    int stack_size_;
    int stack_capacity_;

public:
//APPARAT STACK FUNCS
    int Stack_OK();

    bool Stack_empty();

    int push(int number);

    int pop();

    int mul();

    int div();

    int add();

    int sub();

    int Stack_getsize();

    int Stack_getcapacity();

//CPU FUNCS
    explicit cpu(const char* exename);

    ~cpu() = default;

    void do_cpu();

    int load_code();

    int find_startpoint();

    int do_commands();

    int create_frame(int ret_adr);

    int delete_frame(int& ret_adr);

//debug print
    void dump();

};

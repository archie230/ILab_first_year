#pragma once

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <cassert>
#include <ctime>
#include "../lib/constants.h"

const char INPUT[] = "../assembly/CPU.notexe";
const char OUTPUT[] = "DISASM.txt";

class disasm {
    std::vector<int> commands_buf_;
    std::map<int, std::string> AsmFuncMap_;
    std::map<int, std::string> LblMap_;
    std::string filename_;
    std::string disasmfilename_;

public:

    disasm(const char *filename,const char *disasmfilename);

    ~disasm() = default;

    void do_disassembly();

    int load_code();

    int load_commands(FILE *code);

    int load_lbls_or_func(FILE *code, std::map<int, std::string> &map);

    int write_disasmcode();

//=================================================
//=====================DUMPERS=====================
//=================================================

//=========PUSH_DUMPERS=========
    int dump_PUSH_NUM(FILE* output, int& ip);

    int dump_PUSH_REG(FILE* output, int& ip);

    int dump_PUSH_RAM_NUM(FILE* output, int& ip);

    int dump_PUSH_RAM_REG(FILE* output, int& ip);

    int dump_PUSH_RAM_REG_A_NUM(FILE* output, int& ip);

    int dump_PUSH_RAM_REG_S_NUM(FILE* output, int& ip);
//==========POP_DUMPERS=========
    int dump_POP_DEF(FILE* output, int& ip);

    int dump_POP_REG(FILE* output, int& ip);

    int dump_POP_RAM_NUM(FILE* output, int& ip);

    int dump_POP_RAM_REG(FILE* output, int& ip);

    int dump_POP_RAM_REG_A_NUM(FILE* output, int& ip);

    int dump_POP_RAM_REG_S_NUM(FILE* output, int& ip);
//===========OTHER_DUMPERS=======
    int dump_OUT(FILE* output, int& ip);

    int dump_MUL(FILE* output, int& ip);

    int dump_DIV(FILE* output, int& ip);

    int dump_ADD(FILE* output, int& ip);

    int dump_SUB(FILE* output, int& ip);

    int dump_JMP(FILE* output, int& ip);

    int dump_JE(FILE* output, int& ip);

    int dump_JA(FILE* output, int& ip);

    int dump_JB(FILE* output, int& ip);

    int dump_END(FILE* output, int& ip);

    int dump_CALL(FILE* output, int& ip);

    int dump_RET(FILE* output, int& ip);

    int dump_MAIN(FILE* output, int& ip);

    int dump_INT(FILE* output, int& ip);

    int dump_SQRT(FILE* output, int& ip);

};

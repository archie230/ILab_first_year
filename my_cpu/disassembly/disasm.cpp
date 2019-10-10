#include "disasm.h"

disasm::disasm(const char *filename, const char *disasmfilename)
        :
        commands_buf_(),
        AsmFuncMap_(),
        LblMap_(),
        filename_(filename),
        disasmfilename_(disasmfilename) {}

void disasm::do_disassembly() {
    if(load_code() == ERRS::ERRCODE)
        return;


    if(write_disasmcode() == ERRS::ERRCODE)
        return;
}

int disasm::load_code() {
    FILE *code = fopen(filename_.c_str(), "rb");
    if (!code) {
        std::cout << "can't open file:" << filename_ << std::endl;
        return ERRS::ERRCODE;
    }

    load_commands(code);

    if (load_lbls_or_func(code, LblMap_) == ERRS::ERRCODE) {
        std::cout << "bad .notexe file" << std::endl;
        fclose(code);
        return ERRS::ERRCODE;
    }

    if (load_lbls_or_func(code, AsmFuncMap_) == ERRS::ERRCODE) {
        std::cout << "bad .notexe file" << std::endl;
        fclose(code);
        return ERRS::ERRCODE;
    }

    fclose(code);
    return 0;
}

int disasm::load_commands(FILE *code) {
    int codesize = 0;
    fread(&codesize, sizeof(int), 1, code);
    fgetc(code);

    commands_buf_.resize(codesize);
    for (int i = 0; i < codesize; i++) {
        fread(&commands_buf_[i], sizeof(int), 1, code);
        fgetc(code);
    }

    return 0;
}

int disasm::load_lbls_or_func(FILE *code, std::map<int, std::string> &map) {
    //std::cout << ftell(code) << std::endl;

    int map_size = 0;
    int str_size = 0;
    std::string str;
    int adr = 0;

    if (fgetc(code) != '|') {
        //std::cout << ftell(code) << std::endl;
        return ERRS::ERRCODE;
    }

    fread(&map_size, sizeof(int), 1, code);
    assert(map_size >= 0);

    for (int i = 0; i < map_size; i++) {
        if (char sym = fgetc(code) != '[') {
            //std::cout << "not [ " << sym << std::endl;
            return ERRS::ERRCODE;
        }

        fread(&str_size, sizeof(int), 1, code);
        assert(str_size > 0);
        str.resize(str_size);

        fread(&str[0], sizeof(char), str_size, code);
        fread(&adr, sizeof(int), 1, code);
        assert(adr >= 0);

        if (!map.emplace(adr, str).second) {
            std::cout << "can't emplace in map " << __PRETTY_FUNCTION__ << std::endl;
            return ERRS::ERRCODE;
        }

        if (char sym = fgetc(code) != ']') {
            //std::cout << "not ] " << sym << std::endl;
            return ERRS::ERRCODE;
        }
    }

    if (fgetc(code) != '|') {
        //std::cout << ftell(code) << std::endl;
        return ERRS::ERRCODE;
    }

    return 0;
}

int disasm::write_disasmcode() {
    FILE* output = fopen(disasmfilename_.c_str(), "w");
    if(!output)
    {
        std::cout << "can't open " << disasmfilename_ << std::endl;
        return ERRS::ERRCODE;
    }

    std::time_t t = std::time(nullptr);

    fprintf(output, ";CODE GENERATED FROM %s  USING DISASSEMBLY\n", filename_.c_str());
    fprintf(output, ";%s\n", std::ctime(&t));

    std::map<int, std::string>::iterator nemo;
    for(int ip = 0; ip < commands_buf_.size(); ip++)
    {
        nemo = AsmFuncMap_.find(ip);
        if(nemo != AsmFuncMap_.end())
        {
            fprintf(output, "%s %s\n", FOO, nemo -> second.c_str());
        }

        nemo = LblMap_.find(ip);
        if(nemo != LblMap_.end())
        {
            fprintf(output, ":%s\n", nemo -> second.c_str());
        }


        switch(commands_buf_[ip])
        {
#define DEF_CMD(command, command_code, str, code) case command_code: {\
                                                    if(dump_##command(output, ip) == ERRS::ERRCODE){\
                                                    fclose(output);\
                                                    return ERRS::ERRCODE;}\
                                                    continue;}
#include "../lib/DEF_CMD.h"
#undef DEF_CMD
            default:
            {
                std::cout << "bad .notexe file" << std::endl;
                return ERRS::ERRCODE;
            }
        }
    }


    fclose(output);
    return 0;
}

int disasm::dump_PUSH_NUM(FILE *output, int &ip) {
    fprintf(output, "push %d\n", commands_buf_[ip+1]);
    ip++;
    return 0;
}

int disasm::dump_PUSH_REG(FILE *output, int &ip) {
    switch(commands_buf_[ip+1]) {

#define DEF_REG(command, code, name)   case code: {fprintf(output, "push ");\
                                                   fprintf(output, name);\
                                                   fputc('\n', output);\
                                                   ip++;\
                                                   return 0;}

#include "../lib/DEF_REG.h"
#undef DEF_REG
        default: {
            std::cout << commands_buf_[ip + 1] << "bad .notexe output" << std::endl;
            return ERRS::ERRCODE;
        }
    }
}

int disasm::dump_PUSH_RAM_NUM(FILE *output, int &ip) {
    fprintf(output, "push [%d]\n", commands_buf_[ip+1]);
    ip++;
    return 0;
}

int disasm::dump_PUSH_RAM_REG(FILE *output, int &ip) {
    switch(commands_buf_[ip+1]) {
#define DEF_REG(command, code, name) case(code): { fprintf(output, "push [");\
                                                fprintf(output, name);\
                                                fprintf(output, "]\n");\
                                                ip++;\
                                                return 0;}

#include "../lib/DEF_REG.h"

#undef DEF_REG
        default: {
            std::cout << "bad .notexe file" << std::endl;
            return ERRS::ERRCODE;
        }
    }
}

int disasm::dump_PUSH_RAM_REG_A_NUM(FILE *output, int &ip) {
    short* rgstr = nullptr;
    short* num = nullptr;

    rgstr = (short*) &commands_buf_[ip+1];
    num = rgstr + 1;

    switch(*rgstr) {
#define DEF_REG(command, code, name) case(code): {fprintf(output, "push [");\
                                                fprintf(output, name);\
                                                fprintf(output, "+%d]\n", *num);\
                                                ip++;\
                                                return 0;}

#include "../lib/DEF_REG.h"

#undef DEF_REG
        default: {
            std::cout << "bad .notexe file" << std::endl;
            return ERRS::ERRCODE;
        }
    }
}

int disasm::dump_PUSH_RAM_REG_S_NUM(FILE *output, int &ip) {
    short* rgstr = nullptr;
    short* num = nullptr;

    rgstr = (short*) &commands_buf_[ip+1];
    num = rgstr + 1;

    switch(*rgstr) {
#define DEF_REG(command, code, name) case(code): {fprintf(output, "push [");\
                                                fprintf(output, name);\
                                                fprintf(output, "-%d]\n", *num);\
                                                ip++;\
                                                return 0;}

#include "../lib/DEF_REG.h"

#undef DEF_REG
        default: {
            std::cout << "bad .notexe file" << std::endl;
            return ERRS::ERRCODE;
        }
    }
}

int disasm::dump_POP_DEF(FILE *output, int &ip) {
    fprintf(output, "push %d\n", commands_buf_[ip+1]);
    ip++;
    return 0;
}

int disasm::dump_POP_REG(FILE *output, int &ip) {
    switch(commands_buf_[ip+1]) {

#define DEF_REG(command, code, name)   case code: {fprintf(output, "pop ");\
                                                   fprintf(output, name);\
                                                   fputc('\n', output);\
                                                   ip++;\
                                                   return 0;}

#include "../lib/DEF_REG.h"
#undef DEF_REG
        default: {
            std::cout << commands_buf_[ip + 1] << "bad .notexe output" << std::endl;
            return ERRS::ERRCODE;
        }
    }
}

int disasm::dump_POP_RAM_NUM(FILE *output, int &ip) {
    fprintf(output, "pop [%d]\n", commands_buf_[ip+1]);
    ip++;
    return 0;
}

int disasm::dump_POP_RAM_REG(FILE *output, int &ip) {
    switch(commands_buf_[ip+1]) {
#define DEF_REG(command, code, name) case(code): { fprintf(output, "pop [");\
                                                fprintf(output, name);\
                                                fprintf(output, "]\n");\
                                                ip++;\
                                                return 0;}

#include "../lib/DEF_REG.h"

#undef DEF_REG
        default: {
            std::cout << "bad .notexe file" << std::endl;
            return ERRS::ERRCODE;
        }
    }
}

int disasm::dump_POP_RAM_REG_A_NUM(FILE *output, int &ip) {
    short* rgstr = nullptr;
    short* num = nullptr;

    rgstr = (short*) &commands_buf_[ip+1];
    num = rgstr + 1;

    switch(*rgstr) {
#define DEF_REG(command, code, name) case(code): {fprintf(output, "pop [");\
                                                fprintf(output, name);\
                                                fprintf(output, "+%d]\n", *num);\
                                                return 0;}

#include "../lib/DEF_REG.h"

#undef DEF_REG
        default: {
            std::cout << "bad .notexe file" << std::endl;
            return ERRS::ERRCODE;
        }
    }
}

int disasm::dump_POP_RAM_REG_S_NUM(FILE *output, int &ip) {
    short* rgstr = nullptr;
    short* num = nullptr;

    rgstr = (short*) &commands_buf_[ip+1];
    num = rgstr + 1;

    switch(*rgstr) {
#define DEF_REG(command, code, name) case(code): {fprintf(output, "pop [");\
                                                fprintf(output, name);\
                                                fprintf(output, "-%d]\n", *num);\
                                                ip++;\
                                                return 0;}

#include "../lib/DEF_REG.h"

#undef DEF_REG
        default: {
            std::cout << "bad .notexe file" << std::endl;
            return ERRS::ERRCODE;
        }
    }
}

int disasm::dump_OUT(FILE *output, int &ip) {
    fprintf(output, "out\n");
    return 0;
}

int disasm::dump_MUL(FILE *output, int &ip) {
    fprintf(output, "mul\n");
    return 0;
}

int disasm::dump_DIV(FILE *output, int &ip) {
    fprintf(output, "div\n");
    return 0;
}

int disasm::dump_ADD(FILE *output, int &ip) {
    fprintf(output, "add\n");
    return 0;
}

int disasm::dump_SUB(FILE *output, int &ip) {
    fprintf(output, "sub\n");
    return 0;
}

int disasm::dump_JMP(FILE *output, int &ip) {
    auto LblIter = LblMap_.find(commands_buf_[ip+1]);
    if(LblIter == LblMap_.end())
    {
        std::cout << "bad .notexe file" << std::endl;
        return ERRS::ERRCODE;
    }

    fprintf(output, "jmp %s\n", LblIter -> second.c_str());
    ip++;
    return 0;
}

int disasm::dump_JE(FILE *output, int &ip) {
    auto LblIter = LblMap_.find(commands_buf_[ip+1]);
    if(LblIter == LblMap_.end())
    {
        std::cout << "bad .notexe file" << std::endl;
        return ERRS::ERRCODE;
    }

    fprintf(output, "je %s\n", LblIter -> second.c_str());
    ip++;
    return 0;
}

int disasm::dump_JA(FILE *output, int &ip) {
    auto LblIter = LblMap_.find(commands_buf_[ip+1]);
    if(LblIter == LblMap_.end())
    {
        std::cout << "bad .notexe file" << std::endl;
        return ERRS::ERRCODE;
    }

    fprintf(output, "ja %s\n", LblIter -> second.c_str());
    ip++;
    return 0;
}

int disasm::dump_JB(FILE *output, int &ip) {
    auto LblIter = LblMap_.find(commands_buf_[ip+1]);
    if(LblIter == LblMap_.end())
    {
        std::cout << "bad .notexe file" << std::endl;
        return ERRS::ERRCODE;
    }

    fprintf(output, "jb %s\n", LblIter -> second.c_str());
    ip++;
    return 0;
}

int disasm::dump_END(FILE *output, int &ip) {
    fprintf(output, "end\n");
    return 0;
}

int disasm::dump_CALL(FILE *output, int &ip) {
    auto AsmFunsIter = AsmFuncMap_.find(commands_buf_[ip+1]);
    if(AsmFunsIter == AsmFuncMap_.end())
    {
        std::cout << "bad .notexe file" << std::endl;
        return ERRS::ERRCODE;
    }

    fprintf(output, "call %s\n", AsmFunsIter -> second.c_str());
    ip++;
    return 0;
}

int disasm::dump_RET(FILE *output, int &ip) {
    fprintf(output, "ret\n");
    fputc('\n', output);
    return 0;
}

int disasm::dump_MAIN(FILE *output, int &ip) {
    fprintf(output, "main\n");
    return 0;
}

int disasm::dump_INT(FILE *output, int &ip) {
    fprintf(output, "int\n");
    return 0;
}

int disasm::dump_SQRT(FILE* output, int& ip) {
    fprintf(output, "sqrt\n");
    return 0;
}

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <assert.h>
#include "string_to_int.hpp"


//--------------------------//
enum FUNC
{
    PUSH_NUM           = 1,
    PUSH_REG           = 2,
    PUSH_RAM_NUM       = 3,
    PUSH_RAM_REG       = 4,
    PUSH_RAM_REG_A_NUM = 5,
    PUSH_RAM_REG_S_NUM = 6,
    
    POP_DEF            = 7,
    POP_REG            = 8,
    POP_RAM_NUM        = 9,
    POP_RAM_REG        = 10,
    POP_RAM_REG_A_NUM  = 11,
    POP_RAM_REG_S_NUM  = 12,
    
    OUT                = 13,
    MUL                = 14,
    DIV                = 15,
    ADD                = 16,
    SUB                = 17,
    JMP                = 18,
    JE                 = 19,
    JL                 = 20,
    JB                 = 21,
    END                = 22,
    CALL               = 23,
    RET                = 24,
    MAIN               = 25
};

enum REG
{
    AX           = 26,
    BX           = 27,
    CX           = 28,
    DX           = 29,
    EB           = 30,
    ES           = 31
};

//-------------------//
const int ERRCODE    = -1;
//-------------------//
const char SYNT[]    = "\tSyntactic error: ";
const char FOO[]    = "func";

const char NMBR      = 'n';
const char REGSTR    = 'r';
const char LBL       = 'l';
const char FUNCTN    = 'f';
const char RAM       = 'o';
const char RAM_A_NUM = 'q';
const char RAM_S_NUM = 'w';
const char ASM_FUNC  = 'e';

//----------------//

class assembly;

//------TOKENS----//
class token
{
public:
    std::string str_;
    int line_;

    token()
    :   str_(),
        line_(0)
    {}

    ~token() = default;

    void clear()
    {
        str_.erase();
        line_ = 0;
    }

    void err_dump()
    {
        std::cout << str_;
        std::cout << " line: " << line_ << std::endl;
    }

    friend assembly;
};


class transtoken{
public:
    char tokentype_;
    int code_;
    int line_;

    transtoken(char tokentype, int code):
        tokentype_(tokentype),
        code_(code)
    {}

    ~transtoken() = default;

    void print()
    {
        std::cout << "-------------" << std::endl;
        std::cout << "tokentype: " << tokentype_ << '\n';
        std::cout<< "code: " << code_ << '\n';
    }

    friend assembly;
};
//----------------//


//-----MAPS-------//
class maps
{
public:
    std::map<std::string, int> FuncMap;
    std::map<std::string, int> RegMap;
    std::map<std::string, int> LblMap;
    std::map<std::string, int> AsmFuncMap;

    maps()
    {
        FuncMapFilling();
        RegMapFilling();
    }

    ~maps() = default;



#define CHCKEMPLACE(str, const, Mapname)\
                                         if( Mapname##Map.emplace(str, const).second != true) {\
                                                    printf("Can't emplace in func %s, line %d \n",\
                                                    __PRETTY_FUNCTION__, __LINE__);  return;}

    void FuncMapFilling()
    {
        CHCKEMPLACE("push", FUNC::PUSH_NUM, Func)
        CHCKEMPLACE("pop", FUNC::POP_DEF, Func)
        CHCKEMPLACE("out", FUNC::OUT, Func)
        CHCKEMPLACE("mul", FUNC::MUL, Func)
        CHCKEMPLACE("div", FUNC::DIV, Func)
        CHCKEMPLACE("add", FUNC::ADD, Func)
        CHCKEMPLACE("sub", FUNC::SUB, Func)
        CHCKEMPLACE("jmp", FUNC::JMP, Func)
        CHCKEMPLACE("je", FUNC::JE, Func)
        CHCKEMPLACE("jl", FUNC::JL, Func)
        CHCKEMPLACE("jb", FUNC::JB, Func)
        CHCKEMPLACE("end", FUNC::END, Func)
        CHCKEMPLACE("call", FUNC::CALL, Func)
        CHCKEMPLACE("ret", FUNC::RET, Func)
    }

    void RegMapFilling()
    {
        CHCKEMPLACE("ax", REG::AX, Reg)
        CHCKEMPLACE("bx", REG::BX, Reg)
        CHCKEMPLACE("cx", REG::CX, Reg)
        CHCKEMPLACE("dx", REG::DX, Reg)
        CHCKEMPLACE("eb", REG::EB, Reg)
        CHCKEMPLACE("es", REG::ES, Reg)
    }

#undef CHCKEMPLACE

friend assembly;
};
//----------------//

class assembly {
public:

    std::string buf_;
    std::vector<token> tokenbuf_;
    std::vector<transtoken> transbuf_;
    maps my_maps_;


    assembly() :
            buf_(),
            tokenbuf_(),
            my_maps_()
            {}

    ~assembly() = default;

//===========================================================================//
//=================================FUNCTIONS=================================//
//===========================================================================//

    int ReadAsmCode() {
        FILE *file = fopen("ASM.txt", "r"); int line = __LINE__;

        if (!file)
        {
            std::cout << '\n' << "Can't open file ASM.txt in function: " << __PRETTY_FUNCTION__ << " line:" << line;
            return ERRCODE;
        }

        fseek(file, 0, SEEK_END);
        int strsize = ftell(file);
        rewind(file);

        buf_.resize(strsize, '\n');
        fread(&buf_[0], strsize, sizeof(char), file);

        fclose(file);

        return 0;
    }

#define CATCH catch(std::exception& exc)\
                {\
                    std::cout<< exc.what() << "in function: " << __PRETTY_FUNCTION__ << "line: " << __LINE__ << std::endl;\
                }

    void CreateTokens() {
        token tkn;
        int i = 0;
        int linenum = 1;

        while (i < buf_.size()) {
            while (((buf_[i] == ' ') || (buf_[i] == '\n')) && (i < buf_.size()))
            {
                if(buf_[i] == '\n')
                    ++linenum;
                ++i;
            }

            while ((buf_[i] != ' ') && (buf_[i] != '\n') && (buf_[i] != ';') && (i < buf_.size()))
            {
                tkn.str_.push_back(buf_[i]);
                ++i;
            }

            if (!tkn.str_.empty()) {
                tkn.line_ = linenum;

                try {
                    tokenbuf_.push_back(tkn);
                }
                CATCH

                tkn.clear();
            }

            if ((buf_[i] == ';') && (i < buf_.size())) {
                while (buf_[i] != '\n')
                    ++i;

                ++linenum;
                ++i;
            }
        }
    }

#define FUNCMAP my_maps_.FuncMap
#define REGMAP my_maps_.RegMap
#define LBLMAP my_maps_.LblMap
#define FOOMAP my_maps_.AsmFuncMap

    void Translation()
    {
        if(scanfuncs() == ERRCODE)
            return;
        if(scanlbls() == ERRCODE)
            return;
        /*if(filltransbuf() == ERRCODE)
            return;
        if(checklogic() == ERRCODE)
            return;*/
    }


    int scanfuncs()
    {
        int funcnum = 0;
        std::string funcname;

        for (int ip = 0; ip < tokenbuf_.size(); ++ip) {

            if(tokenbuf_[ip].str_ == FOO)
            {
                if((FUNCMAP.find(tokenbuf_[ip+1].str_) != FUNCMAP.end()) || (REGMAP.find(tokenbuf_[ip+1].str_) != REGMAP.end())) {
                    std::cout << SYNT << "Can't name function ";
                    tokenbuf_[ip+1].err_dump();
                    return ERRCODE;
                }

                if(!FOOMAP.emplace(tokenbuf_[ip+1].str_, ip - 2*funcnum).second) {
                    std::cout << "Can't emplace in AsmFuncMap: func " << __PRETTY_FUNCTION__ << ", line " << __LINE__ << std::endl;
                    return ERRCODE;
                }

                ++ip;

                if(!checkRET(ip))
                    return ERRCODE;

                funcnum++;
            }
        }

        return 0;
    }


    bool checkRET(int index)
    {
        assert(index >= 0);
        while(tokenbuf_[index].str_ != "main")
        {
            if(tokenbuf_[index].str_ == FOO) {
                std::cout << SYNT << "Met func declaration without ret before ";
                tokenbuf_[index].err_dump();
                return false;
            }

            if(tokenbuf_[index].str_ == "ret")
                return true;

            index++;
        }

        std::cout << "forgot ret at line:" << tokenbuf_[--index].line_ + 1 << std::endl;
        return false;
    }


    int scanlbls()
    {
        int lblsnum = 0;
        int lable_size = 0;

        std::string lable;

        for (int ip = 0; ip < tokenbuf_.size(); ++ip) {

            if (tokenbuf_[ip].str_[0] == ':') {

                lable_size = tokenbuf_[ip].str_.size() - 1;
                lable.resize(lable_size);
                lable_size = 0;

                for (int i = 0; i < lable_size; ++i) {
                    lable[i] = tokenbuf_[ip].str_[i + 1];
                }

                if(LBLMAP.find(lable) != LBLMAP.end()) {
                    std::cout << SYNT << "met lable :" << tokenbuf_[ip].str_ << " more tham 1 time line:" << tokenbuf_[ip].line_ << std::endl;
                    return ERRCODE;
                }

                if (!LBLMAP.emplace(lable, ip - lblsnum).second) {
                    std::cout << "Can't emplace in LblMap: func " << __PRETTY_FUNCTION__ << ", line " << __LINE__;
                    return ERRCODE;
                }

                ++lblsnum;
            }
        }
        return 0;
    }


    int filltransbuf() {
        transtoken transtkn('0', 0);

        std::map<std::string, int>::iterator FuncIter, RegIter, LblIter, AsmFuncIter;

        for (int ip = 0; ip < tokenbuf_.size(); ++ip) {

            FuncIter    = FUNCMAP.find(tokenbuf_[ip].str_);
            RegIter     = REGMAP.find(tokenbuf_[ip].str_);
            LblIter     = LBLMAP.find(tokenbuf_[ip].str_);
            AsmFuncIter = FOOMAP.find(tokenbuf_[ip].str_);


            if (FuncIter != FUNCMAP.end() || RegIter != REGMAP.end()) {

                if (RegIter != REGMAP.end()) {

                    transtkn.set(FUNCTN, FuncIter->second);

                    try {
                        transbuf_.push_back(transtkn);
                    }

                    CATCH
                }

                else {

                    transtkn.set(REGSTR, RegIter->second);

                    try {
                        transbuf_.push_back(transtkn);
                    }

                    CATCH
                }
            }

            else {

                int notanumber = 0;
                int number = string_to_int(tokenbuf_[ip], notanumber);

                if (notanumber == 0) {
                    transtkn.set(NMBR, number);

                    try {
                        transbuf_.push_back(transtkn);
                    }

                    CATCH
                }

                else {
                    search = LBLMAP.find(tokenbuf_[ip]);

                    if (search != LBLMAP.end()) {
                        transtkn.set(LBL, search -> second);

                        try {
                            transbuf_.push_back(transtkn);
                        }

                        CATCH
                    }

                    else {
                        if(tokenbuf_[ip][0] == '[') {
                            if(tokenbuf_[ip][tokenbuf_[ip].size() - 1] != ']') {
                                std::cout << SYNT << tokenbuf_[ip] << "!!!!" << std::endl;
                                return ERRCODE;
                            }

                            if (checkRAMtoken() == NOTRAM) {  // дописать эту функцию
                                std::cout << SYNT << tokenbuf_[ip] << "!!!!" << std::endl;
                                return ERRCODE;
                            }

                            else{

                            }
                        }

                        if(tokenbuf_[ip][0] != ':') {
                            std::cout << SYNT << tokenbuf_[ip] << "!!!!" << std::endl;
                            return ERRCODE;
                        }
                    }
                }
            }
        }

        return 0;
    }
/*
    void checklogic() {
        for (int i = 0; i < transbuf_.size(); ++i) {

            switch (transbuf_[i].tokentype_) {

                case (FUNCTN): {
                    switch (transbuf_[i].code_) {


                        case (FUNC::PUSH): {
                            if ((transbuf_[i + 1].tokentype_ != NMBR) && (transbuf_[i + 1].tokentype_ != REGSTR)) {
                                std::cout << SYNT << "Bad argument in function push!!!command ip:" << i << std::endl;
                                return;
                            }

                            ++i;
                            break;
                        }

                        case (FUNC::JMP): {
                            if ((transbuf_[i + 1].tokentype_ != LBL) && (transbuf_[i + 1].tokentype_ != NMBR)) {
                                std::cout << SYNT << "Bad argument in function jmp!!!  command ip:" << i << std::endl;
                                return;
                            }

                            ++i;
                            break;
                        }

                        case (FUNC::END): {
                            if (i != transbuf_.size() - 1) {
                                std::cout << SYNT << "End in the wrong place!!!  command ip:" << i << std::endl;
                                return;
                            }
                            break;
                        }

                        default:
                            break;
                    }

                    break;
                }

                default:
                    std::cout << SYNT << transbuf_[i].code_;
                    return;
            }
        }

        if((transbuf_[transbuf_.size() - 1].tokentype_ != FUNCTN) || transbuf_[transbuf_.size() - 1].code_ != FUNC::END)
        {
            std::cout << SYNT << "forgot end at the end" << std::endl;
            return;
        }
    }*/

#undef FUNCMAP
#undef REGMAP
#undef LBLMAP
#undef CATCH
//===============RAM_TOKEN_CHECK===============//

int checkRAMtoken(transtoken& tkn)
{

}

//===============FUNC_LOGIC_CHECKERS===============//

    int checkPUSH(int index)
    {
        if ((transbuf_[index + 1].tokentype_ != NMBR) && (transbuf_[index + 1].tokentype_ != REGSTR)
        && (transbuf_[index + 1].tokentype_ != RAM))
        {
            std::cout << SYNT << "Bad argument in function: push" << std::endl;
            return ERRCODE;
        }

        return 0;
    }

    int checkPOP(int index)
    {
        if ((transbuf_[index + 1].tokentype_ != NMBR) && (transbuf_[index + 1].tokentype_ != REGSTR)
            && (transbuf_[index + 1].tokentype_ != RAM))
        {
            std::cout << SYNT << "Bad argument in function: pop" << std::endl;
            return ERRCODE;
        }

        return 0;
    }

    int checkJMP(int index)
    {
        if ((transbuf_[index + 1].tokentype_ != LBL) && (transbuf_[index + 1].tokentype_ != NMBR))
        {
            std::cout << SYNT << "Bad argument in function: push" << std::endl;
            return ERRCODE;
        }

        return 0;
    }

    int checkEND(int index)
    {
        if(index != transbuf_.size() - 1)
        {
            std::cout << SYNT << "End in the wrong place" << std::endl;
            return ERRCODE;
        }

        return 0;
    }

};

int main() {

    assembly* ass = new assembly;

    ass -> ReadAsmCode();
    ass -> CreateTokens();

    std::cout <<  '\n'  <<   ass -> scanfuncs() << std::endl;

    std::cout << ass -> my_maps_.AsmFuncMap["mul1"];

    /*for(int i = 0; i < ass -> tokenbuf_.size(); i++)
    {
        ass -> tokenbuf_[i].dump();
    }*/

    delete ass;

    return 0;
}

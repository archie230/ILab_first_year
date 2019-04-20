#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <map>
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
    AX                 = 26,
    BX                 = 27,
    CX                 = 28,
    DX                 = 29,
    EB                 = 30,
    ES                 = 31
};

//-------------------//
enum ERRS{
    ERRCODE            = -1,
};

//-------------------//
const char SYNT[]    = "\tSyntactic error: ";
const char FOO[]     = "func";

const char FUNCTN    = 'f';
const char REGSTR    = 'r';
const char ASM_FUNC  = 'e';
const char LBL       = 'l';

const char NMBR      = 'n';
const char RAM_NUM   = 'o';
const char RAM_REG   = 't';
const char RAM_A_NUM = 'q';
const char RAM_S_NUM = 'w';
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
        std::cout << SYNT << std::endl;
        std::cout << str_ << std::endl;
        std::cout << " line: " << line_ << std::endl;
    }

    friend assembly;
};

class transtoken{
public:
    char tokentype_;
    int code_;
    int line_;

    transtoken(char tokentype, int code, int line):
        tokentype_(tokentype),
        code_(code),
        line_(line)
    {}

    ~transtoken() = default;

    void set(char tokentype, int code, int line)
    {
        assert((line >= 0) && (code >= 0));
        tokentype_ = tokentype;
        code_ = code;
        line_ = line;
    }

    void err_dump()
    {
        std::cout << SYNT << std::endl;
        std::cout << " line: " << line_ << std::endl;
    }

//debug printing
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
        CHCKEMPLACE("main", FUNC::MAIN, Func)
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

#define CATCH catch(std::exception& exc)\
                {\
                    std::cout<< exc.what() << "in function: " << __PRETTY_FUNCTION__ \
                                                                            << "line: " << __LINE__ << std::endl;\
                    return ERRS::ERRCODE;\
                }

#define FUNCMAP my_maps_.FuncMap
#define REGMAP my_maps_.RegMap
#define LBLMAP my_maps_.LblMap
#define FOOMAP my_maps_.AsmFuncMap

//===========================================================================//
//=================================FUNCTIONS=================================//
//===========================================================================//
#define DO(funcname) if(funcname() == ERRS::ERRCODE) return ERRS::ERRCODE;

    int assembly_code()
    {
        DO(ReadAsmCode)
        DO(CreateTokens)
        DO(scan_funcs_and_lbls)
        DO(filltransbuf)
        DO(checklogic)

        return 0;
    }

    int ReadAsmCode() {
        FILE *file = fopen("ASM.txt", "r"); int line = __LINE__;

        if (!file)
        {
            std::cout << '\n' << "Can't open file ASM.txt in function: " << __PRETTY_FUNCTION__ << " line:" << line;
            return ERRS::ERRCODE;
        }

        fseek(file, 0, SEEK_END);
        int strsize = ftell(file);
        rewind(file);

        buf_.resize(strsize, '\n');
        fread(&buf_[0], strsize, sizeof(char), file);

        fclose(file);

        return 0;
    }


    int CreateTokens() {
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

        return 0;
    }


    int scan_funcs_and_lbls()
    {
        int funcnum = 0;
        int lblsnum = 0;

        std::string lable;
        int main_adr = findMAIN();

        for (int ip = 0; ip < tokenbuf_.size(); ++ip) {

            if(tokenbuf_[ip].str_ == FOO)
            {
                if((FUNCMAP.find(tokenbuf_[ip+1].str_) != FUNCMAP.end()) || (REGMAP.find(tokenbuf_[ip+1].str_) != REGMAP.end())) {
                    std::cout << SYNT << "Can't name function " << tokenbuf_[ip+1].str_ << " line:" << tokenbuf_[ip+1].line_ << std::endl;
                    return ERRS::ERRCODE;
                }

                if((ip + 1 > main_adr) && (main_adr !=0))
                {
                    std::cout << "met function declaration after start point (main)" << std::endl;
                    return ERRS::ERRCODE;
                }


                if(!FOOMAP.emplace(tokenbuf_[ip+1].str_, ip - 2*funcnum - lblsnum).second) {
                    std::cout << "Can't emplace in AsmFuncMap: func " << __PRETTY_FUNCTION__ << ", line " << __LINE__ << std::endl;
                    return ERRS::ERRCODE;
                }

                ++ip;
                if(!findRET(ip))
                    return ERRS::ERRCODE;


                funcnum++;
            }

            if (tokenbuf_[ip].str_[0] == ':') {

                lable.resize(tokenbuf_[ip].str_.size() - 1);

                for (int i = 0; i < lable.size(); ++i) {
                    lable[i] = tokenbuf_[ip].str_[i + 1];
                }

                if(LBLMAP.find(lable) != LBLMAP.end()) {
                    std::cout << SYNT << "met lable :" << tokenbuf_[ip].str_ << " more tham 1 time line:" << tokenbuf_[ip].line_ << std::endl;
                    return ERRS::ERRCODE;
                }

                if(REGMAP.find(lable) != REGMAP.end() || FUNCMAP.find(lable) != FUNCMAP.end()) {
                    std::cout << SYNT << "can't name lable:" << std::endl;
                    tokenbuf_[ip].err_dump();
                    return ERRS::ERRCODE;
                }

                if (!LBLMAP.emplace(lable, ip - 2*funcnum - lblsnum).second) {
                    std::cout << "Can't emplace in LblMap: func " << __PRETTY_FUNCTION__ << ", line " << __LINE__;
                    return ERRS::ERRCODE;
                }

                lable.erase();

                ++lblsnum;
            }


        }

        return 0;
    }


    bool findRET(int index)
    {
        assert(index >= 0);

        while((tokenbuf_[index].str_ != "main") && (index < tokenbuf_.size()))
        {
            if(tokenbuf_[index].str_ == FOO) {
                std::cout << SYNT << "met func declaration without ret before ";
                tokenbuf_[index].err_dump();
                return false;
            }

            if(tokenbuf_[index].str_ == "ret")
                return true;

            index++;
        }


        std::cout << "forgot ret at line: " << tokenbuf_[--index].line_ + 1 << std::endl;

        return false;
    }


    int findMAIN()
    {
        for(int ip = 0; ip < tokenbuf_.size(); ip++)
        {
            if(tokenbuf_[ip].str_ == "main")
                return ip;
        }

        return 0;
    }


//===============================TOKEN_CHECKERS===============================//
    int transbuf_insert(char transtkntype, int code, int line)
    {
        assert(line >= 1);

        transtoken transtkn(transtkntype, code, line);
        try {
            transbuf_.push_back(transtkn);
        }

        CATCH

        return 0;
    }


    bool check_num(std::string& tkn_str)
    {
        try {
            int number = string_to_int(tkn_str);
        }

        catch(std::logic_error& err)
        {
            return false;
        }

        return true;
    }


    bool check_lbl(std::string& tkn_str)
    {
        if(tkn_str[0] != ':')
            return false;

        std::string lbl(tkn_str.size() - 1, '\0');
        for(int i = 0; i < lbl.size(); i++)
        {
            lbl[i] = tkn_str[i+1];
        }

        auto nemo = LBLMAP.find(lbl);
        return !(nemo == LBLMAP.end());
    }


    int* check_RAM_NUM(const std::string& tkn_str)
    {
        if((tkn_str[0] != '[') || (tkn_str[tkn_str.size() - 1] != ']')) {
            return nullptr;
        }

        std::string inner(tkn_str.size() - 2 , '\0');

        for(int i = 0; i < inner.size(); i++) {
            inner[i] = tkn_str[i+1];
        }

        int numbr = 0;

        try {
            numbr = string_to_int(inner);
        }

        catch(std::logic_error& err) {
            return nullptr;
        }

        int* RAM_NUM_ptr = new(std::nothrow) int;
        *RAM_NUM_ptr = numbr;

        return RAM_NUM_ptr;
    }


    int* check_RAM_REG(const std::string& tkn_str)
    {
        if((tkn_str[0] != '[') || (tkn_str[tkn_str.size() - 1] != ']')) {
            return nullptr;
        }

        std::string inner(tkn_str.size() - 2 , '\0');
        for(int i = 0; i < inner.size(); i++) {
            inner[i] = tkn_str[i+1];
        }


        std::map<std::string, int>::iterator nemo = REGMAP.find(inner);
        if(nemo == REGMAP.end())
            return nullptr;

        int* RAM_REG_ptr = new(std::nothrow) int;
        *RAM_REG_ptr = nemo -> second;
        return RAM_REG_ptr;
    }


    int* check_RAM_OPERATOR_NUM(const std::string& tkn_str, char opertr)
    {
        if((tkn_str[0] != '[') || (tkn_str[tkn_str.size() - 1] != ']'))
            return nullptr;

        std::string reg_str(2, '\0');
        for(int i = 0; i < 2; i++)
        {
            reg_str[i] = tkn_str[i+1];
        }


        std::map<std::string, int>::iterator RegIter = REGMAP.find(reg_str);


        switch(opertr){
            case('+'): {
                if((RegIter == REGMAP.end()) || tkn_str[3] != '+')
                    return nullptr;
                break;
            }

            case('-'): {
                if((RegIter == REGMAP.end()) || tkn_str[3] != '-')
                    return nullptr;
                break;
            }

            default: {
                std::cout << "unexpected operator " << opertr << " in function: " << __PRETTY_FUNCTION__;
                return nullptr;
            }
        }

        std::string num_str(tkn_str.size() - 5, '\0');
        for(int i = 0; i < num_str.size(); i++)
        {
            num_str[i] = tkn_str[i+4];
        }

        int numbr = 0;
        try {
            numbr = string_to_int(num_str);
        }
        catch(std::logic_error& err)
        {
            return nullptr;
        }

        int* RAM_OPERATOR_NUM_ptr = new(std::nothrow) int;

        short* reg = (short*) RAM_OPERATOR_NUM_ptr;
        short* num = reg + 1;

        *reg = RegIter -> second;
        *num = numbr;

        return RAM_OPERATOR_NUM_ptr;
    }
//============================================================================//


    int filltransbuf() {
                    
        std::map<std::string, int>::iterator FuncIter, RegIter, LblIter, AsmFuncIter;

        for (int ip = 0; ip < tokenbuf_.size(); ++ip) {

            FuncIter    = FUNCMAP.find(tokenbuf_[ip].str_);
            RegIter     = REGMAP.find(tokenbuf_[ip].str_);
            LblIter     = LBLMAP.find(tokenbuf_[ip].str_);
            AsmFuncIter = FOOMAP.find(tokenbuf_[ip].str_);

            if(tokenbuf_[ip].str_ == FOO)
                continue;

            if (FuncIter != FUNCMAP.end()) {

                if(transbuf_insert(FUNCTN, FuncIter -> second, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                    return ERRS::ERRCODE;
                continue;

            }

            if (RegIter != REGMAP.end()) {

                if(transbuf_insert(REGSTR, RegIter -> second, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                    return ERRS::ERRCODE;
                continue;

            }

            if (LblIter != LBLMAP.end()) {

                if(transbuf_insert(LBL, LblIter -> second, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                    return ERRS::ERRCODE;
                continue;

            }

            if (AsmFuncIter != FOOMAP.end()) {

                if(tokenbuf_[ip - 1].str_ == FOO)
                    continue;

                else {
                    if (transbuf_insert(ASM_FUNC, AsmFuncIter->second, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                        return ERRS::ERRCODE;
                    continue;
                }
            }

            if (check_num(tokenbuf_[ip].str_)) {

                if(transbuf_insert(NMBR, string_to_int(tokenbuf_[ip].str_), tokenbuf_[ip].line_) == ERRS::ERRCODE)
                    return ERRS::ERRCODE;
                continue;

            }

            int* RAM_NUM_ptr = check_RAM_NUM(tokenbuf_[ip].str_);
            if (RAM_NUM_ptr) {

                if(transbuf_insert(RAM_NUM, *RAM_NUM_ptr, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                    return ERRS::ERRCODE;

                delete RAM_NUM_ptr;
                continue;

            }


            int* RAM_REG_ptr = check_RAM_REG(tokenbuf_[ip].str_);
            if (RAM_REG_ptr) {

                if(transbuf_insert(RAM_REG, *RAM_REG_ptr, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                    return ERRS::ERRCODE;

                delete RAM_REG_ptr;
                continue;

            }


            int* RAM_A_NUM_ptr = check_RAM_OPERATOR_NUM(tokenbuf_[ip].str_, '+');
            if(RAM_A_NUM_ptr) {

                if(transbuf_insert(RAM_A_NUM, *RAM_A_NUM_ptr, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                    return ERRS::ERRCODE;

                delete RAM_A_NUM_ptr;
                continue;

            }

            int* RAM_S_NUM_ptr = check_RAM_OPERATOR_NUM(tokenbuf_[ip].str_, '-');
            if(RAM_S_NUM_ptr) {

                if(transbuf_insert(RAM_S_NUM, *RAM_S_NUM_ptr, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                    return ERRS::ERRCODE;

                delete RAM_S_NUM_ptr;
                continue;

            }

            if(check_lbl(tokenbuf_[ip].str_))
                continue;

            tokenbuf_[ip].err_dump();
            return ERRS::ERRCODE;
        }

        return 0;
    }


#define CHECKER(checkingfunc) if(checkingfunc(ip) == false) {\
                                    transbuf_[ip].err_dump();\
                                    return ERRS::ERRCODE; }\
                                else {\
                                    ip++;\
                                    continue; }

    int checklogic()
    {
        int main_flag = 0;
        int end_flag = 0;

        for(int ip = 0; ip < transbuf_.size(); ++ip)
        {
            switch(transbuf_[ip].tokentype_)
            {
                case(FUNCTN):
                {
                    switch(transbuf_[ip].code_)
                    {
                        case(FUNC::PUSH_NUM):
                        {
                            CHECKER(inspectPUSH)
                        }

                        case(FUNC::POP_DEF):
                        {
                            CHECKER(inspectPOP)
                        }

                        case(FUNC::JMP):
                        {
                            CHECKER(inspectJUMP)
                        }

                        case(FUNC::JE):
                        {
                            CHECKER(inspectJUMP)
                        }

                        case(FUNC::JL):
                        {
                            CHECKER(inspectJUMP)
                        }

                        case(FUNC::JB):
                        {
                            CHECKER(inspectJUMP)
                        }

                        case(FUNC::CALL):
                        {
                            CHECKER(inspectCALL)
                        }

                        case(FUNC::MAIN):
                        {
                            main_flag++;
                            continue;
                            break;
                        }

                        case(FUNC::END):
                        {
                            if(main_flag == 0)
                            {
                                std::cout << "met end point (end) before main" << std::endl;
                                return ERRS::ERRCODE;
                            }

                            end_flag++;
                            continue;
                            break;
                        }

                        default:
                        {
                            continue;
                        }
                    }
                    break;
                }

                default:
                {
                    tokenbuf_[ip].err_dump();
                    return ERRS::ERRCODE;
                }

            }
        }

        if(main_flag == 0)
        {
            std::cout << "no start point (main)" << std::endl;
            return ERRS::ERRCODE;
        }

        if(main_flag > 1)
        {
            std::cout << "met start point (main) many times" << std::endl;
            return ERRS::ERRCODE;
        }

        if(end_flag == 0)
        {
            std::cout << "forgot end point (end)" << std::endl;
            return ERRS::ERRCODE;
        }

        if(end_flag > 1)
        {
            std::cout << "met end point (end) many times" << std::endl;
            return ERRS::ERRCODE;
        }
            return 0;
    }


#undef CHECKER
//===============LOGIC_PATROL===============//

    bool inspectPUSH(int ip)
    {
        switch(transbuf_[ip+1].tokentype_)
        {
            case(NMBR): {
                return true;
            }

            case(REGSTR):
            {
                transbuf_[ip].code_ = FUNC::PUSH_REG;
                return true;
            }

            case(RAM_NUM):
            {
                transbuf_[ip].code_ = FUNC::PUSH_RAM_NUM;
                return true;
            }

            case(RAM_REG):
            {
                transbuf_[ip].code_ = FUNC::PUSH_RAM_REG;
                return true;
            }

            case(RAM_A_NUM):
            {
                transbuf_[ip].code_ = FUNC::PUSH_RAM_REG_A_NUM;
                return true;
            }

            case(RAM_S_NUM):
            {
                transbuf_[ip].code_ = FUNC::PUSH_RAM_REG_S_NUM;
                return true;
            }

            default: {
                return false;
            }
        }
    }
    
    
    bool inspectPOP(int ip)
    {
        switch(transbuf_[ip+1].tokentype_)
        {
            case(NMBR): {
                return true;
            }

            case(REGSTR):
            {
                transbuf_[ip].code_ = FUNC::POP_REG;
                return true;
            }

            case(RAM_NUM):
            {
                transbuf_[ip].code_ = FUNC::POP_RAM_NUM;
                return true;
            }

            case(RAM_REG):
            {
                transbuf_[ip].code_ = FUNC::POP_RAM_REG;
                return true;
            }

            case(RAM_A_NUM):
            {
                transbuf_[ip].code_ = FUNC::POP_RAM_REG_A_NUM;
                return true;
            }

            case(RAM_S_NUM):
            {
                transbuf_[ip].code_ = FUNC::POP_RAM_REG_S_NUM;
                return true;
            }

            default: {
                return false;
            }
        }
    }


    bool inspectJUMP(int ip)
    {
        return transbuf_[ip + 1].code_ != LBL ? false : true;
    }


    bool inspectCALL(int ip)
    {
        return transbuf_[ip + 1].code_ != ASM_FUNC ? false : true;
    }



//==========================================//
#undef FUNCMAP
#undef REGMAP
#undef LBLMAP
#undef CATCH

};

int main() {


    assembly* ass = new assembly;


    int oops = ass -> assembly_code();

    if(oops == 0)
    {
        for(int i = 0; i < ass -> transbuf_.size(); i++)
            ass -> transbuf_[i].print();
    }

    delete ass;


    return 0;
}

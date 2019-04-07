#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
#include <map>
#include "string_to_int.hpp"


//-------------------//
enum FUNC
{
    PUSH = 1,
    POP,
    OUT,
    MUL,
    DIV,
    ADD,
    SUB,
    JMP,
    END,//9
};

enum REG
{
    AX = 10,
    BX,
    CX,
    DX
};

const char SYNT[] = "Syntactic error: ";

const char NMBR = 'n';
const char REGSTR = 'r';
const char LBL = 'l';
const char FUNCTN = 'f';
//------------------//

class transtoken{
public:
    char tokentype_;
    int code_;

    transtoken(char tokentype, int code):
    tokentype_(tokentype),
    code_(code)
    {}

    void set(char tokentype, int code)
    {
        tokentype_ = tokentype;
        code_ = code;
    }

    void print()
    {
        std::cout << "-------------" << std::endl;
        std::cout << "tokentype: " << tokentype_ << '\n';
        std::cout<< "code: " << code_ << '\n';
    }

};

class maps
{
public:
    std::map<std::string, int> FuncMap;
    std::map<std::string, int> RegMap;
    std::map<std::string, int> LblMap;

    maps()
    {
        FuncMapFilling();
        RegMapFilling();
    }



#define CHCKEMPLACE(str, const, Mapname)\
                                         if( Mapname##Map.emplace(str, const).second != true) {\
                                                    printf("Can't emplace in func %s, line %d \n",\
                                                             __PRETTY_FUNCTION__, __LINE__);  return;}

    void FuncMapFilling()
    {
        CHCKEMPLACE("push", FUNC::PUSH, Func);
        CHCKEMPLACE("pop", FUNC::POP, Func);
        CHCKEMPLACE("out", FUNC::OUT, Func);
        CHCKEMPLACE("mul", FUNC::MUL, Func);
        CHCKEMPLACE("div", FUNC::DIV, Func);
        CHCKEMPLACE("add", FUNC::ADD, Func);
        CHCKEMPLACE("sub", FUNC::SUB, Func);
        CHCKEMPLACE("jump", FUNC::JMP, Func);
        CHCKEMPLACE("end", FUNC::END, Func);
    }

    void RegMapFilling()
    {
        CHCKEMPLACE("ax", REG::AX, Reg);
        CHCKEMPLACE("bx", REG::BX, Reg);
        CHCKEMPLACE("cx", REG::CX, Reg);
        CHCKEMPLACE("dx", REG::DX, Reg);
    }

#undef CHCKEMPLACE

};

class compiler {
public:

    std::string buf_;                         //буффер строк
    std::vector<std::string> tokenbuf_;      //вектор токенов
    std::vector<transtoken> transbuf_;      //вектор токенов трансл€ции
    maps my_maps_;                         //карты функций, регистров, переменных


    compiler() :
            buf_(),
            tokenbuf_(),
            my_maps_() {}

    ~compiler() = default;

    void ReadAsmCode() {
        FILE *file = fopen("ASM.txt", "r"); int line = __LINE__;


        if (!file) {
            std::cout << '\n' << "Can't open file ASM.txt in function: " << __PRETTY_FUNCTION__ << " line:" << line;
            return;
        }

        fseek(file, 0, SEEK_END);
        int strsize = ftell(file);
        rewind(file);

        buf_.resize(strsize, '\n');
        fread(&buf_[0], strsize, sizeof(char), file);

        fclose(file);
    }

#define CATCH catch(std::exception& exc)\
                {\
                    std::cout<< exc.what() << "in function: " << __PRETTY_FUNCTION__ << "line: " << __LINE__ << std::endl;\
                }

    void CreateTokens() {
        std::string token;
        int i = 0;

        while (i < buf_.size()) {
            while (((buf_[i] == ' ') || (buf_[i] == '\n')) && (i < buf_.size()))
                ++i;

            while ((buf_[i] != ' ') && (buf_[i] != '\n') && (buf_[i] != ';') && (i < buf_.size())) {
                token.push_back(buf_[i]);
                ++i;
            }

            if (token.empty() == 0) {
                try {
                    tokenbuf_.push_back(token);
                    token.erase();
                }

                CATCH
            }

            if ((buf_[i] == ';') && (i < buf_.size())) {
                while (buf_[i] != '\n')
                    ++i;

                ++i;
            }
        }
    }

#define FUNCMAP my_maps_.FuncMap
#define REGMAP my_maps_.RegMap
#define LBLMAP my_maps_.LblMap

    void Translation() {
        scanlbls();
        filltransbuf();
        //checklogic();
    }

    void scanlbls() {
        int ip = 0;
        int lblsnum = 0;

        std::map<std::string, int>::iterator search;

        for (ip = 0; ip < tokenbuf_.size(); ++ip) {
            if (tokenbuf_[ip][0] == ':') {

                int lable_size = tokenbuf_[ip].size() - 1;
                std::string lable(lable_size, '\0');

                for (int i = 0; i < lable_size; ++i) {
                    lable[i] = tokenbuf_[ip][i + 1];
                }

                search = LBLMAP.find(lable);

                if(search != LBLMAP.end()) {
                    std::cout << SYNT << "met lable :" << lable << " more tham 1 time" << std::endl;
                    return;
                }

                if (!LBLMAP.emplace(lable, ip - lblsnum).second) {
                    printf("Can't emplace in func %s, line %d \n", __PRETTY_FUNCTION__, __LINE__);
                    return;
                }

                ++lblsnum;
            }
        }
    }

    void filltransbuf() {
        transtoken transtkn('0', 0);

        std::map<std::string, int>::iterator FuncIter;
        std::map<std::string, int>::iterator RegIter;

        for (int ip = 0; ip < tokenbuf_.size(); ++ip) {

            if ((tokenbuf_[ip])[0] == ':')                     //problem!!!!!!
                ++ip;

            FuncIter = FUNCMAP.find(tokenbuf_[ip]);
            RegIter = REGMAP.find(tokenbuf_[ip]);

            if (FuncIter != FUNCMAP.end() || RegIter != REGMAP.end()) {

                if (FuncIter == FUNCMAP.end()) {

                    transtkn.set(REGSTR, RegIter->second);

                    try {
                        transbuf_.push_back(transtkn);
                    }

                    CATCH
                }

                else {

                    transtkn.set(FUNCTN, FuncIter->second);

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
                    auto search = LBLMAP.find(tokenbuf_[ip]);

                    if (search != LBLMAP.end()) {
                        transtkn.set(LBL, search -> second);

                        try {
                            transbuf_.push_back(transtkn);
                        }

                        CATCH
                    }

                    else {
                        std::cout << SYNT << tokenbuf_[ip] << "!!!!" << std::endl;
                        return;
                    }
                }
            }
        }
    }

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
                                std::cout << SYNT << "Bad argument in function jump!!!  command ip:" << i << std::endl;
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
    }

#undef FUNCMAP
#undef REGMAP
#undef LBLMAP
#undef CATCH


};

//добавить в токен номер строки дл€ отладки

int main() {

    compiler* comp = new compiler;

    comp -> ReadAsmCode();
    comp -> CreateTokens();
    comp -> Translation();

    for(int i = 0; i < comp -> transbuf_.size(); i++)
    {
        (comp -> transbuf_)[i].print();
    }

    delete comp;

    return 0;
}
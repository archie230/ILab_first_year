#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <map>

const int POISON = -230;

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
    JMP
};

enum REG
{
    AX,
    BX,
    CX,
    DX
};

enum ARGS
{
    NTHNG, //nothing
    NMBR, //number
    REG, //register
    L, //label
};
//-------------------//

int string_to_int(std::string str, int& flag) // протестить перевод из строки в число!!!!!
{
    int temp = 0;
    int sign = 0; // 0 if temp > 0, 1 if temp < 0
    int i = 0;

    if(str[i] == '-'){
        sign = 1;
        ++i;
    }

    while(i < str.size())
    {
        if((str[i] < '0') || (str[i] > '9'))
        {
            flag = 1;
            return POISON;
        }

        temp += (str[i] & 0x0F);
        temp = temp * 10;
        ++i;
    }

    temp = temp / 10;

    if(sign == 1)
        temp = -temp;

    return temp;
}


class transtoken{
public:
    char tokentype_;
    int param_;

    transtoken(char tokentype, int param):
    tokentype_(tokentype),
    param_(param)
    {}

    void set(char tokentype, int param)
    {
        tokentype_ = tokentype;
        param_ = param;
    }
};

class maps
{
public:
    std::map<std::string, int> FuncMap;
    std::map<std::string, int> RegMap;

    maps()
    {
        FuncMapFilling();
        RegMapFilling();
    }



#define CHCKEMPLACE(str, const, Mapname)\
                                         if( Mapname##Map.emplace(str, const).second != true)\
                                                    {printf("can't emplace in func %s, line %d",\
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

class compiler{
public:

    std::string buf_;        //буффер строк
    std::vector<std::string> tokenbuf_;      //вектор токенов
    std::vector<transtoken> transbuf_;      //вектор токенов трансляции
    maps my_maps_;


    compiler():
    buf_(),
    tokenbuf_(),
    my_maps_()
    {}

    ~compiler()
    {}

    void ReadAsmCode()
    {
        FILE* file = fopen("ASM.txt", "r"); int line = __LINE__;

        if(!file)
        {
            std::cout << '\n' << "Can't open file ASM.txt in function: " << __PRETTY_FUNCTION__ << " line:" << line;
            return;
        }

        fseek(file, 0, SEEK_END);
        int strsize = ftell(file);
        rewind(file);

        buf_.resize(strsize, '\0');
        fread(&buf_[0], strsize, sizeof(char), file);
        buf_.push_back('\n');

        fclose(file);
    }

#define CATCH catch(std::exception& exc)\
                {\
                    std::cout<< exc.what() << "in function: " << __PRETTY_FUNCTION__ << "line: " << __LINE__;\
                }

    void CreateTokens()
    {
        std::string token;
        int i = 0;

        while(i < buf_.size())
        {
            while (((buf_[i] == ' ') || (buf_[i] == '\n')) && (i < buf_.size()))
                ++i;

            while((buf_[i] != ' ') && (buf_[i] != '\n') && (buf_[i] != ';') && (i < buf_.size()))
            {
                token.push_back(buf_[i]);
                ++i;
            }

            if(token.empty() == 0)
            {
                try {
                    tokenbuf_.push_back(token);
                    token.erase();
                }

                CATCH
            }

            if((buf_[i] == ';') && (i < buf_.size()))
            {
                while(buf_[i] != '\n')
                    ++i;

                ++i;
            }
        }
    }

    void Translation()
    {
        transtoken transtkn('0', 0);

        for(auto it = tokenbuf_.begin(); it < tokenbuf_.end(); it++)
        {
            auto FuncIter = my_maps_.FuncMap.find(*it);
            auto RegIter = my_maps_.RegMap.find(*it);

            if(FuncIter != my_maps_.FuncMap.end() || RegIter != my_maps_.RegMap.end()) {

                if(FuncIter == my_maps_.FuncMap.end()){

                    transtkn.set('f', FuncIter -> second);

                    try {
                        transbuf_.push_back(transtkn);
                    }

                    CATCH
                }

                else{

                    transtkn.set('r', RegIter -> second);

                    try {
                        transbuf_.push_back(transtkn);
                    }

                    CATCH
                }
            }

            else{
                int notanumber = 0;
                int number = string_to_int(*it, notanumber);

                if(notanumber == 0)
                {
                    transtkn.set('n', number);

                    try{
                        transbuf_.push_back(transtkn);
                    }

                    CATCH
                }

                else{
                    std::cout << "syntactic error: " << *it << "!!!!\n";
                }
            }
        }
    }

};

#undef CATCH


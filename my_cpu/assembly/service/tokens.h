#pragma once
#include <cstdlib>
#include <iostream>
#include "../../lib/constants.h"

typedef struct token
{
    std::string str_;
    int line_;

    token();

    void clear();

    void err_dump();
}token;

typedef struct transtoken{
public:
    char tokentype_;
    int code_;
    int line_;

    explicit transtoken(char tokentype, int code, int line);

    void err_dump();

//debug printing
    void print();

}transtoken;

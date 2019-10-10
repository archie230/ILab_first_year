#include "tokens.h"
token::token()
        :   str_(),
            line_(0)
{}

void token::clear() {
    str_.erase();
    line_ = 0;
}

void token::err_dump() {
    std::cout << SYNT << std::endl;
    std::cout << '\t' << str_ << std::endl;
    std::cout << "\tline: " << line_ << std::endl;
}

transtoken::transtoken(char tokentype, int code, int line)
        :
        tokentype_(tokentype),
        code_(code),
        line_(line)
{}

void transtoken::err_dump() {
    std::cout << SYNT << std::endl;
    std::cout << "\tline: " << line_ << std::endl;
}

void transtoken::print() {
    std::cout << "--------------" << std::endl;
    std::cout << "tokentype: " << tokentype_ << std::endl;
    std::cout << "code:      " << code_ << std::endl;
    std::cout << "line:      " << line_ << std::endl;
}

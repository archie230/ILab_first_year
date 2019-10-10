#pragma once
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <vector>
#include "../lib/constants.h"
#include "service/maps.h"
#include "service/tokens.h"
#include "service/string_to_int.h"

const char OUTPUT[] = "CPU.notexe";

class assembly {
    std::string buf_;
    std::vector<token> tokenbuf_;
    std::vector<transtoken> transbuf_;
    maps my_maps_;

public:
    assembly();


    ~assembly() = default;


    int assembly_code();


    int ReadAsmCode();


    int CreateTokens();


    int scan_funcs_and_lbls();


    int filltransbuf();


    int checklogic();


    int create_exe_file();


    void write_lbl_or_funcs(FILE* exe_name, std::map<std::string, int>& map);


//===============================SERVICE FUNCS===============================//
    bool findRET(int index);


    int findMAIN();


    int transbuf_insert(char transtkntype, int code, int line);


    bool check_num(std::string& tkn_str);


    bool check_lbl(std::string& tkn_str);


    int* check_RAM_NUM(const std::string& tkn_str);


    int* check_RAM_REG(const std::string& tkn_str);


    int* check_RAM_OPERATOR_NUM(const std::string& tkn_str, char opertr);
//============================================================================//

//===============LOGIC_PATROL===============//

    bool inspectPUSH(int ip);


    bool inspectPOP(int ip);


    bool inspectJUMP(int ip);


    bool inspectCALL(int ip);

};

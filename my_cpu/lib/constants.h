#pragma once
//--------------------------//
enum FUNC
{
#define DEF_CMD(command, cmd_code, str, code) command = cmd_code,
#include "DEF_CMD.h"
#undef DEF_CMD
};

enum REG
{
#define DEF_REG(name, code, txt) name = code,
#include "DEF_REG.h"
#undef DEF_REG
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

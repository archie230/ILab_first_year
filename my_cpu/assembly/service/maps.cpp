#include "maps.h"
void maps::filling()
{
    FuncMapFilling();
    RegMapFilling();
}

#define CHCKEMPLACE(str, const, Mapname)\
                                         if( Mapname##Map.emplace(str, const).second != true) {\
                                                    printf("Can't emplace in func %s, line %d \n",\
                                                    __PRETTY_FUNCTION__, __LINE__);  return;}

void maps::FuncMapFilling()
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
    CHCKEMPLACE("ja", FUNC::JA, Func)
    CHCKEMPLACE("jb", FUNC::JB, Func)
    CHCKEMPLACE("end", FUNC::END, Func)
    CHCKEMPLACE("call", FUNC::CALL, Func)
    CHCKEMPLACE("ret", FUNC::RET, Func)
    CHCKEMPLACE("main", FUNC::MAIN, Func)
    CHCKEMPLACE("int", FUNC::INT, Func)
    CHCKEMPLACE("sqrt", FUNC::SQRT, Func)
}

void maps::RegMapFilling()
{
    CHCKEMPLACE("ax", REG::AX, Reg)
    CHCKEMPLACE("bx", REG::BX, Reg)
    CHCKEMPLACE("cx", REG::CX, Reg)
    CHCKEMPLACE("dx", REG::DX, Reg)
    CHCKEMPLACE("ox", REG::OX, Reg)
    CHCKEMPLACE("eb", REG::EB, Reg)
    CHCKEMPLACE("es", REG::ES, Reg)
}

#undef CHCKEMPLACE

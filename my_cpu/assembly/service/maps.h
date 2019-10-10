#pragma once
#include <map>
#include "../../lib/constants.h"

class maps
{
public:
    std::map<std::string, int> FuncMap;
    std::map<std::string, int> RegMap;
    std::map<std::string, int> LblMap;
    std::map<std::string, int> AsmFuncMap;

    void filling();

    void FuncMapFilling();

    void RegMapFilling();
};

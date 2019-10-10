#include "string_to_int.h"

int string_to_int(const std::string& str)
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
            throw std::logic_error("not a number");
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
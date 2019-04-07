#pragma once

const int POISON = -230;

int string_to_int(std::string str, int& flag)
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

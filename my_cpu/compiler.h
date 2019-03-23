#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <string>

class compiler{
public:

    std::string buf_;
    std::vector<std::string>* tokenbuf_;

    compiler():
            buf_(),
            tokenbuf_(nullptr)
    {}

    ~compiler()
    {
        if(tokenbuf_)
            delete tokenbuf_;

        tokenbuf_ = nullptr;
    }

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

    void CreateTokens()
    {
        tokenbuf_ = new(std::nothrow) std::vector<std::string>; int line = __LINE__;
        if(!tokenbuf_)
        {
            std::cout << '\n' << "Can't allocate mem in function: " << __PRETTY_FUNCTION__ << " line:" << line;
            return;
        }

        std::string token;
        int i = 0;

        while(i < buf_.size())
        {
            while (((buf_[i] == ' ') || (buf_[i] == '\n')) && (i < buf_.size()))
                i++;

            while((buf_[i] != ' ') && (buf_[i] != '\n') && (buf_[i] != ';') && (i < buf_.size()))
            {
                token.push_back(buf_[i]);
                i++;
            }

            if(token.empty() == 0)
            {
                tokenbuf_ -> push_back(token);
                token.erase();
            }

            if((buf_[i] == ';') && (i < buf_.size()))
            {
                while(buf_[i] != '\n')
                    i++;

                i++;
            }
        }
    }

};

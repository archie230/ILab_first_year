#pragma once

#define CATCH catch(std::exception& exc)\
                {\
                    std::cout<< exc.what() << "in function: " << __PRETTY_FUNCTION__ \
                                                                            << "line: " << __LINE__ << std::endl;\
                    return ERRS::ERRCODE;\
                }

#define FUNCMAP my_maps_.FuncMap
#define REGMAP my_maps_.RegMap
#define LBLMAP my_maps_.LblMap
#define FOOMAP my_maps_.AsmFuncMap
#define DO(funcname) if(funcname() == ERRS::ERRCODE) return ERRS::ERRCODE;

//===========================================================================//
//=================================FUNCTIONS=================================//
//===========================================================================//

assembly::assembly() {
    my_maps_.filling();
}

int assembly::assembly_code() {
    DO(ReadAsmCode)
    DO(CreateTokens)
    DO(scan_funcs_and_lbls)
    DO(filltransbuf)
    DO(checklogic)
    DO(create_exe_file)

    return 0;
}
#undef DO

int assembly::ReadAsmCode() {
    FILE *file = fopen("ASM.txt", "r"); int line = __LINE__;

    if (!file)
    {
        std::cout << '\n' << "Can't open file ASM.txt in function: " << __PRETTY_FUNCTION__ << " line:" << line;
        return ERRS::ERRCODE;
    }

    fseek(file, 0, SEEK_END);
    int strsize = ftell(file);
    rewind(file);

    buf_.resize(strsize, '\n');
    fread(&buf_[0], strsize, sizeof(char), file);

    fclose(file);

    return 0;
}

int assembly::CreateTokens() {
    token tkn;
    int i = 0;
    int linenum = 1;

    while (i < buf_.size()) {
        while (((buf_[i] == ' ') || (buf_[i] == '\n')) && (i < buf_.size()))
        {
            if(buf_[i] == '\n')
                ++linenum;
            ++i;
        }

        while ((buf_[i] != ' ') && (buf_[i] != '\n') && (buf_[i] != ';') && (i < buf_.size()))
        {
            tkn.str_.push_back(buf_[i]);
            ++i;
        }

        if (!tkn.str_.empty()) {
            tkn.line_ = linenum;

            try {
                tokenbuf_.push_back(tkn);
            }
            CATCH

            tkn.clear();
        }

        if ((buf_[i] == ';') && (i < buf_.size())) {
            while (buf_[i] != '\n')
                ++i;

            ++linenum;
            ++i;
        }
    }

    return 0;
}

int assembly::scan_funcs_and_lbls() {
    int funcnum = 0;
    int lblsnum = 0;
    int ret_or_end_num = 0;

    std::string lable;
    int main_adr = findMAIN();

    for (int ip = 0; ip < tokenbuf_.size(); ++ip) {

        if(tokenbuf_[ip].str_ == FOO)
        {
            if((FUNCMAP.find(tokenbuf_[ip+1].str_) != FUNCMAP.end()) || (REGMAP.find(tokenbuf_[ip+1].str_) != REGMAP.end())) {
                std::cout << SYNT << "Can't name function " << tokenbuf_[ip+1].str_ << " line:" << tokenbuf_[ip+1].line_ << std::endl;
                return ERRS::ERRCODE;
            }

            if((ip + 1 > main_adr) && (main_adr != 0))
            {
                std::cout << "met function declaration after start point (main)" << std::endl;
                return ERRS::ERRCODE;
            }


            if(FOOMAP.find(tokenbuf_[ip+1].str_) != FOOMAP.end())
            {
                std::cout << "met redefenition of function: " << tokenbuf_[ip+1].str_ << " line:" << tokenbuf_[ip+1].line_ << std::endl;
                return ERRS::ERRCODE;
            }


            if(tokenbuf_[ip+1].str_[0] == ':')
            {
                std::cout << "can't use lable to name function\nline:" << tokenbuf_[ip+1].line_ << std::endl;
                return ERRS::ERRCODE;
            }


            if(!FOOMAP.emplace(tokenbuf_[ip+1].str_, ip - 2*funcnum - lblsnum).second) {
                std::cout << "Can't emplace in AsmFuncMap: func " << __PRETTY_FUNCTION__ << ", line " << __LINE__ << std::endl;
                return ERRS::ERRCODE;
            }

            ++ip;
            if(!findRET(ip))
                return ERRS::ERRCODE;

            funcnum++;
        }


        if (tokenbuf_[ip].str_[0] == ':') {

            lable.resize(tokenbuf_[ip].str_.size() - 1);

            for (int i = 0; i < lable.size(); ++i) {
                lable[i] = tokenbuf_[ip].str_[i + 1];
            }

            if(LBLMAP.find(lable) != LBLMAP.end()) {
                std::cout << SYNT << "met lable :" << tokenbuf_[ip].str_ << " more tham 1 time line:" << tokenbuf_[ip].line_ << std::endl;
                return ERRS::ERRCODE;
            }

            if(REGMAP.find(lable) != REGMAP.end() || FUNCMAP.find(lable) != FUNCMAP.end()) {
                std::cout << SYNT << "can't name lable:" << std::endl;
                tokenbuf_[ip].err_dump();
                return ERRS::ERRCODE;
            }

            if (!LBLMAP.emplace(lable, ip - 2*funcnum - lblsnum).second) {
                std::cout << "Can't emplace in LblMap: func " << __PRETTY_FUNCTION__ << ", line " << __LINE__;
                return ERRS::ERRCODE;
            }

            lable.erase();

            ++lblsnum;
        }
    }

    return 0;
}

int assembly::filltransbuf() {

    std::map<std::string, int>::iterator FuncIter, RegIter, LblIter, AsmFuncIter;
    int* ptr = nullptr;

    for (int ip = 0; ip < tokenbuf_.size(); ++ip) {

        FuncIter    = FUNCMAP.find(tokenbuf_[ip].str_);
        RegIter     = REGMAP.find(tokenbuf_[ip].str_);
        LblIter     = LBLMAP.find(tokenbuf_[ip].str_);
        AsmFuncIter = FOOMAP.find(tokenbuf_[ip].str_);

        if(tokenbuf_[ip].str_ == FOO)
            continue;

        if (FuncIter != FUNCMAP.end()) {

            if(transbuf_insert(FUNCTN, FuncIter -> second, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                return ERRS::ERRCODE;
            continue;

        }

        if (RegIter != REGMAP.end()) {

            if(transbuf_insert(REGSTR, RegIter -> second, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                return ERRS::ERRCODE;
            continue;

        }

        if (LblIter != LBLMAP.end()) {

            if(transbuf_insert(LBL, LblIter -> second, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                return ERRS::ERRCODE;
            continue;

        }

        if (AsmFuncIter != FOOMAP.end()) {

            if(tokenbuf_[ip - 1].str_ == FOO)
                continue;

            else {
                if (transbuf_insert(ASM_FUNC, AsmFuncIter->second, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                    return ERRS::ERRCODE;
                continue;
            }
        }

        if (check_num(tokenbuf_[ip].str_)) {

            if(transbuf_insert(NMBR, string_to_int(tokenbuf_[ip].str_), tokenbuf_[ip].line_) == ERRS::ERRCODE)
                return ERRS::ERRCODE;
            continue;

        }

        ptr = check_RAM_NUM(tokenbuf_[ip].str_);
        if (ptr) {

            if(transbuf_insert(RAM_NUM, *ptr, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                return ERRS::ERRCODE;

            delete ptr;
            ptr = nullptr;
            continue;

        }


        ptr = check_RAM_REG(tokenbuf_[ip].str_);
        if (ptr) {

            if(transbuf_insert(RAM_REG, *ptr, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                return ERRS::ERRCODE;

            delete ptr;
            ptr = nullptr;
            continue;

        }


        ptr = check_RAM_OPERATOR_NUM(tokenbuf_[ip].str_, '+');
        if(ptr) {

            if(transbuf_insert(RAM_A_NUM, *ptr, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                return ERRS::ERRCODE;

            delete ptr;
            ptr = nullptr;
            continue;

        }

        ptr = check_RAM_OPERATOR_NUM(tokenbuf_[ip].str_, '-');
        if(ptr) {

            if(transbuf_insert(RAM_S_NUM, *ptr, tokenbuf_[ip].line_) == ERRS::ERRCODE)
                return ERRS::ERRCODE;

            delete ptr;
            ptr = nullptr;
            continue;

        }

        if(check_lbl(tokenbuf_[ip].str_))
            continue;

        tokenbuf_[ip].err_dump();
        return ERRS::ERRCODE;
    }

    return 0;
}

#define CHECKER(checkingfunc) if(checkingfunc(ip) == false) {\
                                    transbuf_[ip].err_dump();\
                                    return ERRS::ERRCODE; }\
                                else {\
                                    ip++;\
                                    continue; }

int assembly::checklogic() {
    int main_flag = 0;
    int end_flag = 0;

    for(int ip = 0; ip < transbuf_.size(); ++ip)
    {
        switch(transbuf_[ip].tokentype_)
        {
            case(FUNCTN):
            {
                switch(transbuf_[ip].code_)
                {
                    case(FUNC::PUSH_NUM):
                    {
                        CHECKER(inspectPUSH)
                    }

                    case(FUNC::POP_DEF):
                    {
                        CHECKER(inspectPOP)
                    }

                    case(FUNC::JMP):
                    {
                        CHECKER(inspectJUMP)
                    }

                    case(FUNC::JE):
                    {
                        CHECKER(inspectJUMP)
                    }

                    case(FUNC::JL):
                    {
                        CHECKER(inspectJUMP)
                    }

                    case(FUNC::JB):
                    {
                        CHECKER(inspectJUMP)
                    }

                    case(FUNC::CALL):
                    {
                        CHECKER(inspectCALL)
                    }

                    case(FUNC::MAIN):
                    {
                        main_flag++;
                        continue;
                    }

                    case(FUNC::END):
                    {
                        if(main_flag == 0)
                        {
                            std::cout << "met end point (end) before main" << std::endl;
                            return ERRS::ERRCODE;
                        }

                        end_flag++;
                        continue;
                    }

                    default:
                    {
                        continue;
                    }
                }

                break;
            }

            default:
            {
                transbuf_[ip].err_dump();
                return ERRS::ERRCODE;
            }

        }
    }

    if(main_flag == 0)
    {
        std::cout << "no start point (main)" << std::endl;
        return ERRS::ERRCODE;
    }

    if(main_flag > 1)
    {
        std::cout << "met start point (main) many times" << std::endl;
        return ERRS::ERRCODE;
    }

    if(end_flag == 0)
    {
        std::cout << "forgot end point (end)" << std::endl;
        return ERRS::ERRCODE;
    }

    if(end_flag > 1)
    {
        std::cout << "met end point (end) many times" << std::endl;
        return ERRS::ERRCODE;
    }
    return 0;
}
#undef CHECKER

int assembly::create_exe_file() {
    FILE* notexe = fopen(OUTPUT, "wb"); int line = __LINE__;
    if(!notexe)
    {
        std::cout << "can't open" << OUTPUT << std::endl << __PRETTY_FUNCTION__ << "  line: " << line << std::endl;
        return ERRS::ERRCODE;
    }

    int codesize = transbuf_.size();
    fwrite(&codesize, sizeof(int), 1, notexe);
    fputc(' ', notexe);

    for(int ip = 0; ip < transbuf_.size(); ip++)
    {
        fwrite(&transbuf_[ip].code_, sizeof(int), 1, notexe);
        fputc(' ', notexe);
    }

    write_lbl_or_funcs(notexe, LBLMAP);
    write_lbl_or_funcs(notexe, FOOMAP);


    fclose(notexe);
    return 0;
}

void assembly::write_lbl_or_funcs(FILE *exe_name, std::map<std::string, int> &map) {
    int name_size = 0;
    int map_size = map.size();

    fputc('|', exe_name);
    fwrite(&map_size, sizeof(int), 1, exe_name);

    for(auto it = map.begin(); it != map.end(); it++)
    {
        name_size = it -> first.size();
        fputc('[', exe_name);
        fwrite(&name_size, sizeof(int), 1, exe_name);
        fwrite(&(it -> first)[0], sizeof(char), name_size, exe_name);
        fwrite(&(it -> second), sizeof(int), 1, exe_name);
        fputc(']', exe_name);
    }

    fputc('|', exe_name);
}

bool assembly::findRET(int index) {
    assert(index >= 0);

    while((tokenbuf_[index].str_ != "main") && (index < tokenbuf_.size()))
    {
        if(tokenbuf_[index].str_ == FOO) {
            std::cout << SYNT << "met func declaration without ret before ";
            tokenbuf_[index].err_dump();
            return false;
        }

        if(tokenbuf_[index].str_ == "ret")
            return true;

        index++;
    }


    std::cout << "forgot ret at line: " << tokenbuf_[--index].line_ + 1 << std::endl;

    return false;
}

int assembly::findMAIN() {
    for(int ip = 0; ip < tokenbuf_.size(); ip++)
    {
        if(tokenbuf_[ip].str_ == "main")
            return ip;
    }

    return 0;
}

int assembly::transbuf_insert(char transtkntype, int code, int line) {
    assert(line >= 1);

    transtoken transtkn(transtkntype, code, line);
    try {
        transbuf_.push_back(transtkn);
    }

    CATCH

    return 0;
}

bool assembly::check_num(std::string &tkn_str) {
    try {
        int number = string_to_int(tkn_str);
    }

    catch(std::logic_error& err)
    {
        return false;
    }

    return true;
}

bool assembly::check_lbl(std::string &tkn_str) {
    if(tkn_str[0] != ':')
        return false;

    std::string lbl(tkn_str.size() - 1, '\0');
    for(int i = 0; i < lbl.size(); i++)
    {
        lbl[i] = tkn_str[i+1];
    }

    auto nemo = LBLMAP.find(lbl);
    return !(nemo == LBLMAP.end());
}

int *assembly::check_RAM_NUM(const std::string &tkn_str) {
    if((tkn_str[0] != '[') || (tkn_str[tkn_str.size() - 1] != ']')) {
        return nullptr;
    }

    std::string inner(tkn_str.size() - 2 , '\0');

    for(int i = 0; i < inner.size(); i++) {
        inner[i] = tkn_str[i+1];
    }

    int numbr = 0;

    try {
        numbr = string_to_int(inner);
    }

    catch(std::logic_error& err) {
        return nullptr;
    }

    int* RAM_NUM_ptr = new(std::nothrow) int;
    *RAM_NUM_ptr = numbr;

    return RAM_NUM_ptr;
}

int *assembly::check_RAM_REG(const std::string &tkn_str) {
    if((tkn_str[0] != '[') || (tkn_str[tkn_str.size() - 1] != ']')) {
        return nullptr;
    }

    std::string inner(tkn_str.size() - 2 , '\0');
    for(int i = 0; i < inner.size(); i++) {
        inner[i] = tkn_str[i+1];
    }


    std::map<std::string, int>::iterator nemo = REGMAP.find(inner);
    if(nemo == REGMAP.end())
        return nullptr;

    int* RAM_REG_ptr = new(std::nothrow) int;
    *RAM_REG_ptr = nemo -> second;
    return RAM_REG_ptr;
}

int *assembly::check_RAM_OPERATOR_NUM(const std::string &tkn_str, char opertr) {
    if((tkn_str[0] != '[') || (tkn_str[tkn_str.size() - 1] != ']'))
        return nullptr;

    std::string reg_str(2, '\0');
    for(int i = 0; i < 2; i++)
    {
        reg_str[i] = tkn_str[i+1];
    }


    std::map<std::string, int>::iterator RegIter = REGMAP.find(reg_str);


    switch(opertr){
        case('+'): {
            if((RegIter == REGMAP.end()) || tkn_str[3] != '+')
                return nullptr;
            break;
        }

        case('-'): {
            if((RegIter == REGMAP.end()) || tkn_str[3] != '-')
                return nullptr;
            break;
        }

        default: {
            std::cout << "unexpected operator " << opertr << " in function: " << __PRETTY_FUNCTION__;
            return nullptr;
        }
    }

    std::string num_str(tkn_str.size() - 5, '\0');
    for(int i = 0; i < num_str.size(); i++)
    {
        num_str[i] = tkn_str[i+4];
    }

    int numbr = 0;
    try {
        numbr = string_to_int(num_str);
    }
    catch(std::logic_error& err)
    {
        return nullptr;
    }

    int* RAM_OPERATOR_NUM_ptr = new(std::nothrow) int;

    short* reg = (short*) RAM_OPERATOR_NUM_ptr;
    short* num = reg + 1;

    *reg = RegIter -> second;
    *num = numbr;

    return RAM_OPERATOR_NUM_ptr;
}

bool assembly::inspectPUSH(int ip) {
    switch(transbuf_[ip+1].tokentype_)
    {
        case(NMBR): {
            return true;
        }

        case(REGSTR):
        {
            transbuf_[ip].code_ = FUNC::PUSH_REG;
            return true;
        }

        case(RAM_NUM):
        {
            transbuf_[ip].code_ = FUNC::PUSH_RAM_NUM;
            return true;
        }

        case(RAM_REG):
        {
            transbuf_[ip].code_ = FUNC::PUSH_RAM_REG;
            return true;
        }

        case(RAM_A_NUM):
        {
            transbuf_[ip].code_ = FUNC::PUSH_RAM_REG_A_NUM;
            return true;
        }

        case(RAM_S_NUM):
        {
            transbuf_[ip].code_ = FUNC::PUSH_RAM_REG_S_NUM;
            return true;
        }

        default: {
            return false;
        }
    }
}

bool assembly::inspectPOP(int ip) {
    switch(transbuf_[ip+1].tokentype_)
    {
        case(NMBR): {
            return true;
        }

        case(REGSTR):
        {
            transbuf_[ip].code_ = FUNC::POP_REG;
            return true;
        }

        case(RAM_NUM):
        {
            transbuf_[ip].code_ = FUNC::POP_RAM_NUM;
            return true;
        }

        case(RAM_REG):
        {
            transbuf_[ip].code_ = FUNC::POP_RAM_REG;
            return true;
        }

        case(RAM_A_NUM):
        {
            transbuf_[ip].code_ = FUNC::POP_RAM_REG_A_NUM;
            return true;
        }

        case(RAM_S_NUM):
        {
            transbuf_[ip].code_ = FUNC::POP_RAM_REG_S_NUM;
            return true;
        }

        default: {
            return false;
        }
    }
}

bool assembly::inspectJUMP(int ip) {
    return transbuf_[ip + 1].tokentype_ == LBL ? true : false;
}

bool assembly::inspectCALL(int ip) {
    return transbuf_[ip + 1].tokentype_ == ASM_FUNC ? true : false;
}

#undef FUNCMAP
#undef REGMAP
#undef LBLMAP
#undef CATCH

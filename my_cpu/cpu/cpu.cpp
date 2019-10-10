#include "cpu.h"

bool cpu::Stack_empty() {
    return stack_size_ == 0;
}

int cpu::Stack_OK() {
    if(stack_size_ < 0 || stack_capacity_ < 0)
    {
        printf("\nSTACKERR size: %d\n capacity: %d\n", stack_size_, stack_capacity_);
        return ERRCODE;
    }

    if(stack_size_ > stack_capacity_)
    {
        printf("\nSTACKERR stack overflow size: %d\n capacity: %d\n", stack_size_, stack_capacity_);
        return ERRCODE;
    }

    if(REG_EB < BASE_EB)
    {
        printf("\nSTACKERR base pointer less than 511");
        return ERRCODE;
    }

    if(REG_ES < REG_EB)
    {
        printf("\nSTACKERR stack pointer less than base pointer\n");
        return ERRCODE;
    }

    return 0;
}

int cpu::push(int number) {
    if(Stack_OK() == ERRCODE)
        return ERRCODE;

    if(stack_size_ !=0)
        REG_ES++;

    ram_[REG_ES] = number;
    stack_size_++;
    return 0;
}

int cpu::pop() {
    if(Stack_OK() == ERRCODE)
        return ERRCODE;

    if(Stack_empty())
    {
        printf("\nSTACKERR pop from empty stack\n");
        return ERRCODE;
    }
    stack_size_--;

    if(REG_ES != REG_EB) {
        REG_ES--;
        return ram_[REG_ES + 1];
    }

    else {
        return ram_[REG_ES];
    }
}

int cpu::mul() {
    if(Stack_OK() == ERRCODE)
        return ERRCODE;

    if(stack_size_ < 2) {
        printf("\nSTACKERR size less than 2 in mul\n");
        return ERRCODE;
    }

    int multi1 = pop();
    int multi2 = pop();
    push(multi1*multi2);
    return 0;
}

int cpu::div() {
    if(Stack_OK() == ERRCODE)
        return ERRCODE;

    if(stack_size_ < 2) {
        printf("\nSTACKERR size less than 2 in mul function\n");
        return ERRCODE;
    }

    int multi1 = pop();
    int multi2 = pop();
    push(multi2/multi1);
    return 0;
}

int cpu::add() {
    if(Stack_OK() == ERRCODE)
        return ERRCODE;

    if(stack_size_ < 2) {
        printf("\nSTACKERR size less than 2 in add function\n");
        return ERRCODE;
    }

    int add1 = pop();
    int add2 = pop();
    push(add1+add2);
    return 0;
}

int cpu::sub() {
    if(Stack_OK() == ERRCODE)
        return ERRCODE;

    if(stack_size_ < 2) {
        printf("\nSTACKERR size less than 2 in add function\n");
        return ERRCODE;
    }

    int add1 = pop();
    int add2 = pop();
    push(add2-add1);
    return 0;
}

int cpu::Stack_getsize() {
    return stack_size_;
}

int cpu::Stack_getcapacity() {
    return stack_capacity_;
}

cpu::cpu(const char* exename)
        :
        ram_(),
        registers_(),
        commands_buf_(),
        start_point_(-1),
        exename_(exename),
        stack_size_(0),
        stack_capacity_(BASE_STACK_CAP)
{
    REG_EB = BASE_EB;
    REG_ES = REG_EB;
}

#define DO(func) if(func() == ERRCODE) return;
void cpu::do_cpu() {
    DO(load_code)
    DO(find_startpoint)
    DO(do_commands)
}
#undef DO

int cpu::load_code() {
    FILE *code = fopen(exename_.c_str(), "rb");
    if (!code) {
        std::cout << "can't open file:" << exename_ << std::endl;
        return ERRCODE;
    }

    int codesize = 0;
    if(codesize < 0) {
        std::cout << "bad .notexe file:" << exename_ << std::endl;
        return ERRCODE;
    }
    fread(&codesize, sizeof(int), 1, code);
    fgetc(code);

    commands_buf_.resize(codesize);
    for (int i = 0; i < codesize; i++) {
        fread(&commands_buf_[i], sizeof(int), 1, code);
        fgetc(code);
    }

    fclose(code);
    return 0;
}

int cpu::find_startpoint()
{
    if(commands_buf_[0] == 25) {
        start_point_ = 0;
        return 0;
    }

    for(int i = 0; i < commands_buf_.size(); i++)
    {
        if((i != 0) && (commands_buf_[i] == 25) && (commands_buf_[i-1] == 24)) {
            start_point_ = i;
            return 0;
        }
    }

    std::cout << "bad .notexe file:" << exename_ << std::endl;
    return ERRCODE;
}

int cpu::do_commands() {

#define DEF_CMD(command, command_code, str, instruction) case(command_code) : instruction

    assert(start_point_ >= 0);
    short* reg = nullptr;
    short* num = nullptr;
    int input = 0;
    int first_comp = 0;
    int second_comp = 0;

    for(int ip = start_point_ + 1; ip < commands_buf_.size(); ip++) {
        switch(commands_buf_[ip]) {
#include "../lib/DEF_CMD.h"

            default: {
                //printf("\nmeow\n");
                return ERRCODE;
            }
        }
    }

#undef DEF_CMD

    return 0;
}

int cpu::create_frame(int ret_adr) {
    if(push(REG_EB) == ERRCODE)
        return ERRCODE;

    if(push(ret_adr) == ERRCODE) {
        return ERRCODE;
    }


    REG_EB = REG_ES - 1;
    stack_capacity_ = RAM_SIZE - REG_EB - 1;
    stack_size_ = 2;

    return 0;
}

int cpu::delete_frame(int& ret_adr) {

    if(REG_ES != REG_EB + 1) {
        printf("\nIN STACK FRAME stack pointer and base pointer dosen't match\n");
        return ERRCODE;
    }

    if(stack_size_ < 2) {
        printf("\nBAD STACK FRAME stack size: %d\n", stack_size_);

        return ERRCODE;
    }

    ret_adr = pop();
    REG_EB = pop();
    assert(REG_EB >= 511);

    if(REG_ES > REG_EB) {
        REG_ES--;
        stack_size_ = REG_ES - REG_EB + 1;
    }

    else {
        stack_size_ = REG_ES - REG_EB;
    }

    stack_capacity_ = RAM_SIZE - REG_EB - 1;
    return 0;
}

void cpu::dump() {
    std::cout << "\n-------------" << "\nEB: " << REG_EB << "\nES: " << REG_ES << "\nsize:"\
                                                            << stack_size_ << "\ncapacity:" << stack_capacity_ << std::endl;
}

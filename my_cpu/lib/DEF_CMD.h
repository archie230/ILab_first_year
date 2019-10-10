//================================================================================================================
DEF_CMD(PUSH_NUM, 1, "push",
{
push(commands_buf_[ip + 1]);
ip++; continue;
})
//=================================================================================================================
DEF_CMD(PUSH_REG, 2, "push",
{
assert(commands_buf_[ip + 1] >= REG::AX && commands_buf_[ip + 1] <= REG::ES);
push(registers_[commands_buf_[ip + 1] - REG::AX]);
ip++; continue;
})
//=================================================================================================================
DEF_CMD(PUSH_RAM_NUM, 3, "push",
{
assert(commands_buf_[ip + 1] >= 0 && commands_buf_[ip + 1] <= RAM_SIZE);
push(ram_[commands_buf_[ip+1]]);
ip++; continue;
})
//=================================================================================================================
DEF_CMD(PUSH_RAM_REG, 4, "push",
{
assert(commands_buf_[ip + 1] >= REG::AX && commands_buf_[ip + 1] <= REG::ES);
assert(registers_[commands_buf_[ip + 1] - REG::AX] >= 0 &&\
                                                        registers_[commands_buf_[ip + 1] - REG::AX] <= RAM_SIZE);
push(ram_[registers_[commands_buf_[ip + 1] - REG::AX]]);
ip++; continue;
})
//=================================================================================================================
DEF_CMD(PUSH_RAM_REG_A_NUM, 5, "push",
{
reg = (short*) &commands_buf_[ip + 1];
num = reg + 1;
assert(*reg >= REG::AX && *reg <= REG::ES);
assert(registers_[*reg - REG::AX] + *num >= 0  && registers_[*reg - REG::AX] + *num <= RAM_SIZE);
push(ram_[registers_[*reg - REG::AX] + *num]);
reg = num = nullptr;
ip++; continue;
})
//=================================================================================================================
DEF_CMD(PUSH_RAM_REG_S_NUM, 6, "push",
{
reg = (short*) &commands_buf_[ip + 1];
num = reg + 1;
assert(*reg >= REG::AX && *reg <= REG::ES);
assert(registers_[*reg - REG::AX] - *num >= 0 && registers_[*reg - REG::AX] - *num <= RAM_SIZE);
push(ram_[registers_[*reg - REG::AX] - *num]);
reg = num = nullptr;
ip++; continue;
})
//==================================================================================================================
DEF_CMD(POP_DEF, 7, "pop",
{
pop();
continue;
})
//===================================================================================================================
DEF_CMD(POP_REG, 8, "pop",
{
assert(commands_buf_[ip + 1] >= REG::AX && commands_buf_[ip + 1] <= REG::ES);
registers_[commands_buf_[ip + 1] - REG::AX] = pop();
ip++; continue;
})
//===================================================================================================================
DEF_CMD(POP_RAM_NUM, 9, "pop",
{
assert(commands_buf_[ip + 1] >= 0 && commands_buf_[ip + 1] <= RAM_SIZE);
ram_[commands_buf_[ip + 1]] = pop();
ip++; continue;
})
//===================================================================================================================
DEF_CMD(POP_RAM_REG, 10, "pop",
{
assert(commands_buf_[ip + 1] >= REG::AX && commands_buf_[ip + 1] <= REG::ES);
assert(registers_[commands_buf_[ip + 1] - REG::AX] >= 0 && registers_[commands_buf_[ip + 1] - REG::AX] <= RAM_SIZE);
ram_[registers_[commands_buf_[ip + 1] - REG::AX]] = pop();
ip++; continue;
})
//===================================================================================================================
DEF_CMD(POP_RAM_REG_A_NUM, 11, "pop",
{
reg = (short*) &commands_buf_[ip + 1];
num = reg + 1;
assert(*reg >= REG::AX && *reg <= REG::ES);
assert(registers_[*reg - REG::AX] + *num >= 0  && registers_[*reg - REG::AX] + *num <= RAM_SIZE);
ram_[registers_[*reg - REG::AX] + *num] = pop();
ip++; continue;
})
//===================================================================================================================
DEF_CMD(POP_RAM_REG_S_NUM, 12, "pop", break;
{
reg = (short*) &commands_buf_[ip + 1];
num = reg + 1;
assert(*reg >= REG::AX && *reg <= REG::ES);
assert(registers_[*reg - REG::AX] - *num >= 0  && registers_[*reg - REG::AX] - *num <= RAM_SIZE);
ram_[registers_[*reg - REG::AX] - *num] = pop();
ip++; continue;
})
//===================================================================================================================
DEF_CMD(OUT, 13, "out",
{
printf("\nOUT: %d\n", REG_OX);
continue;
})
//===================================================================================================================
DEF_CMD(MUL, 14, "mul",
{
mul();
continue;
})
//===================================================================================================================
DEF_CMD(DIV, 15, "div",
{
div();
continue;
})
//===================================================================================================================
DEF_CMD(ADD, 16, "add",
{
add();
continue;
})
//===================================================================================================================
DEF_CMD(SUB, 17, "sub",
{
sub();
continue;
})
//===================================================================================================================
DEF_CMD(JMP, 18, "jmp",
{
assert(commands_buf_[ip + 1] >= 0 && commands_buf_[ip + 1] <= commands_buf_.size());
ip = commands_buf_[ip + 1] - 1;
continue;
})
//===================================================================================================================
DEF_CMD(JE, 19, "je",
{

if((stack_size_ >= 2)) {
first_comp = pop();
second_comp = pop();
if(first_comp == second_comp) {
assert(commands_buf_[ip + 1] >= 0 && commands_buf_[ip + 1] <= commands_buf_.size());
ip = commands_buf_[ip + 1] - 1;
continue;
}
}

ip++; continue;
})
//===================================================================================================================
DEF_CMD(JA, 20, "ja",
{
if((stack_size_ >= 2)) {
first_comp = pop();
second_comp = pop();
if(second_comp > first_comp) {
assert(commands_buf_[ip + 1] >= 0 && commands_buf_[ip + 1] <= commands_buf_.size());
ip = commands_buf_[ip + 1] - 1;
continue;
}
}

ip++; continue;
})
//===================================================================================================================
DEF_CMD(JB, 21, "jb",
{
if((stack_size_ >= 2)) {
first_comp = pop();
second_comp = pop();
if(second_comp < first_comp) {
assert(commands_buf_[ip + 1] >= 0 && commands_buf_[ip + 1] <= commands_buf_.size());
ip = commands_buf_[ip + 1] - 1;
continue;
}
}

ip++; continue;
})
//===================================================================================================================
DEF_CMD(END, 22, "end",
{
return 0;
})
//===================================================================================================================
DEF_CMD(CALL, 23, "call",
{
assert(commands_buf_[ip + 1] >= 0 && commands_buf_[ip + 1] <= commands_buf_.size());
if(create_frame(ip + 1) == ERRCODE)
return ERRCODE;
ip = commands_buf_[ip+1] - 1;
continue;
})
//===================================================================================================================
DEF_CMD(RET, 24, "ret",
{
if(delete_frame(ip) == ERRCODE) {
return ERRCODE;
}

continue;
})
//===================================================================================================================
DEF_CMD(MAIN, 25, "main",
{
continue;
})
//===================================================================================================================
DEF_CMD(INT, 26, "int",
{
printf("\nINPUT: ");
scanf("%d", &input);
push(input);
continue;
})
//===================================================================================================================
DEF_CMD(SQRT, 27, "sqrt",
{
push(sqrt(pop()));
continue;
})
//===================================================================================================================

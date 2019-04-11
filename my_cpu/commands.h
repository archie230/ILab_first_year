#pragma once

#ifndef REGISTERS
CMD("push", 1)
CMD("pop", 2)
CMD("out", 3)
CMD("mul", 4)
CMD("div", 5)
CMD("add", 6)
CMD("sub", 7)
CMD("jmp", 8)
CMD("je", 9)
CMD("jl", 10)
CMD("jb", 11)
CMD("end", 12)
CMD("mov", 13)
#endif

#ifndef COMMANDS
REG("ax", 14)
REG("bx", 15)
REG("cx", 16)
REG("dx", 17)
#endif

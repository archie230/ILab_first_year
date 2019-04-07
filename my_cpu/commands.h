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
CMD("end", 9)
#endif

#ifndef COMMANDS
REG("ax", 10)
REG("bx", 11)
REG("cx", 12)
REG("dx", 13)
#endif
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <array>
#include "intel8080.h"
#include "virtualMemory.h"

Intel8080::Intel8080()
{
    // Initialize flags
    flags.z = 0;
    flags.s = 0;
    flags.p = 0;
    flags.cy = 0;
    flags.ac = 0;
    flags.pad = 0;

    // Initialize registers
    regs.a = 0;
    regs.b = 0;
    regs.c = 0;
    regs.d = 0;
    regs.e = 0;
    regs.h = 0;
    regs.l = 0;
    regs.sp = 0;
    regs.pc = 0;

    // Initialize opcode lookup table with illegal instruction handler
    p_opcode_lookup.fill(&Intel8080::op_ILLEGAL);
    p_opcode_lookup[0x00] = &Intel8080::op_NOP; // NOP instruction

}

void Intel8080::op_ILLEGAL()
{
    printf("ILLEGAL\n");
    exit(-1);
}

void Intel8080::op_NOP()
{
    // Opcode: 0x00         Mnemonic: NOP
    // Mnemonic: NOP
    // Size: 1
    // Cycles: 4
    // Description: No Operation
    // Flags: None
    printf("NOP\n");
}   

void Intel8080::test()
{
    // Call the function pointed to by the opcode's table entry
    (this->*p_opcode_lookup[0])();
    (this->*p_opcode_lookup[1])();
}

#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <array>
#include "intel8080.h"
#include "virtualMemory.h"
#include "bus.h"

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

    bus = nullptr;

}

Intel8080 *Intel8080::attachBus(Bus *bus)
{
    this->bus = bus;
    return this;
}

BYTE Intel8080::fetch()
{
    return bus->read(regs.pc++);
}

BYTE Intel8080::read(WORD address)
{
    return bus->read(address);
}

void Intel8080::execute(BYTE opcode)
{
    (this->*p_opcode_lookup[opcode])();
}

void Intel8080::op_ILLEGAL()
{
    printf("ILLEGAL\n");
    throw std::runtime_error("Illegal opcode executed");
}

void Intel8080::op_NOP()
{
    // Opcode: 0x00         Mnemonic: NOP
    // Size: 1  byte        Cycles: 4
    // Description: No Operation
    // Flags: None
    printf("NOP\n");
}   

void Intel8080::test()
{
    // Call the function pointed to by the opcode's table entry
    BYTE opcode = fetch();
    execute(opcode);
    opcode = fetch();
    try {
        execute(opcode);
    } catch (const std::runtime_error &e) {
        printf("Caught expected exception: %s\n", e.what());
    }
    try {
        read(0xFFFF); // Attempt to read from unmapped memory

    } catch (const std::runtime_error &e) {
        printf("Caught expected exception: %s\n", e.what());
    }   
}

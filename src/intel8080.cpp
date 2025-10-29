#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <array>
#include <spdlog/spdlog.h>
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

    buildOpcodeTable();
    bus = nullptr;

}

Intel8080 *Intel8080::attachBus(Bus *bus)
{
    this->bus = bus;
    return this;
}

BYTE Intel8080::fetchOpcode()
{
    return bus->readByte(regs.pc++);
}

BYTE Intel8080::fetchByte()
{
    return bus->readByte(regs.pc++);
}

WORD Intel8080::fetchWord()
{
    BYTE low = bus->readByte(regs.pc++);
    BYTE high = bus->readByte(regs.pc++);
    return (static_cast<WORD>(high) << 8) | static_cast<WORD>(low);
}

BYTE Intel8080::readOpcode(WORD address)
{
    return bus->readByte(address);
}

BYTE Intel8080::readByte(WORD address)
{
    return bus->readByte(address);
}

WORD Intel8080::readWord(WORD address)
{
    return bus->readWord(address);
}

void Intel8080::writeOpcode(WORD address, BYTE data)
{
    bus->writeByte(address, data);
} 

void Intel8080::writeByte(WORD address, BYTE data)
{
    bus->writeByte(address, data);
}

void Intel8080::writeWord(WORD address, WORD data)
{
    bus->writeWord(address, data);
}

void Intel8080::execute(BYTE opcode)
{
    (this->*p_opcode_lookup[opcode])();
}

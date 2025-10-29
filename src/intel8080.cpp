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
    flags.s = 0;
    flags.z = 0;
    flags.x_zero = 0;
    flags.ac = 0;
    flags.y_zero = 0;
    flags.p = 0;
    flags.x_one = 1;
    flags.cy = 0;

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

void Intel8080::fetchOpcode()
{
    opcode = bus->readByte(regs.pc++);
    spdlog::debug("Fetched opcode: 0x{:02X}", opcode);
}

void Intel8080::fetchByte()
{
    byteData = bus->readByte(regs.pc++);
    spdlog::debug("Fetched byte: 0x{:02X}", byteData);
}

void Intel8080::fetchWord()
{
    wordData = (static_cast<WORD>(bus->readByte(regs.pc)) | static_cast<WORD>(bus->readByte(regs.pc+1) << 8));
    spdlog::debug("Fetched word: 0x{:04X}", wordData);
    regs.pc += 2;
}

void Intel8080::readOpcode(WORD address)
{
    opcode = bus->readByte(address);
}

void Intel8080::readByte(WORD address)
{
    byteData = bus->readByte(address);
}

void Intel8080::readWord(WORD address)
{
    wordData = bus->readWord(address);
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

void Intel8080::execute()
{
    (this->*p_opcode_lookup[opcode])();
}



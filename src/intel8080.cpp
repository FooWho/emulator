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
    reset();
    buildOpcodeTable();
    bus = nullptr;
}
void Intel8080::reset()
{
    // Reset flags to their initial state.
    flags.s = 0;
    flags.z = 0;
    flags.x_zero = 0;
    flags.ac = 0;
    flags.y_zero = 0;
    flags.p = 0;
    flags.x_one = 1; // This bit is always 1.
    flags.cy = 0;

    // Reset general purpose registers, stack pointer, and program counter.
    regs.a = 0;
    regs.b = 0;
    regs.c = 0;
    regs.d = 0;
    regs.e = 0;
    regs.h = 0;
    regs.l = 0;
    regs.sp = 0;
    regs.pc = 0; // Program execution starts at 0x0000 on reset.
}

Intel8080 *Intel8080::attachBus(Bus *bus)
{
    this->bus = bus;
    return this;
}

void Intel8080::fetchOpcode()
{
    opcode = bus->readByte(regs.pc++);
    spdlog::debug("Intel8080::fetchOpcode(): opcode = 0x{:02X}     PC = 0x{:04X}", opcode, regs.pc);
}

void Intel8080::fetchByte()
{
    byteData = bus->readByte(regs.pc++);
    spdlog::debug("Intel8080::fetchByte(): byteDta = 0x{:02X}     PC = 0x{:04X}", byteData, regs.pc);
}

void Intel8080::fetchWord()
{
    BYTE low = bus->readByte(regs.pc);
    BYTE high = bus->readByte(regs.pc + 1);
    wordData = (static_cast<WORD>(high) << 8) | static_cast<WORD>(low);
    regs.pc += 2;
    spdlog::debug("Intel8080::fetchWord(): wordData =  0x{:04X}     PC = 0x{:04X}", wordData, regs.pc);
}

void Intel8080::readOpcode(WORD address)
{
    opcode = bus->readByte(address);
    spdlog::debug("Intel8080::readOpcode(0x{:04X}): opcode = 0x{:02X}     PC = 0x{:04X}", address, opcode, regs.pc);
}

void Intel8080::readByte(WORD address)
{
    byteData = bus->readByte(address);
    spdlog::debug("Intel8080::readByte(0x{:04X}): byteData = 0x{:02X}     PC = 0x{:04X}", address, byteData, regs.pc);
}

void Intel8080::readWord(WORD address)
{
    wordData = bus->readWord(address);
    spdlog::debug("Intel8080::readWord(0x{:04X}): wordData = 0x{:04X}     PC = 0x{:04X}", address, wordData, regs.pc);
}

void Intel8080::writeOpcode(WORD address, BYTE data)
{
    bus->writeByte(address, data);
    spdlog::debug("Intel8080::writeOpcode(0x{:04X}, 0x{:02X})     PC = 0x{:04X}", address, data, regs.pc);
} 

void Intel8080::writeByte(WORD address, BYTE data)
{
    bus->writeByte(address, data);
    spdlog::debug("Intel8080::writeByte(0x{:04X}, 0x{:02X})     PC = 0x{:04X}", address, data, regs.pc);
}

void Intel8080::writeWord(WORD address, WORD data)
{
    bus->writeWord(address, data);
    spdlog::debug("Intel8080::writeWord(0x{:04X}, 0x{:04X})     PC = 0x{:04X}", address, data, regs.pc);
}

void Intel8080::execute()
{
    spdlog::debug("Intel8080::execute(): Executing opcode 0x{:02X} at PC = 0x{:04X}", opcode, regs.pc - 1);
    (this->*p_opcode_lookup[opcode])();
}

void Intel8080::step()
{
    fetchOpcode();
    execute();
}

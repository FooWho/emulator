#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <array>
#include "intel8080.hpp"
#include "virtualMemory.hpp"
#include "bus.hpp"
#include "intel8080TestHelper.hpp"


Intel8080::Intel8080()
{
    reset();
    buildOpcodeTable();
    bus = nullptr;
    inPeripheralDevices.fill(nullptr);
    outPeripheralDevices.fill(nullptr);
}

void Intel8080::reset()
{
    wordData = 0;
    byteData = 0;
    opcode = 0;
    interruptsEnabled = false;
    interruptFlag = 0;
    isHalted = false;


    // Reset flags to their initial state.
    regs.f.flags = 0x02; // Set the always-on bit (bit 1) to 1

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

Intel8080 *Intel8080::attachInputPeripheral(Intel8080PeripheralDevice *device, BYTE port)
{
    inPeripheralDevices[port] = device;
    return this;
}

Intel8080 *Intel8080::attachOutputPeripheral(Intel8080PeripheralDevice *device, BYTE deviceID)
{
    outPeripheralDevices[deviceID] = device;
    return this;
}

void Intel8080::fetchOpcode()
{
    //if (regs.pc == 0x15D3) {
    //    printState();
    //}
    opcode = bus->readByte(regs.pc++);
}

void Intel8080::fetchByte()
{
    byteData = bus->readByte(regs.pc++);
}

void Intel8080::fetchWord()
{
    BYTE low = bus->readByte(regs.pc);
    BYTE high = bus->readByte(regs.pc + 1);
    wordData = (static_cast<WORD>(high) << 8) | static_cast<WORD>(low);
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

int Intel8080::execute()
{
    return (this->*pOpcodeLookup[opcode])();
}

void Intel8080::interrupt(BYTE isrVector)
{
    interruptFlag = isrVector;
}

int Intel8080::step()
{
    if (isHalted) return 4; // Consume cycles while halted

    if (interruptsEnabled && interruptFlag) {
        interruptsEnabled = false;
        isHalted = false;
        BYTE vector = interruptFlag;
        interruptFlag = 0;
        switch (vector) {
            case 0x01:
                return opRST_1();
            case 0x02:
                return opRST_2();
            case 0x04:
                return opRST_3();
            case 0x08:
                return opRST_4();
            case 0x10:
                return opRST_5();
            case 0x20:
                return opRST_6();
            case 0x40:
                return opRST_7();
            default:
                break;
        }
    }
    fetchOpcode();
    return execute();
}

void Intel8080::printState()
{
    printf("Opcode: 0x%02X     SP: 0x%04X   PC: 0x%04X\n", opcode, regs.sp, regs.pc);
    printf("Registers: AF: 0x%04X BC: 0x%04X DE: 0x%04X HL: 0x%04X\n",
           regs.af, regs.bc, regs.de, regs.hl);
    printf("Flags: S: %d Z: %d AC: %d P: %d CY: %d\n", regs.f.s, regs.f.z, regs.f.ac, regs.f.p, regs.f.cy);
}

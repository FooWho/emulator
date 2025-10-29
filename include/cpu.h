#pragma once
#include "types.h"

class Bus; // Forward declaration

class CPU {
 private:

 public:
    virtual void reset() = 0;
    virtual void step() = 0;
    virtual ~CPU() = default;
    virtual CPU *attachBus(Bus *bus) = 0;
    virtual BYTE fetchOpcode() = 0;
    virtual BYTE fetchByte() = 0;
    virtual WORD fetchWord() = 0;
    virtual BYTE readOpcode(WORD address) = 0;
    virtual BYTE readByte(WORD address) = 0;
    virtual WORD readWord(WORD address) = 0;
    virtual void writeOpcode(WORD address, BYTE data) = 0;;
    virtual void writeByte(WORD address, BYTE data) = 0;
    virtual void writeWord(WORD address, WORD data) = 0;
    virtual void execute(BYTE opcode) = 0;
};


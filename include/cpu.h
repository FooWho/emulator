#pragma once
#include "types.h"

class Bus; // Forward declaration

class CPU 
{
 public:
    virtual void reset() = 0;
    virtual void step() = 0;
    virtual ~CPU() = default;
    virtual CPU *attachBus(Bus *bus) = 0;
    virtual void fetchOpcode() = 0;
    virtual void execute() = 0;

 protected:
    virtual void fetchByte() = 0;
    virtual void fetchWord() = 0;
    virtual void readOpcode(WORD address) = 0;
    virtual void readByte(WORD address) = 0;
    virtual void readWord(WORD address) = 0;
    virtual void writeOpcode(WORD address, BYTE data) = 0;
    virtual void writeByte(WORD address, BYTE data) = 0;
    virtual void writeWord(WORD address, WORD data) = 0;
};

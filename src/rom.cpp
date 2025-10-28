#include <cstdio>
#include <stdexcept>
#include "rom.h"

Rom::Rom(WORD size)
{
    romem = std::vector<BYTE>(size, 0);
    bus = nullptr;
} 

Rom *Rom::attachBus(Bus *bus)
{
    this->bus = bus;
    return this;
}

BYTE Rom::read(WORD address)
{
    // Implementation for reading from ROM
    // Should throw runtime_error if address is out of bounds
    if (address >= romem.size()) {
        std::runtime_error ex("Attempt to read from invalid ROM address " + std::to_string(address));
        throw ex;
    }
    return romem[address];
}

void Rom::write(WORD address, BYTE data)
{
    // ROM is read-only; writing is not allowed
    std::runtime_error ex("Attempt to write to ROM at address " + std::to_string(address));
    throw ex;  
}

WORD Rom::memSize()
{
    return static_cast<WORD>(romem.size());
}


void Rom::romLoad(const std::vector<BYTE>& buffer)
{
    for (int i = 0; i < romem.size(); i++) {
        romem[i] = buffer[i];
    }
}

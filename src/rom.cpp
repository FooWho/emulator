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

void Rom::test()
{
    romem[0] = 0x00;
    romem[1] = 0x01; // Example data
    printf("ROM[0]: 0x%02X      (Should be 0x00)\n", read(0));
    printf("ROM[1]: 0x%02X      (Should be 0x01)\n", read(1));
    printf("Should throw exception on out-of-bounds read...\n");
    try {
        read(static_cast<WORD>(romem.size())); // Out of bounds
    } catch (const std::runtime_error &e) {
        printf("Caught expected exception: %s\n", e.what());    
    }
    printf("Should throw exception on write to ROM...\n");
    try {
        write(0, 0xFF);
    } catch (const std::runtime_error &e) {
        printf("Caught expected exception: %s\n", e.what());    
    }
}
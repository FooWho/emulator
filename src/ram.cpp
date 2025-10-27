#include <cstdio>
#include <stdexcept>
#include "ram.h"

Ram::Ram(WORD size)
{
    rwmem = std::vector<BYTE>(size, 0);
    bus = nullptr;
} 

Ram *Ram::attachBus(Bus *bus)
{
    this->bus = bus;
    return this;
}

BYTE Ram::read(WORD address)
{
    // Implementation for reading from ROM
    // Should throw runtime_error if address is out of bounds
    if (address >= rwmem.size()) {
        std::runtime_error ex("Attempt to read from invalid RAM address " + std::to_string(address));
        throw ex;
    }
    return rwmem[address];
}

void Ram::write(WORD address, BYTE data)
{
    // Write data to RAM
        // Should throw runtime_error if address is out of bounds
    if (address >= rwmem.size()) {
        std::runtime_error ex("Attempt to write to invalid RAM address " + std::to_string(address));
        throw ex;
    }
    rwmem[address] = data;
}

WORD Ram::memSize()
{
    return static_cast<WORD>(rwmem.size());
}

void Ram::test()
{
    printf("Testing RAM...\n");
    write(0, 0x00);
    write(1, 0x01); // Example data
    printf("RAM[0]: 0x%02X      (Should be 0x00)\n", read(0));
    printf("RAM[1]: 0x%02X      (Should be 0x01)\n", read(1));
    printf("Should throw exception on out-of-bounds read...\n");
    try {
        read(static_cast<WORD>(rwmem.size())); // Out of bounds
    } catch (const std::runtime_error &e) {
        printf("Caught expected exception: %s\n", e.what());    
    }
    printf("Should throw exception on out-of-bounds write...\n");
    try {
        write(static_cast<WORD>(rwmem.size()), 0xFF); // Out of bounds
    } catch (const std::runtime_error &e) {
        printf("Caught expected exception: %s\n", e.what());
    }
    
}

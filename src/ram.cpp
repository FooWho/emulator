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

BYTE Ram::read(WORD address) const
{
    // Implementation for reading from ROM
    // Should throw runtime_error if address is out of bounds
    if (address >= rwmem.size()) {
        throw std::runtime_error("Attempt to read from invalid RAM address " + std::to_string(address));
    }
    return rwmem[address];
}

void Ram::write(WORD address, BYTE data)
{
    // Write data to RAM
        // Should throw runtime_error if address is out of bounds
    if (address >= rwmem.size()) {
        throw std::runtime_error("Attempt to write to invalid RAM address " + std::to_string(address));
    }
    rwmem[address] = data;
}

WORD Ram::memSize() const
{
    return static_cast<WORD>(rwmem.size());
}


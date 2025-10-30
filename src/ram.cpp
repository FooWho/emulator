#include <cstdio>
#include <stdexcept>
#include <spdlog/spdlog.h>
#include "ram.h"

Ram::Ram(WORD size)
{
    rwmem = std::vector<BYTE>(size, 0);
    bus = nullptr;
} 

Ram::Ram(std::vector<BYTE> initData)
{
    rwmem = std::vector<BYTE>(initData.size(), 0);
    rwmem.assign(initData.begin(), initData.end());
    bus = nullptr;
}

Ram::Ram(WORD size, std::vector<BYTE> initData)
{
    if (initData.size() > size) {
        throw std::runtime_error("Initial data size exceeds RAM size");
    }
    rwmem = std::vector<BYTE>(size, 0);
    for (size_t i = 0; i < initData.size(); ++i) {
        rwmem[i] = initData[i];
    }
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

void Ram::ramLoad(const std::vector<BYTE>& buffer)
{
    if (buffer.size() > rwmem.size()) {
        throw std::runtime_error("Buffer size exceeds RAM size");
    }
    for (int i = 0; i < buffer.size(); ++i) {
        rwmem[i] = buffer[i];
    }
}


WORD Ram::memSize() const
{
    return static_cast<WORD>(rwmem.size());
}


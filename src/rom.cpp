#include <cstdio>
#include <stdexcept>
#include "rom.hpp"

Rom::Rom(WORD size)
{
    romem = std::vector<BYTE>(size, 0);
} 

Rom::Rom(std::vector<BYTE> initData)
{
    romem = std::vector<BYTE>(initData.size(), 0);
    romem.assign(initData.begin(), initData.end());
}

Rom::Rom(WORD size, std::vector<BYTE> initData)
{
    if (initData.size() > size) {
        throw std::runtime_error("Initial data size exceeds ROM size");
    }
    romem = std::vector<BYTE>(size, 0);
    for (size_t i = 0; i < initData.size() && i < size; ++i) {
        romem[i] = initData[i];
    }
}

BYTE Rom::read(WORD address) const
{
    // Implementation for reading from ROM
    // Should throw runtime_error if address is out of bounds
    if (address >= romem.size()) {
        throw std::runtime_error("Attempt to read from invalid ROM address " + std::to_string(address));
    }
    return romem[address];
}

void Rom::write(WORD address, BYTE data)
{
    // ROM is read-only; writing is not allowed
    //throw std::runtime_error("Attempt to write to ROM at address " + std::to_string(address));
    printf("Tried to write to ROM at address 0x%04X with data 0x%02X\n", address, data);
    return;
}

WORD Rom::memSize() const
{
    WORD mySize = romem.size();
    return static_cast<WORD>(romem.size());
}


void Rom::romLoad(const std::vector<BYTE>& buffer)
{
    if (buffer.size() > romem.size()) {
        throw std::runtime_error("Buffer size exceeds ROM size");
    }
    for (int i = 0; i < buffer.size(); ++i) {
        romem[i] = buffer[i];
    }
}

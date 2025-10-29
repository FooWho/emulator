#include <stdexcept>
#include <spdlog/spdlog.h>
#include "bus.h"
#include "virtualMemory.h"
#include "intel8080.h"

Bus::Bus()
{
    cpu = nullptr;
    device_count = 0;
    memory_map = new std::array<VirtualMemory*, MEMORY_MAP_SIZE>();
    return;
}

Bus *Bus::attachCpu(Intel8080* cpu)
{
    this->cpu = cpu;
    return this;
}

Bus *Bus::attachMemory(VirtualMemory *memory)
{
    memory_map->at(device_count++) = memory;
    return this;
} 

BYTE Bus::readByte(WORD address)
{
    WORD effectiveAddress = address;
    for (unsigned int i = 0; i < device_count; i++) {
        if (effectiveAddress < ((*memory_map)[i])->memSize()) {
            spdlog::debug("BUS: Address=0x{:04X} mapped to device {:02d} with effectiveAddress=0x{:04X} contains BYTE=0x{:02X}", address, i, effectiveAddress, ((*memory_map)[i])->read(effectiveAddress));
            return ((*memory_map)[i])->read(effectiveAddress);
        } else {
            effectiveAddress -= ((*memory_map)[i])->memSize();
        }
    }
    throw std::runtime_error("Attempt to read from unmapped memory address " + std::to_string(address));    
}

WORD Bus::readWord(WORD address)
{
    BYTE low = readByte(address);
    BYTE high = readByte(address + 1);
    return (static_cast<WORD>(high) << 8) | static_cast<WORD>(low);
}

void Bus::writeByte(WORD address, BYTE data)
{
    for (unsigned int i = 0; i < device_count; i++) {
        if (address < ((*memory_map)[i])->memSize()) {
            ((*memory_map)[i])->write(address, data);
            return;
        } else {
            address -= ((*memory_map)[i])->memSize();
        }
    }
    throw std::runtime_error("Attempt to write to unmapped memory address " + std::to_string(address));    
}

void Bus::writeWord(WORD address, WORD data)
{
    BYTE low = static_cast<BYTE>(data & 0x00FF);
    BYTE high = static_cast<BYTE>((data & 0xFF00) >> 8);
    writeByte(address, low);
    writeByte(address + 1, high);
}   
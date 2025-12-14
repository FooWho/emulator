#include <stdexcept>
#include "bus.hpp"
#include "virtualMemory.hpp"
#include "intel8080.hpp"

Bus::Bus()
{
    memory_map.clear();
    return;
}

Bus *Bus::attachMemory(VirtualMemory *memory, WORD startAddress, WORD endAddress)
{
    MemoryMapping mappedMemory(startAddress, endAddress, memory);
    if (mappedMemory.startAddress > mappedMemory.endAddress) {
        throw std::invalid_argument("Start address cannot be greater than end address");
    }
    if ((mappedMemory.endAddress - mappedMemory.startAddress + 1) != mappedMemory.device->memSize()) {
        throw std::invalid_argument("Memory device size does not match mapping range");
    }
    memory_map.push_back(mappedMemory);
    return this;
} 

BYTE Bus::readByte(WORD address) const
{
    // Check for Mirror Effect
    //if (address >= 0x4000) address -= 0x2000;
    for (unsigned int i = 0; i < memory_map.size(); i++) {
        const auto& mapping = memory_map[i];
        if (address >= mapping.startAddress && address <= mapping.endAddress) {
            WORD effectiveAddress = address - mapping.startAddress;
            return (mapping.device->read(effectiveAddress));
        }
    }
    //throw std::runtime_error("Attempt to read from unmapped memory address " + std::to_string(address));    
    // Invalid address, return 0
    //printf("Bad read access: 0x%04X\n", address);
    return 0;
}

void Bus::writeByte(WORD address, BYTE data)
{
    // Check for Mirror Effect
    //if (address >= 0x4000) address -= 0x2000;
    for (unsigned int i = 0; i < memory_map.size(); i++) {
        const auto& mapping = memory_map[i];
        if (address >= mapping.startAddress && address <= mapping.endAddress) {
            WORD effectiveAddress = address - mapping.startAddress;
            mapping.device->write(effectiveAddress, data);
            return;
        }
    }
    //throw std::runtime_error("Attempt to write to unmapped memory address " + std::to_string(address));   
    // Invalid address, do nothing
    //printf("Bad write access: 0x%04X\n", address);
    return;
}

WORD Bus::readWord(WORD address) const
{
    BYTE low = readByte(address);
    BYTE high = readByte(address + 1);
    WORD data = (static_cast<WORD>(high) << 8) | static_cast<WORD>(low);
    return data;
}

void Bus::writeWord(WORD address, WORD data)
{
    BYTE low = static_cast<BYTE>(data & 0x00FF);
    BYTE high = static_cast<BYTE>((data & 0xFF00) >> 8);
    writeByte(address, low);
    writeByte(address + 1, high);
}

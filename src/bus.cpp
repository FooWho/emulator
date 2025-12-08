#include <stdexcept>
#include "bus.h"
#include "virtualMemory.h"
#include "intel8080.h"

Bus::Bus()
{
    cpu = nullptr;
    memory_map.clear();
    return;
}

Bus *Bus::attachCpu(CPU* cpu)
{
    this->cpu = cpu;
    return this;
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
    for (unsigned int i = 0; i < memory_map.size(); i++) {
        const auto& mapping = memory_map[i];
        if (address >= mapping.startAddress && address <= mapping.endAddress) {
            WORD effectiveAddress = address - mapping.startAddress;
            return (mapping.device->read(effectiveAddress));
        }
    }
    throw std::runtime_error("Attempt to read from unmapped memory address " + std::to_string(address));    
}

void Bus::writeByte(WORD address, BYTE data)
{
    for (unsigned int i = 0; i < memory_map.size(); i++) {
        const auto& mapping = memory_map[i];
        if (address >= mapping.startAddress && address <= mapping.endAddress) {
            WORD effectiveAddress = address - mapping.startAddress;
            mapping.device->write(effectiveAddress, data);
            return;
        }
    }
    throw std::runtime_error("Attempt to write to unmapped memory address " + std::to_string(address));    
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

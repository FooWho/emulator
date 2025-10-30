#include <stdexcept>
#include <spdlog/spdlog.h>
#include "bus.h"
#include "virtualMemory.h"
#include "intel8080.h"

Bus::Bus()
{
    cpu = nullptr;
    device_count = 0;
    memory_map.fill({0, 0, nullptr});
    return;
}

Bus *Bus::attachCpu(CPU* cpu)
{
    this->cpu = cpu;
    return this;
}

Bus *Bus::attachMemory(VirtualMemory *memory, WORD startAddress, WORD endAddress)
{
    if (device_count >= MEMORY_MAP_SIZE) {
        throw std::runtime_error("Exceeded maximum number of memory mapped devices");
    }
    if (startAddress > endAddress) {
        throw std::invalid_argument("Start address cannot be greater than end address");
    }
    if ((endAddress - startAddress + 1) != memory->memSize()) {
        throw std::invalid_argument("Memory device size does not match mapping range");
    }
    memory_map[device_count++] = {startAddress, endAddress, memory};
    return this;
} 

BYTE Bus::readByte(WORD address) const
{
    for (unsigned int i = 0; i < device_count; i++) {
        const auto& mapping = memory_map[i];
        if (address >= mapping.startAddress && address <= mapping.endAddress) {
            WORD effectiveAddress = address - mapping.startAddress;
            spdlog::debug("BUS: Address=0x{:04X} mapped to device {:02d} with effectiveAddress=0x{:04X}", address, i, effectiveAddress);
            return mapping.device->read(effectiveAddress);
        }
    }
    throw std::runtime_error("Attempt to read from unmapped memory address " + std::to_string(address));    
}

void Bus::writeByte(WORD address, BYTE data)
{
    for (unsigned int i = 0; i < device_count; i++) {
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
    return (static_cast<WORD>(high) << 8) | static_cast<WORD>(low);
}

void Bus::writeWord(WORD address, WORD data)
{
    BYTE low = static_cast<BYTE>(data & 0x00FF);
    BYTE high = static_cast<BYTE>((data & 0xFF00) >> 8);
    writeByte(address, low);
    writeByte(address + 1, high);
}

#include <stdexcept>
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

BYTE Bus::read(WORD address)
{
    for (unsigned int i = 0; i < device_count; i++) {
        if (address < ((*memory_map)[i])->memSize()) {
            ((*memory_map)[i])->read(address);
            return memory_map->at(i)->read(address);
        } else {
            address -= ((*memory_map)[i])->memSize();
        }
    }
    throw std::runtime_error("Attempt to read from unmapped memory address " + std::to_string(address));    
}
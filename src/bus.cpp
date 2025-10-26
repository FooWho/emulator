#include "bus.h"
#include "virtualMemory.h"

Bus::Bus() : cpu(nullptr), memory(nullptr)
{
    return;
}

Bus *Bus::attachCpu(Intel8080* cpu)
{
    this->cpu = cpu;
    return this;
}

Bus *Bus::attachMemory(VirtualMemory *memory)
{
    this->memory = memory;
    return this;
} 

BYTE Bus::read(WORD address)
{
    if (memory != nullptr) {
        return memory->read(address);
    }
    return 0xFF; // Return 0xFF if no memory is attached
}
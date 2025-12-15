#include "types.hpp"
#include "bus.hpp"
#include "spaceInvadersBus.hpp"
#include "abstractMemory.hpp"
#include <stdexcept>


BYTE SpaceInvadersBus::readByte(WORD address) const {
    // Space Invaders machines only mapped 0x0000 - 0x3FFF. The 2 MSB of the address bus were not used, this results in a "mirror" of the memory being present at addresses 0x4000 - 0x7FFF.
    address = address & 0x3FFF;
    for (unsigned int i = 0; i < memory_map.size(); i++) {
        const auto& mapping = memory_map[i];
        if (address >= mapping.startAddress && address <= mapping.endAddress) {
            WORD effectiveAddress = address - mapping.startAddress;
            return (mapping.device->read(effectiveAddress));
        }
    }
    throw std::runtime_error("Attempt to read from unmapped memory address " + std::to_string(address));   
    //return 0x00;
}

void SpaceInvadersBus::writeByte(WORD address, BYTE data) {
    // Space Invaders machines only mapped 0x0000 - 0x3FFF. The 2 MSB of the address bus were not used, this results in a "mirror" of the memory being present at addresses 0x4000 - 0x7FFF.
    address = address & 0x3FFF;
    for (unsigned int i = 0; i < memory_map.size(); i++) {
        const auto& mapping = memory_map[i];
        if (address >= mapping.startAddress && address <= mapping.endAddress) {
            WORD effectiveAddress = address - mapping.startAddress;
            mapping.device->write(effectiveAddress, data);
            return;
        }
    }
    throw std::runtime_error("Attempt to write to unmapped memory address " + std::to_string(address));   
    //return;
}

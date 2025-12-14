#include "types.hpp"
#include "bus.hpp"
#include "spaceInvadersBus.hpp"
#include "virtualMemory.hpp"
#include <stdexcept>


BYTE SpaceInvadersBus::readByte(WORD address) const {
    // Space Invaders might reference out of bounds memory that it expects to overflow style map into the correct RAM address
    if (address > 0x4000) address = (address & 0x1FFF) | 0x2000;
    for (unsigned int i = 0; i < memory_map.size(); i++) {
        const auto& mapping = memory_map[i];
        if (address >= mapping.startAddress && address <= mapping.endAddress) {
            WORD effectiveAddress = address - mapping.startAddress;
            return (mapping.device->read(effectiveAddress));
        }
    }
    throw std::runtime_error("Attempt to read from unmapped memory address " + std::to_string(address));    
}

void SpaceInvadersBus::writeByte(WORD address, BYTE data) {
    // Space Invaders might reference out of bounds memory that it expects to overflow style map into the correct RAM address
    if (address > 0x4000) address = (address & 0x1FFF) | 0x2000;
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

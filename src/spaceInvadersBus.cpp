#include "types.hpp"
#include "bus.hpp"
#include "spaceInvadersBus.hpp"


BYTE SpaceInvadersBus::readByte(WORD address) const {
    return Bus::readByte(address & 0x3FFF);
}

void SpaceInvadersBus::writeByte(WORD address, BYTE data) {
    Bus::writeByte(address & 0x3FFF, data);
}

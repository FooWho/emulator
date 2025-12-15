#pragma once
#include "types.hpp"
#include "bus.hpp"

class AbstractMemory; // Forward declaration

class SpaceInvadersBus : public Bus {
  public:
    SpaceInvadersBus() { Bus(); }
    ~SpaceInvadersBus() = default;
    
    BYTE readByte(WORD address) const override;
    void writeByte(WORD address, BYTE data) override; 
};
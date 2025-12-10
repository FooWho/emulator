#pragma once
#include "types.h"


class PeripheralDevice {
private:
    BYTE deviceID;
    BYTE data;

public:
    inline PeripheralDevice(BYTE id) { deviceID = id; data = 0; }
    virtual ~PeripheralDevice() = default;

    inline void setID(BYTE id) { deviceID = id; }
    inline BYTE getID() const { return deviceID; }
    
    inline void setData(BYTE d) { data = d; }
    inline BYTE getData() const { return data; }
};
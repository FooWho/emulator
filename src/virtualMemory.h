#pragma once
typedef uint8_t BYTE;
typedef uint16_t WORD;

class VirtualMemory {
 private:

 protected:
    VirtualMemory();

 public:
    virtual BYTE read(WORD address) = 0;
    virtual void write(WORD address, BYTE data) = 0;
};
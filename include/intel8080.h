#pragma once
#include <array>
#include "types.h"

class Bus; // Forward declaration

class Intel8080 {
 private:
    struct p_flags {
        BYTE z : 1;     // Zero
        BYTE s : 1;     // Sign
        BYTE p : 1;     // Parity
        BYTE cy : 1;    // Carry
        BYTE ac : 1;    // Auxillary Carry
        BYTE pad : 3;   // Padding to fill the BYTE
    } flags;

    struct p_regs {
        BYTE a;         // Accumulator
        BYTE b;         // High byte of the BC pair
        BYTE c;         // Low byte of the BC pair
        BYTE d;         // High byte of the DE pair
        BYTE e;         // Low byte of the DE pair
        BYTE h;         // High byte of the HL pair -> Used in indirect addressing operations
        BYTE l;         // Low byte of the HL pair -> Used in indirect addressing operations
        BYTE sp;        // Stack Pointer
        BYTE pc;        // Program Counter
    } regs;

    Bus *bus;

    std::array<void (Intel8080::*)(), 256> p_opcode_lookup;

    void op_ILLEGAL();
    void op_NOP();
    void op_LXI_B_D16();

 public:
    Intel8080();
    void test();
    BYTE fetchOpcode();
    BYTE fetchByte();
    WORD fetchWord();
    BYTE readOpcode(WORD address);
    BYTE readByte(WORD address);
    WORD readWord(WORD address);
    void writeOpcode(WORD address, BYTE data);
    void writeByte(WORD address, BYTE data);
    void writeWord(WORD address, WORD data);
    void execute(BYTE opcode);
    Intel8080 *attachBus(Bus *bus);
};
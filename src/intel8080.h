#pragma once
#include <array>
#include "virtualMemory.h"

typedef uint8_t BYTE;
typedef uint16_t WORD;

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

    std::array<void (Intel8080::*)(), 256> p_opcode_lookup;

    void op_ILLEGAL();
    void op_NOP();

 public:
    Intel8080();
    void test();


};
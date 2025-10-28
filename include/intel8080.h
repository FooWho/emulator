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
    void op_STAX_B();

 public:
    Intel8080();
    Intel8080 *attachBus(Bus *bus);
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

    BYTE examineRegisterA() { return regs.a; }
    void setRegisterA(BYTE value) { regs.a = value; }   
    BYTE examineRegisterB() { return regs.b; }
    void setRegisterB(BYTE value) { regs.b = value; }
    BYTE examineRegisterC() { return regs.c; }
    void setRegisterC(BYTE value) { regs.c = value; }
    BYTE examineRegisterD() { return regs.d; }
    void setRegisterD(BYTE value) { regs.d = value; }
    BYTE examineRegisterE() { return regs.e; }
    void setRegisterE(BYTE value) { regs.e = value; }
    BYTE examineRegisterH() { return regs.h; }
    void setRegisterH(BYTE value) { regs.h = value; }
    BYTE examineRegisterL() { return regs.l; }
    
    WORD examineRegisterBC() { return (static_cast<WORD>(regs.b) << 8) | static_cast<WORD>(regs.c); }
    void setRegisterBC(WORD value) { regs.b = (value >> 8) & 0xFF; regs.c = value & 0xFF; }
    WORD examineRegisterDE() { return (static_cast<WORD>(regs.d) << 8) | static_cast<WORD>(regs.e); }
    void setRegisterDE(WORD value) { regs.d = (value >> 8) & 0xFF; regs.e = value & 0xFF; }
    WORD examineRegisterHL() { return (static_cast<WORD>(regs.h) << 8) | static_cast<WORD>(regs.l); }
    void setRegisterHL(WORD value) { regs.h = (value >> 8) & 0xFF; regs.l = value & 0xFF; }
    WORD examineRegisterSP() { return regs.sp; }
    void setRegisterSP(WORD value) { regs.sp = value; }
    WORD examineRegisterPC() { return regs.pc; }
    void setRegisterPC(WORD value) { regs.pc = value; }
};
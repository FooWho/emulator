#pragma once
#include <array>
#include "cpu.h"
#include "types.h"

class CPUTestHelper; // Forward declaration

class Intel8080 : public CPU {
 private:
    struct p_flags {
        // Flag Register - Bitfield representation
        // Bit 7: Sign 
        // Bit 6: Zero 
        // Bit 5: Not used, always 0 
        // Bit 4: Auxillary Carry 
        // Bit 3: Not used, always 0
        // Bit 2: Parity 
        // Bit 1: Not used, always 1 
        // Bit 0: Carry
        BYTE s : 1;         // Sign
        BYTE z : 1;         // Zero
        BYTE x_zero : 1;    // Not used, always 0
        BYTE ac : 1;        // Auxillary Carry
        BYTE y_zero : 1;    // Not used, always 0
        BYTE p : 1;         // Parity
        BYTE x_one : 1;     // Not used, always 1
        BYTE cy : 1;        // Carry
    } flags;

    struct p_regs {
        BYTE a;         // Accumulator
        BYTE b;         // High byte of the BC pair
        BYTE c;         // Low byte of the BC pair
        BYTE d;         // High byte of the DE pair
        BYTE e;         // Low byte of the DE pair
        BYTE h;         // High byte of the HL pair -> Used in indirect addressing operations
        BYTE l;         // Low byte of the HL pair -> Used in indirect addressing operations
        WORD sp;        // Stack Pointer
        WORD pc;        // Program Counter
    } regs;

    BYTE opcode;
    BYTE byteData;
    WORD wordData;


    void regFlagsBasic(BYTE result);
    void regFlagsAuxCarry(BYTE original, BYTE result);

    inline BYTE examineRegisterA() { return regs.a; }
    inline void setRegisterA(BYTE value) { regs.a = value; }   
    inline BYTE examineRegisterB() { return regs.b; }
    inline void setRegisterB(BYTE value) { regs.b = value; }
    inline BYTE examineRegisterC() { return regs.c; }
    inline void setRegisterC(BYTE value) { regs.c = value; }
    inline BYTE examineRegisterD() { return regs.d; }
    inline void setRegisterD(BYTE value) { regs.d = value; }
    inline BYTE examineRegisterE() { return regs.e; }
    inline void setRegisterE(BYTE value) { regs.e = value; }
    inline BYTE examineRegisterH() { return regs.h; }
    inline void setRegisterH(BYTE value) { regs.h = value; }
    inline BYTE examineRegisterL() { return regs.l; }
    inline void setRegisterL(BYTE value) { regs.l = value; }

    inline WORD examineRegisterBC() { return (static_cast<WORD>(regs.b) << 8) | static_cast<WORD>(regs.c); }
    inline void setRegisterBC(WORD value) { regs.b = (value >> 8) & 0xFF; regs.c = value & 0xFF; }
    inline WORD examineRegisterDE() { return (static_cast<WORD>(regs.d) << 8) | static_cast<WORD>(regs.e); }
    inline void setRegisterDE(WORD value) { regs.d = (value >> 8) & 0xFF; regs.e = value & 0xFF; }
    inline WORD examineRegisterHL() { return (static_cast<WORD>(regs.h) << 8) | static_cast<WORD>(regs.l); }
    inline void setRegisterHL(WORD value) { regs.h = (value >> 8) & 0xFF; regs.l = value & 0xFF; }
    inline WORD examineRegisterSP() { return regs.sp; }
    inline void setRegisterSP(WORD value) { regs.sp = value; }
    inline WORD examineRegisterPC() { return regs.pc; }
    inline void setRegisterPC(WORD value) { regs.pc = value; }

    inline BYTE examineFlags() { return (static_cast<BYTE>(flags.s) << 7) | (static_cast<BYTE>(flags.z) << 6) | (static_cast<BYTE>(flags.x_zero) << 5) | (static_cast<BYTE>(flags.ac) << 4) | (static_cast<BYTE>(flags.y_zero) << 3) | (static_cast<BYTE>(flags.p) << 2) | (static_cast<BYTE>(flags.x_one) << 1) | (static_cast<BYTE>(flags.cy)); }
    inline void setFlags(BYTE value) { flags.s = (value >> 7) & 0x01; flags.z = (value >> 6) & 0x01; flags.x_zero = (value >> 5) & 0x01; flags.ac = (value >> 4) & 0x01; flags.y_zero = (value >> 3) & 0x01; flags.p = (value >> 2) & 0x01; flags.x_one = (value >> 1) & 0x01; flags.cy = value & 0x01; }
    inline BYTE examineOpcode() { return opcode; }
    inline void setOpcode(BYTE value) { opcode = value; }
    inline BYTE examineByteData() { return byteData; }
    inline void setByteData(BYTE value) { byteData = value; }
    inline WORD examineWordData() { return wordData; }
    inline void setWordData(WORD value) { wordData = value; }

    Bus *bus;

    std::array<void (Intel8080::*)(), 256> p_opcode_lookup;
    void op_ILLEGAL();
    void op_NOP();
    void op_LXI_B_D16();
    void op_STAX_B();
    void op_INX_B();
    void op_INR_B();
    void op_DCR_B();
    void op_MVI_B_D8();
    void op_RLC();
    void op_DAD_B();
    void op_LDAX_B();
    void op_DCX_B();
    void op_INR_C();
    void op_DCR_C();
    void op_MVI_C_D8();
    void op_RRC();
    void op_LXI_D_D16();
    void op_STAX_D();
    void op_INX_D();
    void buildOpcodeTable();

 protected:
    void fetchByte() override;
    void fetchWord() override;
    void readOpcode(WORD address) override;
    void readByte(WORD address) override;
    void readWord(WORD address) override;
    void writeOpcode(WORD address, BYTE data) override;
    void writeByte(WORD address, BYTE data) override;
    void writeWord(WORD address, WORD data) override;

 public:
    Intel8080();
    void reset() override;
    // step() is a single fetch-decode-execute cycle.
    void step() override;
    Intel8080 *attachBus(Bus *bus) override;
    void fetchOpcode() override;
    void execute() override;

    friend class CPUTestHelper;
};
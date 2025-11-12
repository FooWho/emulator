#pragma once
#include <array>
#include "cpu.h"
#include "types.h"

class Bus;

class Intel8080TestHelper; // Forward declaration

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
    void regFlagsCarry(WORD result);
    void regFlagsAuxCarry(WORD result);



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

    friend class Intel8080TestHelper;
};
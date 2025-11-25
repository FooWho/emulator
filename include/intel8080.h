#pragma once
#include <array>
#include "cpu.h"
#include "types.h"

class Bus;

class Intel8080TestHelper; // Forward declaration

class Intel8080 : public CPU {
 private:
    struct pFlags {
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
        BYTE xZero : 1;    // Not used, always 0
        BYTE ac : 1;        // Auxillary Carry
        BYTE yZero : 1;    // Not used, always 0
        BYTE p : 1;         // Parity
        BYTE xOne : 1;     // Not used, always 1
        BYTE cy : 1;        // Carry
    } flags;

    struct pRegs {
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
    void regFlagsCarry(WORD ops);
    void regFlagsAuxCarry(WORD ops);
    void regFlagsDoubleCarry(WORD op1, WORD op2);
    void regFlagsAuxDoubleCarry(WORD op1, WORD op2);



    Bus *bus;

    std::array<void (Intel8080::*)(), 256> pOpcodeLookup;
    void opILLEGAL();       // 0x--
    void opNOP();           // 0x00
    void opLXI_B_D16();     // 0x01
    void opSTAX_B();        // 0x02
    void opINX_B();         // 0x03
    void opINR_B();         // 0x04
    void opDCR_B();         // 0x05
    void opMVI_B_D8();      // 0x06
    void opRLC();           // 0x07 
    void opDAD_B();         // 0x09
    void opLDAX_B();        // 0x0A
    void opDCX_B();         // 0x0B
    void opINR_C();         // 0x0C
    void opDCR_C();         // 0x0D
    void opMVI_C_D8();      // 0x0E
    void opRRC();           // 0x0F
    void opLXI_D_D16();     // 0x11
    void opSTAX_D();        // 0x12
    void opINX_D();         // 0x13

    void opLDAX_D();        // 0x1A

    void opLXI_H_D16();     // 0x21

    void opINX_H();         // 0x23

    void opLXI_SP_D16();    // 0x31

    void opMVI_M_D8();      // 0x36

    void opMVI_A_D8();      // 0x3E

    void opMOV_M_A();       // 0x77

    void opMOV_A_H();       // 0x7C

    void opMOV_A_M();       // 0x7F

    void opJNZ();           // 0xC2
    void opJMP();           // 0xC3

    void opRET();           // 0xC9  

    void opCALL();          // 0xCD

    void opPUSH_D();        // 0xD5

    void opPUSH_H();        // 0xE5



    void opCPI_D8();        // 0xFE



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
    void printState();

    friend class Intel8080TestHelper;
};
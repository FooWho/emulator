#pragma once
#include <array>
#include "cpu.h"
#include "types.h"

class Bus;

class Intel8080TestHelper; // Forward declaration

class Intel8080 : public CPU {
 private:

    union pFlags {
        struct {
            // LSB (Bit 0) first for Little Endian machines
            BYTE cy : 1;     // Bit 0: Carry
            BYTE one : 1;    // Bit 1: Always 1 (8080 specific)
            BYTE p : 1;      // Bit 2: Parity
            BYTE zero : 1;   // Bit 3: Always 0
            BYTE ac : 1;     // Bit 4: Auxiliary Carry
            BYTE zero2 : 1;  // Bit 5: Always 0
            BYTE z : 1;      // Bit 6: Zero
            BYTE s : 1;      // Bit 7: Sign
        };
        BYTE flags; // Access everything as a single byte
    };

    struct pRegs {
        union {
            struct {
                pFlags f; // Flags
                BYTE a; // Accumulator
            };
            WORD af;   // Accessed as a 16-bit pair
        };
        union {
            struct {
                BYTE c; // Low byte (Little Endian first)
                BYTE b; // High byte
            };
            WORD bc;    // Accessed as a 16-bit pair
        };
        union {
            struct {
                BYTE e; // Low byte
                BYTE d; // High byte
            };
            WORD de;
        };
        union {
            struct {
                BYTE l; // Low byte
                BYTE h; // High byte
            };
            WORD hl;
        };
        WORD sp;        // Stack Pointer
        WORD pc;        // Program Counter
    } regs;

    bool interruptsEnabled;
    BYTE opcode;
    BYTE byteData;
    WORD wordData;


    void regFlagsSZP(BYTE result);
    void performAdd(BYTE operand, bool withCarry);
    void performSub(BYTE operand, bool withCarry);
    void regFlagsAuxCarry(BYTE op1, BYTE op2, BYTE result);
    void regFlagsDoubleCarry(WORD op1, WORD op2);

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
//  NOP                     // 0x08
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
    void opINR_D();         // 0x14
    void opDCR_D();         // 0x15
    void opMVI_D_D8();      // 0x16
    void opRAL();           // 0x17

    void opDAD_D();         // 0x19
    void opLDAX_D();        // 0x1A
    
    void opINR_E();         // 0x1C
    void opDCR_E();         // 0x1D
    void opMVI_E_D8();      // 0x1E
    void opRAR();           // 0x1F
    void opLXI_H_D16();     // 0x21

    void opINX_H();         // 0x23
    void opINR_H();         // 0x24
    void opDCR_H();         // 0x25
    void opMVI_H_D8();      // 0x26

    void opDAD_H();         // 0x29

    void opDCX_H();         // 0x2B
    void opINR_L();         // 0x2C
    void opDCR_L();         // 0x2D
    void opMVI_L_D8();      // 0x2E
    void opCMA();           // 0x2F
    void opLXI_SP_D16();    // 0x31
    void opSTA();           // 0x32
    void opINX_SP();        // 0x33
    void opINR_M();         // 0x34
    void opDCR_M();         // 0x35
    void opMVI_M_D8();      // 0x36

    void opLDA();           // 0x3A

    void opINR_A();         // 0x3C
    void opDCR_A();         // 0x3D

    void opMVI_A_D8();      // 0x3E

    void opMOV_B_B();       // 0x40
    void opMOV_B_C();       // 0x41
    void opMOV_B_D();       // 0x42
    void opMOV_B_E();       // 0x43
    void opMOV_B_H();       // 0x44
    void opMOV_B_L();       // 0x45
    void opMOV_B_M();       // 0x46
    void opMOV_B_A();       // 0x47

    void opMOV_C_B();       // 0x48
    void opMOV_C_C();       // 0x49
    void opMOV_C_D();       // 0x4A
    void opMOV_C_E();       // 0x4B
    void opMOV_C_H();       // 0x4C
    void opMOV_C_L();       // 0x4D
    void opMOV_C_M();       // 0x4E
    void opMOV_C_A();       // 0x4F

    void opMOV_D_B();       // 0x50
    void opMOV_D_C();       // 0x51
    void opMOV_D_D();       // 0x52
    void opMOV_D_E();       // 0x53
    void opMOV_D_H();       // 0x54
    void opMOV_D_L();       // 0x55
    void opMOV_D_M();       // 0x56
    void opMOV_D_A();       // 0x57

    void opMOV_E_B();       // 0x58
    void opMOV_E_C();       // 0x59
    void opMOV_E_D();       // 0x5A
    void opMOV_E_E();       // 0x5B
    void opMOV_E_H();       // 0x5C
    void opMOV_E_L();       // 0x5D
    void opMOV_E_M();       // 0x5E
    void opMOV_E_A();       // 0x5F

    void opMOV_H_B();       // 0x60
    void opMOV_H_C();       // 0x61
    void opMOV_H_D();       // 0x62
    void opMOV_H_E();       // 0x63
    void opMOV_H_H();       // 0x64
    void opMOV_H_L();       // 0x65
    void opMOV_H_M();       // 0x66
    void opMOV_H_A();       // 0x67

    void opMOV_L_B();       // 0x68
    void opMOV_L_C();       // 0x69
    void opMOV_L_D();       // 0x6A
    void opMOV_L_E();       // 0x6B
    void opMOV_L_H();       // 0x6C
    void opMOV_L_L();       // 0x6D
    void opMOV_L_M();       // 0x6E
    void opMOV_L_A();       // 0x6F

    void opMOV_M_B();       // 0x70
    void opMOV_M_C();       // 0x71
    void opMOV_M_D();       // 0x72
    void opMOV_M_E();       // 0x73
    void opMOV_M_H();       // 0x74
    void opMOV_M_L();       // 0x75

    void opHLT();           // 0x76

    void opMOV_M_A();       // 0x77
    void opMOV_A_B();       // 0x78
    void opMOV_A_C();       // 0x79
    void opMOV_A_D();       // 0x7A
    void opMOV_A_E();       // 0x7B
    void opMOV_A_H();       // 0x7C
    void opMOV_A_L();       // 0x7D
    void opMOV_A_M();       // 0x7E
    void opMOV_A_A();       // 0x7F

    void opANA_A();         // 0xA7

    void opXRA_A();         // 0xAF

    void opRNZ();           // 0xC0
    void opPOP_B();         // 0xC1
    void opJNZ();           // 0xC2
    void opJMP();           // 0xC3
    void opCNZ();           // 0xC4
    void opPUSH_B();        // 0xC5
    void opADI_D8();        // 0xC6

    void opRZ();            // 0xC8
    void opRET();           // 0xC9  

    void opJZ();            // 0xCA

    void opCZ();            // 0xCC
    void opCALL();          // 0xCD
    void opACI_D8();        // 0xCE

    void opRNC();           // 0xD0
    void opPOP_D();         // 0xD1
    void opJNC();           // 0xD2
    void opOUT_D8();        // 0xD3
    void opCNC();           // 0xD4
    void opPUSH_D();        // 0xD5
    void opSUI_D8();        // 0xD6

    void opRC();            // 0xD8

    void opJC();            // 0xDA

    void opCC();            // 0xDC

    void opSBI_D8();        // 0xDE

    void opRPO();           // 0xE0
    void opPOP_H();         // 0xE1
    void opJPO();           // 0xE2

    void opCPO();           // 0xE4
    void opPUSH_H();        // 0xE5
    void opANI_D8();        // 0xE6
    
    void opRPE();           // 0xE8

    void opJPE();           // 0xEA
    void opXCHG();          // 0xEB
    void opCPE();           // 0xEC

    void opXRI_D8();        // 0xEE

    void opRP();            // 0xF0
    void opPOP_PSW();       // 0xF1
    void opJP();            // 0xF2

    void opCP();            // 0xF4
    void opPUSH_PSW();      // 0xF5
    void opORI_D8();        // 0xF6

    void opRM();            // 0xF8

    void opJM();            // 0xFA
    void opEI();            // 0xFB
    void opCM();            // 0xFC
    void opDI();            // 0xFD

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
    void step() override;
    Intel8080 *attachBus(Bus *bus) override;
    void fetchOpcode() override;
    void execute() override;
    void printState();

    friend class Intel8080TestHelper;
};
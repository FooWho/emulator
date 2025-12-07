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
                            // 0x08 *NOP*
    void opDAD_B();         // 0x09
    void opLDAX_B();        // 0x0A
    void opDCX_B();         // 0x0B
    void opINR_C();         // 0x0C
    void opDCR_C();         // 0x0D
    void opMVI_C_D8();      // 0x0E
    void opRRC();           // 0x0F

                            // 0x10 *NOP*    
    void opLXI_D_D16();     // 0x11
    void opSTAX_D();        // 0x12
    void opINX_D();         // 0x13
    void opINR_D();         // 0x14
    void opDCR_D();         // 0x15
    void opMVI_D_D8();      // 0x16
    void opRAL();           // 0x17
                            // 0x18 *NOP*
    void opDAD_D();         // 0x19
    void opLDAX_D();        // 0x1A
    void opDCX_D();         // 0x1B
    void opINR_E();         // 0x1C
    void opDCR_E();         // 0x1D
    void opMVI_E_D8();      // 0x1E
    void opRAR();           // 0x1F

                            // 0x20 *NOP*
    void opLXI_H_D16();     // 0x21
    void opSHLD();          // 0x22
    void opINX_H();         // 0x23
    void opINR_H();         // 0x24
    void opDCR_H();         // 0x25
    void opMVI_H_D8();      // 0x26
    void opDAA();           // 0x27
                            // 0x28 *NOP*
    void opDAD_H();         // 0x29
    void opLHLD();          // 0x2A
    void opDCX_H();         // 0x2B
    void opINR_L();         // 0x2C
    void opDCR_L();         // 0x2D
    void opMVI_L_D8();      // 0x2E
    void opCMA();           // 0x2F

                            // 0x30 *NOP*
    void opLXI_SP_D16();    // 0x31
    void opSTA();           // 0x32
    void opINX_SP();        // 0x33
    void opINR_M();         // 0x34
    void opDCR_M();         // 0x35
    void opMVI_M_D8();      // 0x36
    void opSTC();           // 0x37
                            // 0x38 *NOP*
    void opDAD_SP();        // 0x39
    void opLDA();           // 0x3A
    void opDCX_SP();        // 0x3B
    void opINR_A();         // 0x3C
    void opDCR_A();         // 0x3D
    void opMVI_A_D8();      // 0x3E
    void opCMC();           // 0x3F

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

    void opADD_B();         // 0x80
    void opADD_C();         // 0x81
    void opADD_D();         // 0x82
    void opADD_E();         // 0x83
    void opADD_H();         // 0x84
    void opADD_L();         // 0x85
    void opADD_M();         // 0x86
    void opADD_A();         // 0x87
    void opADC_B();         // 0x88
    void opADC_C();         // 0x89
    void opADC_D();         // 0x8A
    void opADC_E();         // 0x8B
    void opADC_H();         // 0x8C
    void opADC_L();         // 0x8D
    void opADC_M();         // 0x8E
    void opADC_A();         // 0x8F

    void opSUB_B();         // 0x90
    void opSUB_C();         // 0x91
    void opSUB_D();         // 0x92
    void opSUB_E();         // 0x93
    void opSUB_H();         // 0x94
    void opSUB_L();         // 0x95
    void opSUB_M();         // 0x96
    void opSUB_A();         // 0x97
    void opSBB_B();         // 0x98
    void opSBB_C();         // 0x99
    void opSBB_D();         // 0x9A
    void opSBB_E();         // 0x9B
    void opSBB_H();         // 0x9C
    void opSBB_L();         // 0x9D
    void opSBB_M();         // 0x9E
    void opSBB_A();         // 0x9F

    void opANA_B();         // 0xA0
    void opANA_C();         // 0xA1
    void opANA_D();         // 0xA2
    void opANA_E();         // 0xA3
    void opANA_H();         // 0xA4
    void opANA_L();         // 0xA5
    void opANA_M();         // 0xA6
    void opANA_A();         // 0xA7
    void opXRA_B();         // 0xA8
    void opXRA_C();         // 0xA9
    void opXRA_D();         // 0xAA
    void opXRA_E();         // 0xAB
    void opXRA_H();         // 0xAC
    void opXRA_L();         // 0xAD
    void opXRA_M();         // 0xAE
    void opXRA_A();         // 0xAF

    void opORA_B();         // 0xB0
    void opORA_C();         // 0xB1
    void opORA_D();         // 0xB2
    void opORA_E();         // 0xB3
    void opORA_H();         // 0xB4
    void opORA_L();         // 0xB5
    void opORA_M();         // 0xB6
    void opORA_A();         // 0xB7
    void opCMP_B();         // 0xB8
    void opCMP_C();         // 0xB9
    void opCMP_D();         // 0xBA
    void opCMP_E();         // 0xBB
    void opCMP_H();         // 0xBC
    void opCMP_L();         // 0xBD
    void opCMP_M();         // 0xBE
    void opCMP_A();         // 0xBF

    void opRNZ();           // 0xC0
    void opPOP_B();         // 0xC1
    void opJNZ();           // 0xC2
    void opJMP();           // 0xC3
    void opCNZ();           // 0xC4
    void opPUSH_B();        // 0xC5
    void opADI_D8();        // 0xC6
    void opRST_0();         // 0xC7 *** Not yet implimented ***
    void opRZ();            // 0xC8
    void opRET();           // 0xC9  
    void opJZ();            // 0xCA
                            // 0XCB *JMP
    void opCZ();            // 0xCC
    void opCALL();          // 0xCD
    void opACI_D8();        // 0xCE
    void opRST_1();         // 0xCF *** Not yet implimented ***

    void opRNC();           // 0xD0
    void opPOP_D();         // 0xD1
    void opJNC();           // 0xD2
    void opOUT_D8();        // 0xD3
    void opCNC();           // 0xD4
    void opPUSH_D();        // 0xD5
    void opSUI_D8();        // 0xD6
    void opRST_2();         // 0xD7 *** Not yet implimented ***
    void opRC();            // 0xD8
                            // 0xD9 *RET*
    void opJC();            // 0xDA
    void opIN_D8();         // 0xDB *** Not yet implimented ***
    void opCC();            // 0xDC
                            // 0xDD *CALL*
    void opSBI_D8();        // 0xDE
    void opRST_3();         // 0xDF *** Not yet implimented ***

    void opRPO();           // 0xE0
    void opPOP_H();         // 0xE1
    void opJPO();           // 0xE2
    void opXTHL();          // 0xE3
    void opCPO();           // 0xE4
    void opPUSH_H();        // 0xE5
    void opANI_D8();        // 0xE6
    void opRST_4();         // 0xE7 *** Not yet implimented ***
    void opRPE();           // 0xE8
    void opPCHL();          // 0xE9
    void opJPE();           // 0xEA
    void opXCHG();          // 0xEB
    void opCPE();           // 0xEC
                            // 0xED *CALL*
    void opXRI_D8();        // 0xEE
    void opRST_5();         // 0xEF *** Not yet implimented ***

    void opRP();            // 0xF0
    void opPOP_PSW();       // 0xF1
    void opJP();            // 0xF2
    void opDI();            // 0xF3
    void opCP();            // 0xF4
    void opPUSH_PSW();      // 0xF5
    void opORI_D8();        // 0xF6
    void opRST_6();         // 0xF7 *** Not yet implimented ***
    void opRM();            // 0xF8
    void opSPHL();          // 0xF9
    void opJM();            // 0xFA
    void opEI();            // 0xFB
    void opCM();            // 0xFC
                            // 0xFD *CALL*
    void opCPI_D8();        // 0xFE
    void opRST_7();         // 0xFF *** Not yet implimented ***

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
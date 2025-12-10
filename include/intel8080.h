#pragma once
#include <array>
#include "cpu.h"
#include "peripheralDevice.h"
#include "types.h"

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
    bool isHalted;
    BYTE opcode;
    BYTE byteData;
    WORD wordData;


    void regFlagsSZP(BYTE result);
    void performAdd(BYTE operand, bool withCarry);
    void performSub(BYTE operand, bool withCarry);
    void regFlagsAuxCarry(BYTE op1, BYTE op2, BYTE result);
    void regFlagsDoubleCarry(WORD op1, WORD op2);

    Bus *bus;
    std::array<PeripheralDevice*, 256> inPeripheralDevices;
    std::array<PeripheralDevice*, 256> outPeripheralDevices;


    std::array<int (Intel8080::*)(), 256> pOpcodeLookup;

    int opILLEGAL();       // 0x--

    int opNOP();           // 0x00
    int opLXI_B_D16();     // 0x01
    int opSTAX_B();        // 0x02
    int opINX_B();         // 0x03
    int opINR_B();         // 0x04
    int opDCR_B();         // 0x05
    int opMVI_B_D8();      // 0x06
    int opRLC();           // 0x07 
                           // 0x08 *NOP*
    int opDAD_B();         // 0x09
    int opLDAX_B();        // 0x0A
    int opDCX_B();         // 0x0B
    int opINR_C();         // 0x0C
    int opDCR_C();         // 0x0D
    int opMVI_C_D8();      // 0x0E
    int opRRC();           // 0x0F

                           // 0x10 *NOP*    
    int opLXI_D_D16();     // 0x11
    int opSTAX_D();        // 0x12
    int opINX_D();         // 0x13
    int opINR_D();         // 0x14
    int opDCR_D();         // 0x15
    int opMVI_D_D8();      // 0x16
    int opRAL();           // 0x17
                           // 0x18 *NOP*
    int opDAD_D();         // 0x19
    int opLDAX_D();        // 0x1A
    int opDCX_D();         // 0x1B
    int opINR_E();         // 0x1C
    int opDCR_E();         // 0x1D
    int opMVI_E_D8();      // 0x1E
    int opRAR();           // 0x1F

                           // 0x20 *NOP*
    int opLXI_H_D16();     // 0x21
    int opSHLD();          // 0x22
    int opINX_H();         // 0x23
    int opINR_H();         // 0x24
    int opDCR_H();         // 0x25
    int opMVI_H_D8();      // 0x26
    int opDAA();           // 0x27
                           // 0x28 *NOP*
    int opDAD_H();         // 0x29
    int opLHLD();          // 0x2A
    int opDCX_H();         // 0x2B
    int opINR_L();         // 0x2C
    int opDCR_L();         // 0x2D
    int opMVI_L_D8();      // 0x2E
    int opCMA();           // 0x2F

                           // 0x30 *NOP*
    int opLXI_SP_D16();    // 0x31
    int opSTA();           // 0x32
    int opINX_SP();        // 0x33
    int opINR_M();         // 0x34
    int opDCR_M();         // 0x35
    int opMVI_M_D8();      // 0x36
    int opSTC();           // 0x37
                           // 0x38 *NOP*
    int opDAD_SP();        // 0x39
    int opLDA();           // 0x3A
    int opDCX_SP();        // 0x3B
    int opINR_A();         // 0x3C
    int opDCR_A();         // 0x3D
    int opMVI_A_D8();      // 0x3E
    int opCMC();           // 0x3F

    int opMOV_B_B();       // 0x40
    int opMOV_B_C();       // 0x41
    int opMOV_B_D();       // 0x42
    int opMOV_B_E();       // 0x43
    int opMOV_B_H();       // 0x44
    int opMOV_B_L();       // 0x45
    int opMOV_B_M();       // 0x46
    int opMOV_B_A();       // 0x47
    int opMOV_C_B();       // 0x48
    int opMOV_C_C();       // 0x49
    int opMOV_C_D();       // 0x4A
    int opMOV_C_E();       // 0x4B
    int opMOV_C_H();       // 0x4C
    int opMOV_C_L();       // 0x4D
    int opMOV_C_M();       // 0x4E
    int opMOV_C_A();       // 0x4F

    int opMOV_D_B();       // 0x50
    int opMOV_D_C();       // 0x51
    int opMOV_D_D();       // 0x52
    int opMOV_D_E();       // 0x53
    int opMOV_D_H();       // 0x54
    int opMOV_D_L();       // 0x55
    int opMOV_D_M();       // 0x56
    int opMOV_D_A();       // 0x57
    int opMOV_E_B();       // 0x58
    int opMOV_E_C();       // 0x59
    int opMOV_E_D();       // 0x5A
    int opMOV_E_E();       // 0x5B
    int opMOV_E_H();       // 0x5C
    int opMOV_E_L();       // 0x5D
    int opMOV_E_M();       // 0x5E
    int opMOV_E_A();       // 0x5F

    int opMOV_H_B();       // 0x60
    int opMOV_H_C();       // 0x61
    int opMOV_H_D();       // 0x62
    int opMOV_H_E();       // 0x63
    int opMOV_H_H();       // 0x64
    int opMOV_H_L();       // 0x65
    int opMOV_H_M();       // 0x66
    int opMOV_H_A();       // 0x67
    int opMOV_L_B();       // 0x68
    int opMOV_L_C();       // 0x69
    int opMOV_L_D();       // 0x6A
    int opMOV_L_E();       // 0x6B
    int opMOV_L_H();       // 0x6C
    int opMOV_L_L();       // 0x6D
    int opMOV_L_M();       // 0x6E
    int opMOV_L_A();       // 0x6F

    int opMOV_M_B();       // 0x70
    int opMOV_M_C();       // 0x71
    int opMOV_M_D();       // 0x72
    int opMOV_M_E();       // 0x73
    int opMOV_M_H();       // 0x74
    int opMOV_M_L();       // 0x75
    int opHLT();           // 0x76
    int opMOV_M_A();       // 0x77
    int opMOV_A_B();       // 0x78
    int opMOV_A_C();       // 0x79
    int opMOV_A_D();       // 0x7A
    int opMOV_A_E();       // 0x7B
    int opMOV_A_H();       // 0x7C
    int opMOV_A_L();       // 0x7D
    int opMOV_A_M();       // 0x7E
    int opMOV_A_A();       // 0x7F

    int opADD_B();         // 0x80
    int opADD_C();         // 0x81
    int opADD_D();         // 0x82
    int opADD_E();         // 0x83
    int opADD_H();         // 0x84
    int opADD_L();         // 0x85
    int opADD_M();         // 0x86
    int opADD_A();         // 0x87
    int opADC_B();         // 0x88
    int opADC_C();         // 0x89
    int opADC_D();         // 0x8A
    int opADC_E();         // 0x8B
    int opADC_H();         // 0x8C
    int opADC_L();         // 0x8D
    int opADC_M();         // 0x8E
    int opADC_A();         // 0x8F

    int opSUB_B();         // 0x90
    int opSUB_C();         // 0x91
    int opSUB_D();         // 0x92
    int opSUB_E();         // 0x93
    int opSUB_H();         // 0x94
    int opSUB_L();         // 0x95
    int opSUB_M();         // 0x96
    int opSUB_A();         // 0x97
    int opSBB_B();         // 0x98
    int opSBB_C();         // 0x99
    int opSBB_D();         // 0x9A
    int opSBB_E();         // 0x9B
    int opSBB_H();         // 0x9C
    int opSBB_L();         // 0x9D
    int opSBB_M();         // 0x9E
    int opSBB_A();         // 0x9F

    int opANA_B();         // 0xA0
    int opANA_C();         // 0xA1
    int opANA_D();         // 0xA2
    int opANA_E();         // 0xA3
    int opANA_H();         // 0xA4
    int opANA_L();         // 0xA5
    int opANA_M();         // 0xA6
    int opANA_A();         // 0xA7
    int opXRA_B();         // 0xA8
    int opXRA_C();         // 0xA9
    int opXRA_D();         // 0xAA
    int opXRA_E();         // 0xAB
    int opXRA_H();         // 0xAC
    int opXRA_L();         // 0xAD
    int opXRA_M();         // 0xAE
    int opXRA_A();         // 0xAF

    int opORA_B();         // 0xB0
    int opORA_C();         // 0xB1
    int opORA_D();         // 0xB2
    int opORA_E();         // 0xB3
    int opORA_H();         // 0xB4
    int opORA_L();         // 0xB5
    int opORA_M();         // 0xB6
    int opORA_A();         // 0xB7
    int opCMP_B();         // 0xB8
    int opCMP_C();         // 0xB9
    int opCMP_D();         // 0xBA
    int opCMP_E();         // 0xBB
    int opCMP_H();         // 0xBC
    int opCMP_L();         // 0xBD
    int opCMP_M();         // 0xBE
    int opCMP_A();         // 0xBF

    int opRNZ();           // 0xC0
    int opPOP_B();         // 0xC1
    int opJNZ();           // 0xC2
    int opJMP();           // 0xC3
    int opCNZ();           // 0xC4
    int opPUSH_B();        // 0xC5
    int opADI_D8();        // 0xC6
    int opRST_0();         // 0xC7 *** Not yet implimented ***
    int opRZ();            // 0xC8
    int opRET();           // 0xC9  
    int opJZ();            // 0xCA
                           // 0XCB *JMP
    int opCZ();            // 0xCC
    int opCALL();          // 0xCD
    int opACI_D8();        // 0xCE
    int opRST_1();         // 0xCF *** Not yet implimented ***

    int opRNC();           // 0xD0
    int opPOP_D();         // 0xD1
    int opJNC();           // 0xD2
    int opOUT_D8();        // 0xD3
    int opCNC();           // 0xD4
    int opPUSH_D();        // 0xD5
    int opSUI_D8();        // 0xD6
    int opRST_2();         // 0xD7 *** Not yet implimented ***
    int opRC();            // 0xD8
                           // 0xD9 *RET*
    int opJC();            // 0xDA
    int opIN_D8();         // 0xDB *** Not yet implimented ***
    int opCC();            // 0xDC
                           // 0xDD *CALL*
    int opSBI_D8();        // 0xDE
    int opRST_3();         // 0xDF *** Not yet implimented ***

    int opRPO();           // 0xE0
    int opPOP_H();         // 0xE1
    int opJPO();           // 0xE2
    int opXTHL();          // 0xE3
    int opCPO();           // 0xE4
    int opPUSH_H();        // 0xE5
    int opANI_D8();        // 0xE6
    int opRST_4();         // 0xE7 *** Not yet implimented ***
    int opRPE();           // 0xE8
    int opPCHL();          // 0xE9
    int opJPE();           // 0xEA
    int opXCHG();          // 0xEB
    int opCPE();           // 0xEC
                           // 0xED *CALL*
    int opXRI_D8();        // 0xEE
    int opRST_5();         // 0xEF *** Not yet implimented ***

    int opRP();            // 0xF0
    int opPOP_PSW();       // 0xF1
    int opJP();            // 0xF2
    int opDI();            // 0xF3
    int opCP();            // 0xF4
    int opPUSH_PSW();      // 0xF5
    int opORI_D8();        // 0xF6
    int opRST_6();         // 0xF7 *** Not yet implimented ***
    int opRM();            // 0xF8
    int opSPHL();          // 0xF9
    int opJM();            // 0xFA
    int opEI();            // 0xFB
    int opCM();            // 0xFC
                           // 0xFD *CALL*
    int opCPI_D8();        // 0xFE
    int opRST_7();         // 0xFF *** Not yet implimented ***

    void buildOpcodeTable();

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
    int step() override;
    Intel8080 *attachBus(Bus *bus) override;
    Intel8080 *attachInputPeripheral(PeripheralDevice *device, BYTE deviceID);
    Intel8080 *attachOutputPeripheral(PeripheralDevice *device, BYTE deviceID);

    void fetchOpcode() override;
    int execute() override;
    void printState();

    friend class Intel8080TestHelper;
};
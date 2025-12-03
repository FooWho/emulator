#include <spdlog/spdlog.h>
#include "types.h"
#include "intel8080.h"

void Intel8080::buildOpcodeTable()
{
    pOpcodeLookup.fill(&Intel8080::opILLEGAL);          // Initialize with a dummy opcode
    pOpcodeLookup[0x00] = &Intel8080::opNOP;            // No OPeration
    pOpcodeLookup[0x01] = &Intel8080::opLXI_B_D16;      // Load eXtended register BC with Immediate value
    pOpcodeLookup[0x02] = &Intel8080::opSTAX_B;         // STore Accumulator at memory location conatined in eXtended register BC
    pOpcodeLookup[0x03] = &Intel8080::opINX_B;          // INcrement eXtended register BC
    pOpcodeLookup[0x04] = &Intel8080::opINR_B;          // INcrement Register B
    pOpcodeLookup[0x05] = &Intel8080::opDCR_B;          // DeCrement Register B
    pOpcodeLookup[0x06] = &Intel8080::opMVI_B_D8;       // MoVe Immediate byte data into register B
    pOpcodeLookup[0x07] = &Intel8080::opRLC;            // Rotate accumulator Left with Carry
    pOpcodeLookup[0x08] = &Intel8080::opNOP;            // *NOP instruction
    pOpcodeLookup[0x09] = &Intel8080::opDAD_B;          // Double Add register HL and BC, store result in HL

    pOpcodeLookup[0x0C] = &Intel8080::opINR_C;          // INcrement Register C
    pOpcodeLookup[0x0D] = &Intel8080::opDCR_C;          // DeCrement Register C
    pOpcodeLookup[0x0E] = &Intel8080::opMVI_C_D8;       // MoVe Immediate byte data into register C
    pOpcodeLookup[0x0F] = &Intel8080::opRRC;            // Rotate accumulator Right with Carry

    pOpcodeLookup[0x11] = &Intel8080::opLXI_D_D16;      // LXI D,D16 instruction
    pOpcodeLookup[0x13] = &Intel8080::opINX_D;          // INX D instruction
    pOpcodeLookup[0x14] = &Intel8080::opINR_D;          // INR D instruction
    pOpcodeLookup[0x15] = &Intel8080::opDCR_D;          // DCR D instruction
    pOpcodeLookup[0x16] = &Intel8080::opMVI_D_D8;       // MVI D,D8 instruction
    pOpcodeLookup[0x19] = &Intel8080::opDAD_D;          // DAD D instruction
    pOpcodeLookup[0x1A] = &Intel8080::opLDAX_D;         // LDAX D instruction

    pOpcodeLookup[0x1C] = &Intel8080::opINR_E;          // INR E instruction
    pOpcodeLookup[0x1D] = &Intel8080::opDCR_E;          // DCR E instruction
    pOpcodeLookup[0x1E] = &Intel8080::opMVI_E_D8;       // MVI E,D8 instruction
    pOpcodeLookup[0x21] = &Intel8080::opLXI_H_D16;      // LXI H,D16 instruction

    pOpcodeLookup[0x23] = &Intel8080::opINX_H;          // INX H instruction
    pOpcodeLookup[0x24] = &Intel8080::opINR_H;          // INR H instruction
    pOpcodeLookup[0x25] = &Intel8080::opDCR_H;          // DCR H instruction
    pOpcodeLookup[0x26] = &Intel8080::opMVI_H_D8;       // MVI H,D8 instruction

    pOpcodeLookup[0x29] = &Intel8080::opDAD_H;          // DAD H instruction

    pOpcodeLookup[0x2C] = &Intel8080::opINR_L;          // Increment L instruction
    pOpcodeLookup[0x2D] = &Intel8080::opDCR_L;          // Decrement L instruction
    pOpcodeLookup[0x2E] = &Intel8080::opMVI_L_D8;       // MoVe Immediate byte data into register L
    pOpcodeLookup[0x31] = &Intel8080::opLXI_SP_D16;     // LXI SP,D16 instruction
    pOpcodeLookup[0x32] = &Intel8080::opSTA;            // STA instruction

    pOpcodeLookup[0x36] = &Intel8080::opMVI_M_D8;       // MVI M,D8 instruction

    pOpcodeLookup[0x3A] = &Intel8080::opLDA;            // LDA instruction

    pOpcodeLookup[0x3C] = &Intel8080::opINR_A;          // INR A instruction
    pOpcodeLookup[0x3D] = &Intel8080::opDCR_A;          // DCR A instruction

    pOpcodeLookup[0x3E] = &Intel8080::opMVI_A_D8;       // MOV M, A instruction

    pOpcodeLookup[0x40] = &Intel8080::opMOV_B_B;        // MOV B, B instruction
    pOpcodeLookup[0x41] = &Intel8080::opMOV_B_C;        // MOV B, C instruction
    pOpcodeLookup[0x42] = &Intel8080::opMOV_B_D;        // MOV B, D instruction
    pOpcodeLookup[0x43] = &Intel8080::opMOV_B_E;        // MOV B, C instruction
    pOpcodeLookup[0x44] = &Intel8080::opMOV_B_H;        // MOV B, H instruction
    pOpcodeLookup[0x45] = &Intel8080::opMOV_B_L;        // MOV B, L instruction
    pOpcodeLookup[0x46] = &Intel8080::opMOV_B_M;        // MOV B, M instruction
    pOpcodeLookup[0x47] = &Intel8080::opMOV_B_A;        // MOV B, A instruction
    
    pOpcodeLookup[0x48] = &Intel8080::opMOV_C_B;        // MOV C, B instruction
    pOpcodeLookup[0x49] = &Intel8080::opMOV_C_C;        // MOV C, C instruction
    pOpcodeLookup[0x4A] = &Intel8080::opMOV_C_D;        // MOV C, D instruction
    pOpcodeLookup[0x4B] = &Intel8080::opMOV_C_E;        // MOV C, E instruction
    pOpcodeLookup[0x4C] = &Intel8080::opMOV_C_H;        // MOV C, H instruction
    pOpcodeLookup[0x4D] = &Intel8080::opMOV_C_L;        // MOV C, L instruction
    pOpcodeLookup[0x4E] = &Intel8080::opMOV_C_M;        // MOV C, M instruction
    pOpcodeLookup[0x4F] = &Intel8080::opMOV_C_A;        // MOV C, A instruction

    pOpcodeLookup[0x50] = &Intel8080::opMOV_D_B;        // MOV D, B instruction
    pOpcodeLookup[0x51] = &Intel8080::opMOV_D_C;        // MOV D, C instruction
    pOpcodeLookup[0x52] = &Intel8080::opMOV_D_D;        // MOV D, D instruction
    pOpcodeLookup[0x53] = &Intel8080::opMOV_D_E;        // MOV D, E instruction
    pOpcodeLookup[0x54] = &Intel8080::opMOV_D_H;        // MOV D, H instruction
    pOpcodeLookup[0x55] = &Intel8080::opMOV_D_L;        // MOV D, L instruction
    pOpcodeLookup[0x56] = &Intel8080::opMOV_D_M;        // MOV D, M instruction
    pOpcodeLookup[0x57] = &Intel8080::opMOV_D_A;        // MOV D, A instruction

    pOpcodeLookup[0x58] = &Intel8080::opMOV_E_B;        // MOV E, B instruction
    pOpcodeLookup[0x59] = &Intel8080::opMOV_E_C;        // MOV E, C instruction
    pOpcodeLookup[0x5A] = &Intel8080::opMOV_E_D;        // MOV E, D instruction
    pOpcodeLookup[0x5B] = &Intel8080::opMOV_E_E;        // MOV E, E instruction
    pOpcodeLookup[0x5C] = &Intel8080::opMOV_E_H;        // MOV E, L instruction
    pOpcodeLookup[0x5D] = &Intel8080::opMOV_E_L;        // MOV E, M instruction
    pOpcodeLookup[0x5E] = &Intel8080::opMOV_E_M;        // MOV E, M instruction
    pOpcodeLookup[0x5F] = &Intel8080::opMOV_E_A;        // MOV E, A instruction

    pOpcodeLookup[0x60] = &Intel8080::opMOV_H_B;        // MOV H, B instruction
    pOpcodeLookup[0x61] = &Intel8080::opMOV_H_C;        // MOV H, C instruction
    pOpcodeLookup[0x62] = &Intel8080::opMOV_H_D;        // MOV H, D instruction
    pOpcodeLookup[0x63] = &Intel8080::opMOV_H_E;        // MOV H, E instruction
    pOpcodeLookup[0x64] = &Intel8080::opMOV_H_H;        // MOV H, H instruction
    pOpcodeLookup[0x65] = &Intel8080::opMOV_H_L;        //
    pOpcodeLookup[0x66] = &Intel8080::opMOV_H_M;        // MOV H, M instruction
    pOpcodeLookup[0x67] = &Intel8080::opMOV_H_A;        // MOV H, A instruction

    pOpcodeLookup[0x68] = &Intel8080::opMOV_L_B;        // MOV L, B instruction
    pOpcodeLookup[0x69] = &Intel8080::opMOV_L_C;        // MOV H, L instruction
    pOpcodeLookup[0x6A] = &Intel8080::opMOV_L_D;        // MOV L, D instruction
    pOpcodeLookup[0x6B] = &Intel8080::opMOV_L_E;        // MOV L, E instruction
    pOpcodeLookup[0x6C] = &Intel8080::opMOV_L_H;        // MOV L, H instruction
    pOpcodeLookup[0x6D] = &Intel8080::opMOV_L_L;        // MOV L, L instruction
    pOpcodeLookup[0x6E] = &Intel8080::opMOV_L_M;        // MOV L, M instruction
    pOpcodeLookup[0x6F] = &Intel8080::opMOV_L_A;        // MOV L, A instruction

    pOpcodeLookup[0x70] = &Intel8080::opMOV_M_B;        // MOV M, B instruction
    pOpcodeLookup[0x71] = &Intel8080::opMOV_M_C;        // MOV M, C instruction
    pOpcodeLookup[0x72] = &Intel8080::opMOV_M_D;        // MOV M, D instruction
    pOpcodeLookup[0x73] = &Intel8080::opMOV_M_E;        // MOV M, E instruction
    pOpcodeLookup[0x74] = &Intel8080::opMOV_M_H;        // MOV M, H instruction
    pOpcodeLookup[0x75] = &Intel8080::opMOV_M_L;        // MOV M, L instruction
    pOpcodeLookup[0x76] = &Intel8080::opHLT;            // HLT instruction
    pOpcodeLookup[0x77] = &Intel8080::opMOV_M_A;        // MOV M, A instruction

    pOpcodeLookup[0x78] = &Intel8080::opMOV_A_B;        // MOV A, B instruction
    pOpcodeLookup[0x79] = &Intel8080::opMOV_A_C;        // MOV A, C instruction
    pOpcodeLookup[0x7A] = &Intel8080::opMOV_A_D;        // MOV A, D instruction
    pOpcodeLookup[0x7B] = &Intel8080::opMOV_A_E;        // MOV A, E instruction
    pOpcodeLookup[0x7C] = &Intel8080::opMOV_A_H;        // MOV A, H instruction
    pOpcodeLookup[0x7D] = &Intel8080::opMOV_A_L;        // MOV A, L instruction
    pOpcodeLookup[0x7E] = &Intel8080::opMOV_A_M;        // MOV A, M instruction
    pOpcodeLookup[0x7F] = &Intel8080::opMOV_A_A;        // MOV A, A instruction

    pOpcodeLookup[0xA7] = &Intel8080::opANA_A;         // ANA A instruction

    pOpcodeLookup[0xAF] = &Intel8080::opXRA_A;          // XRA A instruction

    pOpcodeLookup[0xC0] = &Intel8080::opRNZ;            // Return if Not Zero instruction
    pOpcodeLookup[0xC1] = &Intel8080::opPOP_B;          // POP B instruction
    pOpcodeLookup[0xC2] = &Intel8080::opJNZ;            // JNZ instruction
    pOpcodeLookup[0xC3] = &Intel8080::opJMP;            // JMP instruction
    pOpcodeLookup[0xC4] = &Intel8080::opCNZ;            // CNZ instruction
    pOpcodeLookup[0xC5] = &Intel8080::opPUSH_B;         // PUSH B instruction
    pOpcodeLookup[0xC6] = &Intel8080::opADI_D8;         // ADI D8 instruction

    pOpcodeLookup[0xCC] = &Intel8080::opCZ;             // Call subroutine if Zero instruction
    pOpcodeLookup[0xCD] = &Intel8080::opCALL;           // CALL instruction
    pOpcodeLookup[0xCE] = &Intel8080::opACI_D8;         // ACI D8 instruction

    pOpcodeLookup[0xC8] = &Intel8080::opRZ;             // Return if Zero instruction
    pOpcodeLookup[0xC9] = &Intel8080::opRET;            // RET instruction
    pOpcodeLookup[0xCA] = &Intel8080::opJZ;             // JZ instruction

    pOpcodeLookup[0xD0] = &Intel8080::opRNC;            // Return if No Carry instruction
    pOpcodeLookup[0xD1] = &Intel8080::opPOP_D;          // POP D instruction
    pOpcodeLookup[0xD2] = &Intel8080::opJNC;            // JNC instruction
    pOpcodeLookup[0xD3] = &Intel8080::opOUT_D8;         // OUT D8 instruction
    pOpcodeLookup[0xD4] = &Intel8080::opCNC;            // CNC instruction
    pOpcodeLookup[0xD5] = &Intel8080::opPUSH_D;         // PUSH D instruction
    pOpcodeLookup[0xD6] = &Intel8080::opSUI_D8;         // SUI D8

    pOpcodeLookup[0xD8] = &Intel8080::opRC;             // Return if Carry instruction

    pOpcodeLookup[0xDA] = &Intel8080::opJC;             // JC instruction

    pOpcodeLookup[0xDC] = &Intel8080::opCC;             // CC instruction

    pOpcodeLookup[0xDE] = &Intel8080::opSBI_D8;         // SBI D8

    pOpcodeLookup[0xE0] = &Intel8080::opRPO;            // Return if Parity Odd instruction
    pOpcodeLookup[0xE1] = &Intel8080::opPOP_H;          // POP H instruction
    pOpcodeLookup[0xE2] = &Intel8080::opJPO;            // Jump if Parity Odd instruction

    pOpcodeLookup[0XE4] = &Intel8080::opCPO;            // Call if Parity Odd instruction
    pOpcodeLookup[0xE5] = &Intel8080::opPUSH_H;         // PUSH H instruction
    pOpcodeLookup[0xE6] = &Intel8080::opANI_D8;         // ANI D8 instruction

    pOpcodeLookup[0xE8] = &Intel8080::opRPE;            // Return if Parity Even instruction

    pOpcodeLookup[0xEA] = &Intel8080::opJPE;            // JPE instruction
    pOpcodeLookup[0xEB] = &Intel8080::opXCHG;           // eXCHanGe instruction
    pOpcodeLookup[0xEC] = &Intel8080::opCPE;            // Call if Parity Even instruction
    pOpcodeLookup[0xEE] = &Intel8080::opXRI_D8;         // eXclusive oR Immediate D8

    pOpcodeLookup[0xF0] = &Intel8080::opRP;             // Return if Positive instruction
    pOpcodeLookup[0xF1] = &Intel8080::opPOP_PSW;        // POP PSW instruction
    pOpcodeLookup[0xF2] = &Intel8080::opJP;             // JP instruction

    pOpcodeLookup[0xF4] = &Intel8080::opCP;             // Call if Positive instruction
    pOpcodeLookup[0xF5] = &Intel8080::opPUSH_PSW;       // PUSH PSW instruction
    pOpcodeLookup[0xF6] = &Intel8080::opORI_D8;         // OR Immediate data

    pOpcodeLookup[0xF8] = &Intel8080::opRM;             // Return if Minus instruction

    pOpcodeLookup[0xFA] = &Intel8080::opJM;             // Jump if Minus instruction
    pOpcodeLookup[0xFB] = &Intel8080::opEI;             // Enable Interrupts instruction
    pOpcodeLookup[0xFC] = &Intel8080::opCM;             // Call if Minus instruction

    pOpcodeLookup[0xFE] = &Intel8080::opCPI_D8;         // ComPare Immediate instruction


    // ... Add other opcode mappings here       

}

void Intel8080::regFlagsDoubleCarry(WORD op1, WORD op2)
{   
    uint32_t result = op1 + op2;
    if (result > 0xFFFF) {
        regs.f.cy = 1;
    } else {
        regs.f.cy = 0;
    }
}

void Intel8080::regFlagsSZP(BYTE result)
{
    regs.f.s = (result & 0x80) ? 1 : 0;
    regs.f.z = (result == 0) ? 1 : 0;
    regs.f.p = __builtin_popcount(result) % 2 == 0;
} 

void Intel8080::regFlagsAuxCarry(BYTE op1, BYTE op2, BYTE result)
{
    regs.f.ac = ((op1 ^ op2 ^ result) & 0x10) != 0;
}

void Intel8080::performAdd(BYTE operand, bool withCarry) {
    WORD cy = withCarry ? regs.f.cy : 0;
    WORD result = regs.a + operand + cy;

    regFlagsSZP(result & 0xFF);
    regFlagsAuxCarry(regs.a, operand, result);
    regs.f.cy = (result > 0xFF);

    regs.a = (BYTE)result;
}

void Intel8080::performSub(BYTE val, bool withBorrow) {

    BYTE borrow = withBorrow ? regs.f.cy : 0;
    WORD result = regs.a - val - borrow;

    regFlagsSZP(result & 0xFF);
    regFlagsAuxCarry(regs.a, val, result);
    regs.f.cy = (result > 0xFF);

    regs.a = result & 0xFF;
}

void Intel8080::opILLEGAL()
{
    spdlog::debug("ILLEGAL");
    printf("Illegal opcode executed at address 0x%04X\n", regs.pc - 1);
    printState();
    throw std::runtime_error("Illegal opcode executed");
}

void Intel8080::opNOP()
{
    // Opcode: 0x00         Mnemonic: NOP
    // Size: 1  byte        Cycles: 4
    // Description: No Operation
    // Flags: None

    spdlog::debug("NOP");
}

void Intel8080::opLXI_B_D16()
{
    // Opcode: 0x01         Mnemonic: LXI B,D16
    // Size: 3  bytes       Cycles: 10
    // Description: Load immediate 16-bit data into BC register pair
    // Flags: None

    fetchWord();
    regs.bc = wordData;
    spdlog::debug("LXI B, D16 -> B: 0x{:02X} C: 0x{:02X} D16: 0x{:04X}", regs.b, regs.c, wordData);
}

void Intel8080::opSTAX_B()
{
    // Opcode: 0x02         Mnemonic: STAX B
    // Size: 1  byte        Cycles: 7
    // Description: Store Accumulator into memory location pointed by BC register pair
    // Flags: None

    wordData = regs.bc;
    writeByte(wordData, regs.a);
    spdlog::debug("STAX B -> [0x{:04X}] = 0x{:02X}", wordData, regs.a);
}   

void Intel8080::opINX_B()
{
    // Opcode: 0x03         Mnemonic: INX B
    // Size: 1 byte         Cycles: 5
    // Description: Increment the BC register pair
    // Flags: None

    regs.bc++;
    spdlog::debug("INX B -> B: 0x{:02X} C: 0x{:02X} BC: 0x{:04X}", regs.b, regs.c, regs.bc);
}

void Intel8080::opINR_B()
{
    // Opcode: 0x04         Mnemonic: INR B
    // Size: 1              Cycles: 5
    // Description: Increment the B register
    // Flags: S, Z, AC, P

    BYTE result = regs.b + 1;
    regFlagsAuxCarry(regs.b, 1, result);
    regFlagsSZP(result);
    regs.b = result;

    spdlog::debug("INR B -> B: 0x{:02X}", regs.b);
}

void Intel8080::opDCR_B()
{
    // Opcode: 0x05         Mnemonic: DCR B
    // Size: 1              Cycles: 5
    // Description: Decrement the B register
    // Flags: S, Z, AC, P

    BYTE result = regs.b - 1;
    regFlagsAuxCarry(regs.b, 1, result);
    regFlagsSZP(result);
    regs.b = result;

    spdlog::debug("DCR B -> B: 0x{:02X}", regs.b);
}

void Intel8080::opMVI_B_D8()
{
    // Opcode: 0x06          Mnemonic: MVI B, D8
    // Size: 2  bytes       Cycles: 7
    // Description: Move immediate 8-bit data into B
    // Flags: None

    fetchByte();
    regs.b = byteData;
    spdlog::debug("MVI B, D8 -> B: 0x{:02X} D8: 0x{:02X}", regs.b, byteData);
}

void Intel8080::opRLC()
{
    // Opcode: 0x07         Mnemonic: RLC
    // Size: 1              Cycles: 4
    // Description: Rotate Accumulator Left with Carry
    // Flags: CY

    BYTE result = regs.a & 0x01;
    regs.a = (regs.a << 1) | (result >> 7);
    regs.f.cy = result & 0xFF ? 1 : 0;
    spdlog::debug("RRC -> A: 0x{:02X}", regs.a);
}

void Intel8080::opDAD_B()
{
    // Opcode: 0x09         Mnemonic: DAD B
    // Size: 1              Cycles: 10
    // Description: Add the contents of the register pair BC to the register pair HL.
    // Flags: CY

    WORD result = regs.hl + regs.bc;
    regFlagsDoubleCarry(regs.hl, regs.bc);
    spdlog::debug("DAD B -> HL: 0x{:04X} BC: 0x{:04X} -> 0x{:04X}", regs.hl, regs.bc, result);
}

void Intel8080::opINR_C()
{
    // Opcode: 0x0C         Mnemonic: INR C
    // Size: 1              Cycles: 5
    // Description: Increment the C register
    // Flags: S, Z, AC, P

    BYTE result = regs.c + 1;
    regFlagsAuxCarry(regs.c, 1, result);
    regFlagsSZP(result);
    regs.c = result;

    spdlog::debug("INR C -> C: 0x{:02X}", regs.c);
}

void Intel8080::opDCR_C()
{
    // Opcode: 0x0D         Mnemonic: DCR C
    // Size: 1              Cycles: 5
    // Description: Decrement the C register
    // Flags: S, Z, AC, P

    BYTE result = regs.c - 1;
    regFlagsAuxCarry(regs.c, 1, result);
    regFlagsSZP(result);
    regs.c = result;

    spdlog::debug("DCR C -> C: 0x{:02X}", regs.c);
}

void Intel8080::opMVI_C_D8()
{
    // Opcode: 0x0E          Mnemonic: MVI C, D8
    // Size: 2  bytes       Cycles: 7
    // Description: Move immediate 8-bit data into C
    // Flags: None

    fetchByte();
    regs.c = byteData;
    spdlog::debug("MVI C, D8 -> C: 0x{:02X} D8: 0x{:02X}", regs.c, byteData);
}

void Intel8080::opRRC()
{
    // Opcode: 0x0F         Mnemonic: RRC
    // Size: 1              Cycles: 4
    // Description: Rotate Accumulator right through carry
    // Flags: CY

    BYTE result = regs.a & 0x01;
    regs.a = (regs.a >> 1) | (result << 7);
    regs.f.cy = result & 0xFF ? 1 : 0;
    spdlog::debug("RRC -> A: 0x{:02X}", regs.a);
}   

void Intel8080::opLXI_D_D16()
{
    // Opcode: 0x11         Mnemonic: LXI D,D16
    // Size: 3  bytes       Cycles: 10
    // Description: Load immediate 16-bit data into DE register pair
    // Flags: None

    fetchWord();
    regs.de = wordData;
    spdlog::debug("LXI D, D16 -> D: 0x{:02X} E: 0x{:02X} D16: 0x{:04X}", regs.d, regs.e, wordData);
}   

void Intel8080::opINX_D()
{
    // Opcode: 0x13         Mnemonic: INX D
    // Size: 1 byte         Cycles: 5
    // Description: Increment the DE register pair
    // Flags: None

    regs.de++;
    spdlog::debug("INX D -> D: 0x{:02X} E: 0x{:02X} DE: 0x{:04X}", regs.d, regs.e, regs.de);
}

void Intel8080::opINR_D()
{
    // Opcode: 0x14         Mnemonic: INR D
    // Size: 1              Cycles: 5
    // Description: Increment the D register
    // Flags: S, Z, AC, P

    BYTE result = regs.d + 1;
    regFlagsAuxCarry(regs.d, 1, result);
    regFlagsSZP(result);
    regs.d = result;

    spdlog::debug("INR D -> D: 0x{:02X}", regs.d);
}

void Intel8080::opDCR_D()
{
    // Opcode: 0x15         Mnemonic: DCR D
    // Size: 1              Cycles: 5
    // Description: Decrement the D register
    // Flags: S, Z, AC, P

    BYTE result = regs.d - 1;
    regFlagsAuxCarry(regs.d, 1, result);
    regFlagsSZP(result);
    regs.d = result;

    spdlog::debug("DCR D -> D: 0x{:02X}", regs.d);
}

void Intel8080::opMVI_D_D8()
{
    // Opcode: 0x16          Mnemonic: MVI D, D8
    // Size: 2 bytes        Cycles: 7
    // Description: Move immediate 8-bit data into register D
    // Flags: None

    fetchByte();
    regs.d = byteData;
    spdlog::debug("MVI D, D8 -> D: 0x{:02X} D8: 0x{:02X}", regs.d, byteData);
}

void Intel8080::opDAD_D()
{
    // Opcode: 0x19         Mnemonic: DAD D
    // Size: 1              Cycles: 10
    // Description: Add the contents of the register pair DE to the register pair HL.
    // Flags: CY

    WORD result = regs.hl + regs.de;
    regFlagsDoubleCarry(regs.hl, regs.de);
    regs.hl = result;
    spdlog::debug("DAD D -> HL: 0x{:04X} DE: 0x{:04X} -> 0x{:04X}", regs.hl, regs.de, result);
}

void Intel8080::opLDAX_D()
{
    // Opcode: 0x1A         Mnemonic: LDAX D
    // Size: 1              Cycles: 7
    // Description: Load Accumulator from memory location pointed by DE register pair
    // Flags: None

    wordData = regs.de;
    readByte(wordData);
    regs.a = byteData;
    spdlog::debug("LDAX D -> A: 0x{:02X} [0x{:04X}] = 0x{:02X}", regs.a, wordData, byteData);
}

void Intel8080::opINR_E()
{
    // Opcode: 0x1C         Mnemonic: INR E
    // Size: 1              Cycles: 5
    // Description: Increment the E register
    // Flags: S, Z, AC, P

    BYTE result = regs.e + 1;
    regFlagsAuxCarry(regs.e, 1, result);
    regFlagsSZP(result);
    regs.e = result;

    spdlog::debug("INR E -> E: 0x{:02X}", regs.e);
}

void Intel8080::opDCR_E()
{
    // Opcode: 0x1D         Mnemonic: DCR E
    // Size: 1              Cycles: 5
    // Description: Decrement the E register
    // Flags: S, Z, AC, P

    BYTE result = regs.e - 1;
    regFlagsAuxCarry(regs.e, 1, result);
    regFlagsSZP(result);
    regs.e = result;

    spdlog::debug("DCR E -> E: 0x{:02X}", regs.e);
}

void Intel8080::opMVI_E_D8()
{
    // Opcode: 0x1E         Mnemonic: MVI E, D8
    // Size: 2  bytes       Cycles: 7
    // Description: Move immediate byte data into E register
    // Flags: None

    fetchByte();
    regs.e = byteData;
    spdlog::debug("MVI E, D8 -> E: 0x{:02X} D8: 0x{:02X}", regs.e, byteData);
}

void Intel8080::opLXI_H_D16()
{
    // Opcode: 0x21         Mnemonic: LXI H,D16
    // Size: 3  bytes       Cycles: 10
    // Description: Load immediate 16-bit data into HL register pair
    // Flags: None

    fetchWord();
    regs.hl = wordData;
    spdlog::debug("LXI H, D16 -> H: 0x{:02X} L: 0x{:02X} D16: 0x{:04X}", regs.h, regs.l, wordData);
}

void Intel8080::opINX_H()
{
    // Opcode: 0x23         Mnemonic: INX H
    // Size: 1 byte         Cycles: 5
    // Description: Increment the HL register pair
    // Flags: None

    regs.hl++;
    spdlog::debug("INX H -> H: 0x{:02X} L: 0x{:02X} HL: 0x{:04X}", regs.h, regs.l, regs.hl);
}

void Intel8080::opINR_H()
{
    // Opcode: 0x24         Mnemonic: INR H
    // Size: 1              Cycles: 5
    // Description: Increment the H register
    // Flags: S, Z, AC, P

    BYTE result = regs.h + 1;
    regFlagsAuxCarry(regs.h, 1, result);
    regFlagsSZP(result);
    regs.h = result;

    spdlog::debug("INR H -> H: 0x{:02X}", regs.h);
}

void Intel8080::opDCR_H()
{
    // Opcode: 0x25         Mnemonic: DCR H
    // Size: 1              Cycles: 5
    // Description: Decrement the H register
    // Flags: S, Z, AC, P

    BYTE result = regs.h - 1;
    regFlagsAuxCarry(regs.h, 1, result);
    regFlagsSZP(result);
    regs.h = result;

    spdlog::debug("DCR H -> H: 0x{:02X}", regs.h);
}


void Intel8080::opMVI_H_D8()
{
    // Opcode: 0x26         Mnemonic: MVI H, D8
    // Size 2 bytes         Cycles: 7
    // Description: Move immediate 8-bit data into H
    // Flags: None

    fetchByte();
    regs.h = byteData;
    spdlog::debug("MVI H, D8 -> H: 0x{:02X} D8: 0x{:02X}", regs.h, byteData);
}

void Intel8080::opDAD_H()
{
    // Opcode: 0x29         Mnemonic: DAD H
    // Size: 1              Cycles: 10
    // Description: Add the contents of the register pair HL to the register pair HL.
    // Flags: CY

    WORD result = regs.hl + regs.hl;
    regFlagsDoubleCarry(regs.hl, regs.hl);
    regs.hl = result;
    spdlog::debug("DAD H -> HL: 0x{:04X} HL: 0x{:04X} -> 0x{:04X}", regs.hl, regs.hl, result);
}

void Intel8080::opINR_L()
{
    // Opcode: 0x2C         Mnemonic: INR L
    // Size: 1              Cycles: 5
    // Description: Increment the L register
    // Flags: S, Z, AC, P

    BYTE result = regs.l + 1;
    regFlagsAuxCarry(regs.l, 1, result);
    regFlagsSZP(result);
    regs.l = result;

    spdlog::debug("INR L -> L: 0x{:02X}", regs.l);
}

void Intel8080::opDCR_L()
{
    // Opcode: 0x2D         Mnemonic: DRC L
    // Size: 1              Cycles: 5
    // Description: Decrement the L register
    // Flags: S, Z, AC, P

    BYTE result = regs.l - 1;
    regFlagsAuxCarry(regs.l, 1, result);
    regFlagsSZP(result);
    regs.l = result;

    spdlog::debug("DCR L -> L: 0x{:02X}", regs.l);
}

void Intel8080::opLXI_SP_D16()
{
    // Opcode: 0x31         Mnemonic: LXI SP,D16
    // Size: 3  bytes       Cycles: 10
    // Description: Load immediate 16-bit data into Stack Pointer
    // Flags: None

    fetchWord();
    regs.sp = wordData;
    spdlog::debug("LXI SP, D16 -> SP: 0x{:04X} D16: 0x{:04X}", regs.sp, wordData);
}

void Intel8080::opSTA()
{
    // Opcode: 0x32         Mnemonic: STA
    // Size: 3  bytes       Cycles: 13
    // Description: Store the accumulator in the address from WORD operand
    // Flags: None

    fetchWord();
    writeByte(wordData, regs.a);
    spdlog::debug("STA -> [0x{:04X}] = 0x{:02X}", wordData, regs.a);
}


void Intel8080::opMVI_M_D8()
{
    // Opcode: 0x36         Mnemonic: MVI M, D8
    // Size: 2  bytes       Cycles: 10
    // Description: Move immediate byte data into the memory address pointed by HL register pair
    // Flags: None

    fetchByte();
    writeByte(regs.hl, byteData);
}

void Intel8080::opLDA()
{
    // Opcode: 0x3A         Mnemonic: LDA
    // Size: 3 bytes        Cycles: 13
    // Description: Load the accumulator with the contents of the memory address provided by the WORD operand
    // Flags: None

    fetchWord();
    readByte(wordData);
    regs.a = byteData;
    spdlog::debug("LDA -> A: 0x{:02X} [0x{:04X}] = 0x{:02X}", regs.a, wordData, byteData);
}

void Intel8080::opINR_A()
{
    // Opcode: 0x3C         Mnemonic: INR A
    // Size: 1 byte         Cycles: 5
    // Description: Increment the Accumulator
    // Flags: S, Z, AC, P

    BYTE result = regs.a + 1;
    regFlagsAuxCarry(regs.a, 1, result);
    regFlagsSZP(result);
    regs.a = result;

    spdlog::debug("INR A -> A: 0x{:02X}", regs.a);
}

void Intel8080::opDCR_A()
{
    // Opcode: 0x3D         Mnemonic: DCR A
    // Size: 1 byte         Cycles: 5
    // Description: Decrement the Accumulator
    // Flags: S, Z, AC, P

    BYTE result = regs.a - 1;
    regFlagsAuxCarry(regs.a, 1, result);
    regFlagsSZP(result);
    regs.a = result;

    spdlog::debug("DCR A -> A: 0x{:02X}", regs.a);
}

void Intel8080::opMVI_A_D8()
{
    // Opcode: 0x3E         Mnemonic: MVI A, D8
    // Size: 2 bytes        Cycles: 7
    // Description: Move immediate byte data into the accumulator
    // Flags: None

    fetchByte();
    regs.a = byteData;
    spdlog::debug("MVI A, D8 -> A: 0x{:02X} D8: 0x{:02X}", regs.a, byteData);
}

void Intel8080::opMOV_B_B()
{
    // Opcode: 0x40         Mnemonic: MOV B, B
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of B register into B register
    // Flags: None

    regs.b = regs.b;
    spdlog::debug("MOV B,B -> B: 0x{:02X} B: 0x{:02X}", regs.b, regs.b);
}

void Intel8080::opMOV_B_C()
{
    // Opcode: 0x41         Mnemonic: MOV B, C
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of C register into B register
    // Flags: None

    regs.b = regs.c;
    spdlog::debug("MOV B,C -> B: 0x{:02X} C: 0x{:02X}", regs.b, regs.c);
}

void Intel8080::opMOV_B_D()
{
    // Opcode: 0x42         Mnemonic: MOV B, D
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of D register into B register
    // Flags: None

    regs.b = regs.d;
    spdlog::debug("MOV B,D -> B: 0x{:02X} D: 0x{:02X}", regs.b, regs.d);
}

void Intel8080::opMOV_B_E()
{
    // Opcode: 0x43         Mnemonic: MOV B, E
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of E register into B register
    // Flags: None

    regs.b = regs.e;
    spdlog::debug("MOV B,E -> B: 0x{:02X} E: 0x{:02X}", regs.b, regs.e);
}

void Intel8080::opMOV_B_H()
{
    // Opcode: 0x44         Mnemonic: MOV B, H
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of H register into B register
    // Flags: None

    regs.b = regs.h;
    spdlog::debug("MOV B,H -> B: 0x{:02X} H: 0x{:02X}", regs.b, regs.h);
}

void Intel8080::opMOV_B_L()
{
    // Opcode: 0x45         Mnemonic: MOV B, L
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of L register into B register
    // Flags: None

    regs.b = regs.l;
    spdlog::debug("MOV B,L -> B: 0x{:02X} L: 0x{:02X}", regs.b, regs.l);
}

void Intel8080::opMOV_B_M()
{
    // Opcode: 0x46         Mnemonic: MOV B, M
    // Size: 1 byte         Cycles: 7
    // Description: Move contents of memory address stored in HL to B
    // Flags: None

    readByte(regs.hl);
    regs.b = byteData;

    spdlog::debug("MOV B, M -> B: 0x{:02X} [0x{:04X}] = 0x{:02X}", regs.b, regs.hl, byteData);
}



void Intel8080::opMOV_B_A()
{
    // Opcode: 0x47         Mnemonic: MOV B, A
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of Accumulator into B register
    // Flags: None

    regs.b = regs.a;
    spdlog::debug("MOV B,A -> B: 0x{:02X} A: 0x{:02X}", regs.b, regs.a);
}

void Intel8080::opMOV_C_B()
{
    // Opcode: 0x48         Mnemonic: MOV C, B
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of B register into C register
    // Flags: None

    regs.c = regs.b;
    spdlog::debug("MOV C,B -> C: 0x{:02X} B: 0x{:02X}", regs.c, regs.b);
}

void Intel8080::opMOV_C_C()
{
    // Opcode: 0x49         Mnemonic: MOV C, C
    // Size: 1              Cycles: 5
    // Description: Move contents of C register into C register
    // Flags: None

    regs.c = regs.c;
    spdlog::debug("MOV C,C -> C: 0x{:02X} C: 0x{:02X}", regs.c, regs.c);
}

void Intel8080::opMOV_C_D()
{
    // Opcode: 0x4A         Mnemonic: MOV C, D
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of D register into C register
    // Flags: None

    regs.c = regs.d;
    spdlog::debug("MOV C,D -> C: 0x{:02X} D: 0x{:02X}", regs.c, regs.d);
}

void Intel8080::opMOV_C_E()
{
    // Opcode: 0x4B         Mnemonic: MOV C, E
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of E register into C register
    // Flags: None

    regs.c = regs.e;
    spdlog::debug("MOV C,E -> C: 0x{:02X} E: 0x{:02X}", regs.c, regs.e);
}

void Intel8080::opMOV_C_H()
{
    // Opcode: 0x4C         Mnemonic: MOV C, H
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of H register into C register
    // Flags: None

    regs.c = regs.h;
    spdlog::debug("MOV C,H -> C: 0x{:02X} H: 0x{:02X}", regs.c, regs.h);
}

void Intel8080::opMOV_C_L()
{
    // Opcode: 0x4D         Mnemonic: MOV C, L
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of L register into C register
    // Flags: None

    regs.c = regs.l;
    spdlog::debug("MOV C,L -> C: 0x{:02X} L: 0x{:02X}", regs.c, regs.l);
}

void Intel8080::opMOV_C_M()
{
    // Opcode: 0x4E         Mnemonic: MOV C, M
    // Size: 1              Cycles: 7
    // Description: Move contents of memory address contained in HL to C
    // Flags: None

    readByte(regs.hl);
    regs.c = byteData;
    spdlog::debug("MOV C, M -> C: 0x{:02X} [0x{:04X}] = 0x{:02X}", regs.c, regs.hl, byteData);
}

void Intel8080::opMOV_C_A()
{
    // Opcode: 0x4F         Mnemonic: MOV C, A
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of Accumulator into C register
    // Flags: None

    regs.c = regs.a;
    spdlog::debug("MOV C,A -> C: 0x{:02X} A: 0x{:02X}", regs.c, regs.a);
}

void Intel8080::opMOV_D_B()
{
    // Opcode: 0x50         Mnemonic: MOV D, B
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of B register into D register
    // Flags: None

    regs.d = regs.b;
    spdlog::debug("MOV D,B -> D: 0x{:02X} B: 0x{:02X}", regs.d, regs.b);
}

void Intel8080::opMOV_D_C()
{
    // Opcode: 0x51         Mnemonic: MOV D, C
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of C register into D register
    // Flags: None

    regs.d = regs.c;
    spdlog::debug("MOV D,C -> D: 0x{:02X} C: 0x{:02X}", regs.d, regs.c);
}

void Intel8080::opMOV_D_D()
{
    // Opcode: 0x52         Mnemonic: MOV D, D
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of D register into D register
    // Flags: None

    regs.d = regs.d;
    spdlog::debug("MOV D,D -> D: 0x{:02X} D: 0x{:02X}", regs.d, regs.d);
}

void Intel8080::opMOV_D_E()
{
    // Opcode: 0x53         Mnemonic: MOV D, E
    // Size: 1              Cycles: 5
    // Description: Move contents of E register into D register
    // Flags: None

    regs.d = regs.e;
    spdlog::debug("MOV D,E -> D: 0x{:02X} E: 0x{:02X}", regs.d, regs.e);
}

void Intel8080::opMOV_D_H()
{
    // Opcode: 0x54         Mnemonic: MOV D, H
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of H register into D register
    // Flags: None

    regs.d = regs.h;
    spdlog::debug("MOV D,H -> D: 0x{:02X} H: 0x{:02X}", regs.d, regs.h);
}

void Intel8080::opMOV_D_L()
{
    // Opcode: 0x55         Mnemonic: MOV D, L
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of L register into D register
    // Flags: None

    regs.d = regs.l;
    spdlog::debug("MOV D,L -> D: 0x{:02X} L: 0x{:02X}", regs.d, regs.l);
}

void Intel8080::opMOV_D_M()
{
    // Opcode: 0x56         Mnemonic: MOV D, M
    // Size: 1 byte         Cycles: 7
    // Description: Move the conents of memory location pointed by HL register paie into D
    // Flags: None

    readByte(regs.hl);
    regs.d = byteData;
    spdlog::debug("MOV D, M -> D: 0x{:02X} [0x{:04X}] = 0x{:02X}", regs.d, regs.hl, byteData);
}

void Intel8080::opMOV_D_A()
{
    // Opcode: 0x57         Mnemonic: MOV D, A
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of Accumulator into D register
    // Flags: None

    regs.d = regs.a;
    spdlog::debug("MOV D,A -> D: 0x{:02X} A: 0x{:02X}", regs.d, regs.a);
}

void Intel8080::opMOV_E_B()
{
    // Opcode: 0x58         Mnemonic: MOV E, B
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of B register into E register
    // Flags: None

    regs.e = regs.b;
    spdlog::debug("MOV E,B -> E: 0x{:02X} B: 0x{:02X}", regs.e, regs.b);
}

void Intel8080::opMOV_E_C()
{
    // Opcode: 0x59         Mnemonic: MOV E, C
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of C register into E register
    // Flags: None

    regs.e = regs.c;
    spdlog::debug("MOV E,C -> E: 0x{:02X} C: 0x{:02X}", regs.e, regs.c);
}

void Intel8080::opMOV_E_D()
{
    // Opcode: 0x5A         Mnemonic: MOV E, D
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of D register into E register
    // Flags: None

    regs.e = regs.d;
    spdlog::debug("MOV E,D -> E: 0x{:02X} D: 0x{:02X}", regs.e, regs.d);
}

void Intel8080::opMOV_E_E()
{
    // Opcode: 0x5B         Mnemonic: MOV E, E
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of E register into E register
    // Flags: None

    regs.e = regs.e;
    spdlog::debug("MOV E,E -> E: 0x{:02X} E: 0x{:02X}", regs.e, regs.e);
}

void Intel8080::opMOV_E_H()
{
    // Opcode: 0x5C         Mnemonic: MOV E, H
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of H register into E register
    // Flags: None

    regs.e = regs.h;
    spdlog::debug("MOV E,H -> E: 0x{:02X} H: 0x{:02X}", regs.e, regs.h);
}

void Intel8080::opMOV_E_L()
{
    // Opcode: 0x5D         Mnemonic: MOV E, L
    // Size: 1              Cycles: 5
    // Description: Move contents of L register into E register
    // Flags: None

    regs.e = regs.l;
    spdlog::debug("MOV E,L -> E: 0x{:02X} L: 0x{:02X}", regs.e, regs.l);
}

void Intel8080::opMOV_E_M()
{
    // Opcode: 0x5E         Mnemonic: MOV E, M
    // Size: 1  byte        Cycles: 7
    // Description: Move contents of memory location pointed by HL register pair into E
    // Flags: None

    readByte(regs.hl);
    regs.e = byteData;
    spdlog::debug("MOV E, M -> E: 0x{:02X} [0x{:04X}] = 0x{:02X}", regs.e, regs.hl, byteData);
}

void Intel8080::opMOV_E_A()
{
    // Opcode: 0x5F         Mnemonic: MOV E, A
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of Accumulator into E register
    // Flags: None

    regs.e = regs.a;
    spdlog::debug("MOV E,A -> E: 0x{:02X} A: 0x{:02X}", regs.e, regs.a);
}

void Intel8080::opMOV_H_B()
{
    // Opcode: 0x60         Mnemonic: MOV H, B
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of B register into H register
    // Flags: None

    regs.h = regs.b;
    spdlog::debug("MOV H,B -> H: 0x{:02X} B: 0x{:02X}", regs.h, regs.b);
}

void Intel8080::opMOV_H_C()
{
    // Opcode: 0x61         Mnemonic: MOV H, C
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of C register into H register
    // Flags: None

    regs.h = regs.c;
    spdlog::debug("MOV H,C -> H: 0x{:02X} C: 0x{:02X}", regs.h, regs.c);
}

void Intel8080::opMOV_H_D()
{
    // Opcode: 0x62         Mnemonic: MOV H, D
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of D register into H register
    // Flags: None

    regs.h = regs.d;
    spdlog::debug("MOV H,D -> H: 0x{:02X} D: 0x{:02X}", regs.h, regs.d);
}

void Intel8080::opMOV_H_E()
{
    // Opcode: 0x63         Mnemonic: MOV H, E
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of E register into H register
    // Flags: None

    regs.h = regs.e;
    spdlog::debug("MOV H,E -> H: 0x{:02X} E: 0x{:02X}", regs.h, regs.e);
}

void Intel8080::opMOV_H_H()
{
    // Opcode: 0x64         Mnemonic: MOV H, H
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of H register into H register
    // Flags: None

    regs.h = regs.h;
    spdlog::debug("MOV H,H -> H: 0x{:02X} H: 0x{:02X}", regs.h, regs.h);
}

void Intel8080::opMOV_H_L()
{
    // Opcode: 0x65         Mnemonic: MOV H, L
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of L register into H register
    // Flags: None

    regs.h = regs.l;
    spdlog::debug("MOV H,L -> H: 0x{:02X} L: 0x{:02X}", regs.h, regs.l);
}

void Intel8080::opMOV_H_M()
{
    // Opcode: 0x66         Mnemonic: MOV H, M
    // Size: 1 byte         Cycles: 7
    // Description: Move contents of memory location point by HL register pair into H
    // Flags: None

    readByte(regs.hl);
    regs.h = byteData;
    spdlog::debug("MOV H, M -> H: 0x{:02X} [0x{:04X}] = 0x{:02X}", regs.h, regs.hl, byteData);
}

void Intel8080::opMOV_H_A()
{
    // Opcode: 0x67         Mnemonic: MOV H, A
    // Size: 1              Cycles: 5
    // Description: Move contents of Accumulator into H register
    // Flags: None

    regs.h = regs.a;
    spdlog::debug("MOV H,A -> H: 0x{:02X} A: 0x{:02X}", regs.h, regs.a);
}

void Intel8080::opMOV_L_B()
{
    // Opcode: 0x68         Mnemonic: MOV L, B
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of B register into L register
    // Flags: None

    regs.l = regs.b;
    spdlog::debug("MOV L,B -> L: 0x{:02X} B: 0x{:02X}", regs.l, regs.b);
}

void Intel8080::opMOV_L_C()
{
    // Opcode: 0x69         Mnemonic: MOV L, C
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of C register into L register
    // Flags: None

    regs.l = regs.c;
    spdlog::debug("MOV L,C -> L: 0x{:02X} C: 0x{:02X}", regs.l, regs.c);
}

void Intel8080::opMOV_L_D()
{
    // Opcode: 0x6A         Mnemonic: MOV L, D
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of D register into L register
    // Flags: None

    regs.l = regs.d;
    spdlog::debug("MOV L,D -> L: 0x{:02X} D: 0x{:02X}", regs.l, regs.d);
}

void Intel8080::opMOV_L_E()
{
    // Opcode: 0x6B         Mnemonic: MOV L, E
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of E register into L register
    // Flags: None

    regs.l = regs.e;
    spdlog::debug("MOV L,E -> L: 0x{:02X} E: 0x{:02X}", regs.l, regs.e);
}

void Intel8080::opMOV_L_H()
{
    // Opcode: 0x6C         Mnemonic: MOV L, H
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of H register into L register
    // Flags: None

    regs.l = regs.h;
    spdlog::debug("MOV L,H -> L: 0x{:02X} H: 0x{:02X}", regs.l, regs.h);
}

void Intel8080::opMOV_L_L()
{
    // Opcode: 0x6D         Mnemonic: MOV L, L
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of L register into L register
    // Flags: None

    regs.l = regs.l;
    spdlog::debug("MOV L,L -> L: 0x{:02X} L: 0x{:02X}", regs.l, regs.l);
}

void Intel8080::opMOV_L_M()
{
    // Opcode: 0x6E         Mnemonic: MOV L, M
    // Size: 1 byte         Cycles: 7
    // Description: Move contents of memory location contained in HL to register L

    readByte(regs.hl);
    regs.l = byteData;
    spdlog::debug("MOV L, M -> L: 0x{:02X} [0x{:04X}] = 0x{:02X}", regs.l, regs.hl, byteData);
}

void Intel8080::opMOV_L_A()
{
    // Opcode: 0x6F         Mmnemonic: MOV L, A
    // Size: 1  byte        Cycles: 5
    // Description: Move contents of Accumulator into L register
    // Flags: None

    regs.l = regs.a;
    spdlog::debug("MOV L,A -> L: 0x{:02X} A: 0x{:02X}", regs.l, regs.a);
}

void Intel8080::opMOV_M_B()
{
    // Opcode: 0x70         Mnemonic: MOV M, B
    // Size: 1 byte         Cycles: 7
    // Description: Move contents of B register to the memory location contained in HL
    // Flags: None

    writeByte(regs.hl, regs.b);

    spdlog::debug("MOV M,B -> [0x{:04X}] = 0x{:02X}", regs.hl, regs.b);
}

void Intel8080::opMOV_M_C()
{
    // Opcode: 0x71         Mnemonic: MOV M, C
    // Size: 1 byte         Cycles: 7
    // Description: Move contents of C register to the memory location contained in HL
    // Flags: None

    writeByte(regs.hl, regs.c);

    spdlog::debug("MOV M,C -> [0x{:04X}] = 0x{:02X}", regs.hl, regs.c);
}

void Intel8080::opMOV_M_D()
{
    // Opcode: 0x72         Mnemonic: MOV M, D
    // Size: 1 byte         Cycles: 7
    // Description: Move contens of D register to the memory location contained in HL
    // Flags: None

    writeByte(regs.hl, regs.d);

    spdlog::debug("MOV M,D -> [0x{:04X}] = 0x{:02X}", regs.hl, regs.d);
}

void Intel8080::opMOV_M_E()
{
    // Opcode: 0x73         Mnemonic: MOV M, E
    // Size: 1 byte         Cycles: 7
    // Description: Move contents of E register to the memory location contained in HL
    // Flags: None

    writeByte(regs.hl, regs.e);

    spdlog::debug("MOV M,E -> [0x{:04X}] = 0x{:02X}", regs.hl, regs.e);
}

void Intel8080::opMOV_M_H()
{
    // Opcode: 0x74         Mnemonic: MOV M, H
    // Size: 1 byte         Cycles: 7
    // Description: Move contents of H register to the memory location contained in HL
    // Flags: None

    writeByte(regs.hl, regs.h);

    spdlog::debug("MOV M,H -> [0x{:04X}] = 0x{:02X}", regs.hl, regs.h);
}

void Intel8080::opMOV_M_L()
{
    // Opcode: 0x75         Mnemonic: MOV M, L
    // Size: 1 byte         Cycles: 7
    // Description: Move contents of register L to the memory location contained in HL
    // Flags: None

    writeByte(regs.hl, regs.l);

    spdlog::debug("MOV M,L -> [0x{:04X}] = 0x{:02X}", regs.hl, regs.l);
}

void Intel8080::opHLT()
{
    // Opcode: 0x76         Mnemonic: HLT
    // Size: 1 byte         Cycles: 7
    // Description: Advance PC to the next address. CPU enters stopped state and executes no more instructions until an interrupt ocurrs.
    // Flags: None

    spdlog::debug("HLT");
}

void Intel8080::opMOV_M_A()
{
    // Opcode: 0x77         Mnemonic: MOV M,A
    // Size: 1  byte        Cycles: 7
    // Description: Move contents of Accumulator into memory location pointed by HL register pair
    // Flags: None

    writeByte(regs.hl, regs.a);
    spdlog::debug("MOV M,A -> [0x{:04X}] = 0x{:02X}", regs.hl, regs.a);
}   

void Intel8080::opMOV_A_B()
{
    // Opcode: 0x78         Mnemonic: MOV A, B
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of B register into Accumulator
    // Flags: None

    regs.a = regs.b;

    spdlog::debug("MOV A,B -> A: 0x{:02X} B: 0x{:02X}", regs.a, regs.b);
}

void Intel8080::opMOV_A_C()
{
    // Opcode: 0x79         Mnemonic: MOV A, C
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of C register into Accumulator
    // Flags: None

    regs.a = regs.c;
    spdlog::debug("MOV A,C -> A: 0x{:02X} C: 0x{:02X}", regs.a, regs.c);
}

void Intel8080::opMOV_A_D()
{
    // Opcode: 0x7A         Mnemonic: MOV A, D
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of D register into Accumulator
    // Flags: None  

    regs.a = regs.d;
    spdlog::debug("MOV A,D -> A: 0x{:02X} D: 0x{:02X}", regs.a, regs.d);
}

void Intel8080::opMOV_A_E()
{
    // Opcode: 0x7B         Mnemonic: MOV A, E
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of E register into Accumulator
    // Flags: None

    regs.a = regs.e;
    spdlog::debug("MOV A,E -> A: 0x{:02X} E: 0x{:02X}", regs.a, regs.e);
}

void Intel8080::opMOV_A_H()
{
    // Opcode: 0x7C         Mnemonic: MOV A,H
    // Size: 1  byte        Cycles: 5
    // Description: Move contents of H register into Accumulator
    // Flags: None

    regs.a = regs.h;
    spdlog::debug("MOV A,H -> A: 0x{:02X} H: 0x{:02X}", regs.a, regs.h);
}

void Intel8080::opMOV_A_L()
{
    // Opcode: 0x7D         Mnemonic: MOV A, L
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of L register into Accumulator
    // Flags: None

    regs.a = regs.l;
    spdlog::debug("MOV A,L -> A: 0x{:02X} L: 0x{:02X}", regs.a, regs.l);
}

void Intel8080::opMOV_A_M()
{
    // Opcode: 0x7E         Mnemonic: MOV A, M
    // Size: 1 byte         Cycles 7
    // Description: Move contents of memory location pointed by HL into register A
    // Flags: None

    readByte(regs.hl);
    regs.a = byteData;
    spdlog::debug("MOV A,M -> A: 0x{:02X} [0x{:04X}] = 0x{:02X}", regs.a, regs.hl, byteData);
}

void Intel8080::opMOV_A_A()
{
    // Opcode: 0x7F         Mnemonic: MOV A, A
    // Size: 1 byte         Cycles: 5
    // Description: Move contents of Accumulator into Accumulator
    // Flags: None

    regs.a = regs.a;
    spdlog::debug("MOV A,A -> A: 0x{:02X} A: 0x{:02X}", regs.a, regs.a);
}

void Intel8080::opANA_A()
{
    // Opcode: 0xA7         Mnemonic: ANA A
    // Size: 1 byte         Cycles: 4
    // Description: Bitwise AND of the accumulator with itself
    // Flags: S, Z, AC, P, CY

    regFlagsSZP(regs.a & regs.a);
    regs.f.cy = 0;
    regs.f.ac = 0;
    spdlog::debug("ANA A -> A: 0x{:02X}", regs.a);
}

void Intel8080::opXRA_A()
{
    // Opcode: 0xAF         Mnemonic: XRA A
    // Size: 1 byte         Cycles: 4
    // Description: Bitwise XOR operation of Accumulator and Accumulator
    // Flags: S, Z, AC, P, CY

    regs.a = 0;
    regs.f.s = 0;
    regs.f.z = 1;
    regs.f.ac = 0;
    regs.f.p = 1;
    regs.f.cy = 0;
    spdlog::debug("XRA A -> A: 0x{:02X}", regs.a);
}

void Intel8080::opRNZ()
{
    // Opcode: 0xC0         Mnemonic: RNZ
    // Size: 1  byte        Cycles: 11/5
    // Description: Return from subroutine if Zero flag is not set
    // Flags: None  

    if (!regs.f.z) {
        readWord(regs.sp);
        regs.pc = wordData;
        regs.sp += 2;
        spdlog::debug("RNZ taken -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
    } else {
        spdlog::debug("RNZ not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opPOP_B()
{
    // Opcode: 0xC1         Mnemonic: POP B
    // Size: 1 byte         Cycles: 10
    // Description: Pop BC register pair from the stack.
    // Flags: None 

    readWord(regs.sp);
    regs.bc = wordData;
    regs.sp += 2;
    spdlog::debug("POP B -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opJNZ()
{
    // Opcode: 0xC2         Mnemonic: JNZ addr
    // Size: 3  bytes       Cycles: 10
    // Description: Jump to address if Zero flag is not set
    // Flags: None

    fetchWord();
    if (regs.f.z == 0) {
        regs.pc = wordData;
        spdlog::debug("JNZ taken -> PC: 0x{:04X}", regs.pc);
    } else {
        spdlog::debug("JNZ not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opJMP()
{
    // Opcode: 0xC3         Mnemonic: JMP addr
    // Size: 3  bytes       Cycles: 10
    // Description: Jump to address
    // Flags: None

    fetchWord();
    regs.pc = wordData;
    spdlog::debug("JMP -> PC: 0x{:04X}", regs.pc);
} 

void Intel8080::opCNZ()
{
    // Opcode: 0xC4         Mnemonic: CNZ addr
    // Size: 3  bytes       Cycles: 17/11
    // Description: Call subroutine if Zero flag is not set
    // Flags: None

    fetchWord();
    if (!regs.f.z) {
        regs.sp -= 2;
        writeWord(regs.sp, regs.pc);
        regs.pc = wordData;
        spdlog::debug("CNZ -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
    } else {
        spdlog::debug("CNZ not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opPUSH_B()
{
    // Opcode: 0xC5         Mnemonic: PUSH B
    // Size: 1  byte        Cycles: 11
    // Description: Push BC register pair onto the stack.
    // Flags: None

    regs.sp -= 2;
    writeWord(regs.sp, regs.bc);
    spdlog::debug("PUSH B -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opADI_D8()
{
    // Opcode: 0xC6         Mnemonic: ADI D8
    // Size: 2  bytes       Cycles: 7
    // Description: Add immediate data to Accumulator
    // Flags: S, Z, AC, P, CY

    fetchByte();
    performAdd(byteData, false);

    spdlog::debug("ADI D8 -> A: 0x{:02X} D8: 0x{:02X}", regs.a, byteData);
}

void Intel8080::opRZ()
{
    // Opcode: 0xC8         Mnemonic: RZ
    // Size: 1  byte        Cycles: 11/5
    // Description: Return from subroutine if Zero flag is set
    // Flags: None  

    if (regs.f.z) {
        readWord(regs.sp);
        regs.pc = wordData;
        regs.sp += 2;
        spdlog::debug("RZ taken -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
        } else {
        spdlog::debug("RZ not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}


void Intel8080::opRET()
{
    // Opcode: 0xC9         Mnemonic: RET
    // Size: 1  byte        Cycles: 10
    // Description: Return from subroutine
    // Flags: None  

    readWord(regs.sp);
    regs.pc = wordData;
    regs.sp += 2;
    spdlog::debug("RET -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
}

void Intel8080::opJZ()
{
    // Opcode: 0xCA         Mnemonic: JZ
    // Size: 3  bytes       Cycles: 10
    // Description: Jump to address if Zero flag is set
    // Flags: None

    fetchWord();
    if (regs.f.z) {
        regs.pc = wordData;
        spdlog::debug("JZ taken -> PC: 0x{:04X}", regs.pc);
    } else {
        spdlog::debug("JZ not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opCZ()
{
    // Opcode: 0xCC         Mnemonic: CZ addr
    // Size: 3  bytes       Cycles: 17/11
    // Description: Call subroutine if zero bit is set
    // Flags: None

    fetchWord();
    if (regs.f.z) {
        regs.sp -= 2;
        writeWord(regs.sp, regs.pc);
        regs.pc = wordData;
        spdlog::debug("CZ -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
        } else {
        spdlog::debug("CZ not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opCALL()
{
    // Opcode: 0xCD         Mnemonic: CALL addr
    // Size: 3  bytes       Cycles: 17
    // Description: Call subroutine at address
    // Flags: None  

    fetchWord();
    regs.sp -= 2;
    writeWord(regs.sp, regs.pc);
    regs.pc = wordData;
    spdlog::debug("CALL -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
}  

void Intel8080::opACI_D8()
{
    // Opcode: 0xCE         Mnemonic: ACI D8
    // Size: 2  bytes       Cycles: 7
    // Description: Add immediate data to Accumulator with carry
    // Flags: S, Z, AC, P, CY

    fetchByte();
    performAdd(byteData, true);

    spdlog::debug("ACI D8 -> A: 0x{:02X} D8: 0x{:02X}", regs.a, byteData);
}

void Intel8080::opRNC()
{
    // Opcode: 0xD0         Mnemonic: RNC
    // Size: 1  byte        Cycles: 11/5
    // Description: Return from subroutine if carry bit is not set
    // Flags: None  

    if (!regs.f.cy) {
        readWord(regs.sp);
        regs.pc = wordData;
        regs.sp += 2;
        spdlog::debug("RNC taken -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
        } else {
        spdlog::debug("RNC not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}


void Intel8080::opPOP_D()
{
    // Opcode: 0xD1         Mnemonic: POP D
    // Size: 1  byte        Cycles: 10
    // Description: Pop DE register pair from the stack.
    // Flags: None  

    readWord(regs.sp);
    regs.de = wordData;
    regs.sp += 2;
    spdlog::debug("POP D -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opJNC()
{
    // Opcode: 0xD2         Mnemonic: JNC addr
    // Size: 3  bytes       Cycles: 10
    // Description: Jump to address if Carry flag is not set
    // Flags: None  

    fetchWord();
    if (!regs.f.cy) {
        regs.pc = wordData;
        spdlog::debug("JNC taken -> PC: 0x{:04X}", regs.pc);
    } else {
        spdlog::debug("JNC not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}


void Intel8080::opOUT_D8()
{
    // Opcode: 0xD3         Mnemonic: OUT D8
    // Size: 2 bytes        Cycles: 10
    // Description: Special
    // Flags: None  

    fetchByte();
    spdlog::debug("OUT D8 -> D8: 0x{:02X}", byteData);
}

void Intel8080::opCNC()
{
    // Opcode: 0xD4         Mnemonic: CNC addr
    // Size: 3  bytes       Cycles: 17/11
    // Description: Call subroutine if Carry flag is not set
    // Flags: None  

    fetchWord();
    if (!regs.f.cy) {
        regs.sp -= 2;
        writeWord(regs.sp, regs.pc);
        regs.pc = wordData;
        spdlog::debug("CNC -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
        } else {
        spdlog::debug("CNC not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opPUSH_D()
{
    // Opcode: 0xD5         Mnemonic: PUSH D
    // Size: 1  byte        Cycles: 11
    // Description: Push DE register pair onto the stack.
    // Flags: None  

    regs.sp -= 2;
    writeWord(regs.sp, regs.de);
    spdlog::debug("PUSH D -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opSUI_D8()
{
    // Opcode: 0xD6         Mnemonic: SUI D8
    // Size: 2  bytes       Cycles: 7
    // Description: Subtract immediate data from Accumulator

    fetchByte();
    performSub(byteData, false);

    spdlog::debug("SUI D8 -> A: 0x{:02X} D8: 0x{:02X}", regs.a, byteData);
}

void Intel8080::opRC()
{
    // Opcode: 0xD8         Mnemonic: RC
    // Size: 1  byte        Cycles: 11/5
    // Description: Return from subroutine if carry bit is set
    // Flags: None  

    if (regs.f.cy) {
        readWord(regs.sp);
        regs.pc = wordData;
        regs.sp += 2;
        spdlog::debug("RC taken -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
        } else {
        spdlog::debug("RC not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}


void Intel8080::opJC()
{
    // Opcde: 0xDA          Mnemonic: JC addr
    // Size: 3  bytes       Cycles: 10
    // Description: Jump to address if Carry flag is set
    // Flags: None  

    fetchWord();
    if (regs.f.cy) {
        regs.pc = wordData;
        spdlog::debug("JC taken -> PC: 0x{:04X}", regs.pc);
    } else {
        spdlog::debug("JC not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opCC()
{
    // Opcode: 0xDC         Mnemonic: CC addr
    // Size: 3  bytes       Cycles: 17/11
    // Description: Call subroutine if Carry flag is set
    // Flags: None  

    fetchWord();
    if (regs.f.cy) {
        regs.sp -= 2;
        writeWord(regs.sp, regs.pc);
        regs.pc = wordData;
        spdlog::debug("CC -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
        } else {
        spdlog::debug("CC not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opSBI_D8()
{
    // Opcode: 0xDE         Mnemonic: SBI D8
    // Size: 2 bytes        Cycles: 7
    // Description: Subtract immediate data from Accumulator with carry
    // Flags: S, Z, AC, P, CY

    fetchByte();
    performSub(byteData, true);

    spdlog::debug("SBI D8 -> A: 0x{:02X} D8: 0x{:02X}", regs.a, byteData);
}

void Intel8080::opRPO()
{
    // Opcode: 0xE0         Mnemonic: RPO
    // Size: 1  byte        Cycles: 11/5
    // Description: Return from subroutine if parity bit is not set
    // Flags: None  

    if (!regs.f.p) {
        readWord(regs.sp);
        regs.pc = wordData;
        regs.sp += 2;
        spdlog::debug("RPO taken -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
    } else {
        spdlog::debug("RPO not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}


void Intel8080::opPOP_H()
{
    // Opcode: 0xE1         Mnemonic: POP H
    // Size: 1 byte         Cycles: 10
    // Description: Pop HL register pair from the stack.
    // Flags: None  

    readWord(regs.sp);
    regs.hl = wordData;
    regs.sp += 2;
    spdlog::debug("POP H -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opJPO()
{
    // Opcode: 0xE2         Mnemonic: JPO addr
    // Size: 3 bytes        Cycles: 10
    // Description: Jump to address if parity bit is odd (i.e. not set)
    // Flags: None  
    
    fetchWord();
    if (!regs.f.p) {
        regs.pc = wordData;
        spdlog::debug("JPO taken -> PC: 0x{:04X}", regs.pc);
    } else {
        spdlog::debug("JPO not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opCPO()
{
    // Opcode: 0xE4         Mnemonic: CPO addr
    // Size: 3  bytes       Cycles: 17/11
    // Description: Call subroutine if parity bit is odd 

    fetchWord();
    if (!regs.f.p) {
        regs.sp -= 2;
        writeWord(regs.sp, regs.pc);
        regs.pc = wordData;
        spdlog::debug("CPO -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
    } else {
        spdlog::debug("CPO not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opPUSH_H()
{
    // Opcode: 0xE5         Mnemonic: PUSH H
    // Size: 1  byte        Cycles: 11
    // Description: Push HL register pair onto the stack.
    // Flags: None  

    regs.sp -= 2;
    writeWord(regs.sp, regs.hl);
    spdlog::debug("PUSH H -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opANI_D8()
{
    // Opcode: 0xE6         Mnemonic: ANI D8
    // Size: 2  bytes       Cycles: 7
    // Description: Bitwise AND operation of Accumulator and immediated data, result to accumulator
    // Flags: S, Z, AC, P, CY

    fetchByte();
    // The "Quirk": AC reflects the logical OR of bit 3
    // This is inconsistently documented. Google Gemini states that without this behavior, the emulator will fail CPUDiag.bin
    regs.f.ac = ((regs.a | byteData) & 0x08) != 0;
    regs.f.cy = 0;
    regs.a &= byteData;
    regFlagsSZP(regs.a);
    
    spdlog::debug("ANI D8 -> A: 0x{:02X} D8: 0x{:02X}", regs.a, byteData);
}

void Intel8080::opRPE()
{
    // Opcode: 0xE8         Mnemonic: RPE
    // Size: 1  byte        Cycles: 11/5
    // Description: Return from subroutine if parity bit is set
    // Flags: None  

    if (regs.f.p) {
        readWord(regs.sp);
        regs.pc = wordData;
        regs.sp += 2;
        spdlog::debug("RPE taken -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp); 
    } else {
        spdlog::debug("RPE not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opJPE()
{
    // Opcode: 0xEA         Mnemonic: JPE addr
    // Size: 3  bytes       Cycles: 10
    // Description: Jump to address if Parity flag is even
    // Flags: None  

    fetchWord();
    if (regs.f.p) {
        regs.pc = wordData;
        spdlog::debug("JPE taken -> PC: 0x{:04X}", regs.pc);
    } else {
        spdlog::debug("JPE not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}


void Intel8080::opXCHG()
{
    // Opcode: 0xEB         Mnemonic: XCHG
    // Size: 1  byte        Cycles: 5
    // Description: Exchange the HL and DE register pairs
    // Flags: None  

    wordData = regs.de;
    regs.de = regs.hl;
    regs.hl = wordData;

    spdlog::debug("XCHG -> DE: 0x{:04X} HL: 0x{:04X}", regs.de, regs.hl);
}

void Intel8080::opCPE()
{
    // Opcode: 0xEC         Mnemonic: CPE addr
    // Size: 3 bytes        Cycles 17/11
    // Description: Call subroutine if Parity flag is even
    // Flags: None  

    fetchWord();
    if (regs.f.p) {
        regs.sp -= 2;
        writeWord(regs.sp, regs.pc);
        regs.pc = wordData;
        spdlog::debug("CPE -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
    } else {
        spdlog::debug("CPE not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opXRI_D8()
{
    // Opcode: 0xEE         Mnemonic: XRI D8
    // Size: 2  bytes       Cycles: 7
    // Description: Bitwise XOR operation of Accumulator with immediated data
    // Flags: S, Z, AC, P, CY

    fetchByte();
    regs.a ^= byteData;
    regFlagsSZP(regs.a);
    regs.f.cy = 0;
    regs.f.ac = 0;

    spdlog::debug("XRI D8 -> A: 0x{:02X} D8: 0x{:02X}", regs.a, byteData);
}

void Intel8080::opRP()
{
    // Opcode: 0xF0         Mnemonic: RP
    // Size: 1  byte        Cycles: 11/5
    // Description: Return from subroutine if sign bit is not set
    // Flags: None  

    if (!regs.f.s) {
        readWord(regs.sp);
        regs.pc = wordData;
        regs.sp += 2;
        spdlog::debug("RP taken -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
    } else {
        spdlog::debug("RP not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}


void Intel8080::opPOP_PSW()
{
    // Opcode: 0xF1         Mnemonic: POP PSW
    // Size: 1  byte        Cycles: 10
    // Description: Pop stack to PSW
    // Flags: S, Z, AC, P, CY

    readWord(regs.sp);
    regs.af = wordData;
    regs.sp += 2;

    // Force unused flag bits to 1 and 0 as appropriate
    regs.f.one = 1;
    regs.f.zero = 0;
    regs.f.zero2 = 0;

    spdlog::debug("POP PSW -> A: 0x{:02X} SP: 0x{:04X}", regs.a, regs.sp);
}

void Intel8080::opJP()
{
    // Opcode: 0xF2         Mnemonic: JP addr
    // Size: 3 bytes        Cycles: 10
    // Description: Jump to address if positive (i.e. sign bit is not set)
    // Flags: None  

    fetchWord();
    if (!regs.f.s) {
        regs.pc = wordData;
        spdlog::debug("JP taken -> PC: 0x{:04X}", regs.pc);
    } else {
        spdlog::debug("JP not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opCP()
{
    // Opcode: 0xF4         Mnemonic: CP addr
    // Size: 3  bytes       Cycles: 17/11
    // Description: Call subroutine if positive (sign bit is not set)

    fetchWord();
    if (!regs.f.s) {
        regs.sp -= 2;
        writeWord(regs.sp, regs.pc);
        regs.pc = wordData;
        spdlog::debug("CP -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
    } else {
        spdlog::debug("CP not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opPUSH_PSW()
{
    // Opcode: 0xF5         Mnemonic: PUSH PSW
    // Size: 1  byte        Cycles: 11
    // Description: Push PSW onto the stack.
    // Flags: None  

    regs.sp -= 2;
    writeWord(regs.sp, regs.af);
    spdlog::debug("PUSH PSW -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opORI_D8()
{
    // Opcode: 0xF6         Mnemonic: ORI D8
    // Size: 2  bytes       Cycles: 7
    // Description OR immediated data with Accumulator, store result in Accumulator
    // Flags: S, Z, AC, P, CY

    fetchByte();
    regs.a |= byteData;
    regFlagsSZP(regs.a);
    regs.f.cy = 0;
    regs.f.ac = 0;

    spdlog::debug("ORI D8 -> A: 0x{:02X} D8: 0x{:02X}", regs.a, byteData);
}

void Intel8080::opRM()
{
    // Opcode: 0xF8         Mnemonic: RM
    // Size: 1  byte        Cycles: 11/5
    // Description: Return from subroutine if sign bit is set
    // Flags: None  

    if (regs.f.s) {
        readWord(regs.sp);
        regs.pc = wordData;
        regs.sp += 2;
        spdlog::debug("RM taken -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
    } else {
        spdlog::debug("RM not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}


void Intel8080::opJM()
{
    // Opcode: 0xFA         Mnemonic: JM
    // Size: 3  bytes       Cycles: 10
    // Description: Jump to address if minus (i.e. if sign bit is set)
    // Flags: None  

    fetchWord();
    if (regs.f.s) {
        regs.pc = wordData;
        spdlog::debug("JM taken -> PC: 0x{:04X}", regs.pc);
    } else {
        spdlog::debug("JM not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}

void Intel8080::opEI()
{
    // Opcode: 0xFB         Mnemonic: EI
    // Size: 1 byte         Cycles: 4
    // Description: Enable interrupts
    // Flags: None


    interruptsEnabled = true;
    spdlog::debug("EI");
}

void Intel8080::opCM()
{
    // Opcode: 0cFC         Mnemonic: CM addr
    // Size: 3  bytes       Cycles: 17/11
    // Description: Call subroutine at addr if minus (sign bit is set)
    // Flags: None  

    fetchWord();
    if (regs.f.s) {
        regs.sp -= 2;
        writeWord(regs.sp, regs.pc);
        regs.pc = wordData;
        spdlog::debug("CM -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
    } else {
        spdlog::debug("CM not taken -> PC remains: 0x{:04X}", regs.pc);
    }
}


void Intel8080::opCPI_D8()
{
    // Opcode:  0xFE        Mnemonic: CPI D8
    // Size: 2 bytes        Cycles: 7
    // Description: Compare register A with the immediate mode BYTE data. Set flags.
    // Flags: S, Z, AC, P, CY

    fetchByte();
    BYTE accumulator = regs.a;
    performSub(byteData, false);
    regs.a = accumulator;
    spdlog::debug("CPI D8 -> A: 0x{:02X} D8: 0x{:02X}", accumulator, byteData);
}
#include <spdlog/spdlog.h>
#include "types.h"
#include "intel8080.h"

void Intel8080::buildOpcodeTable()
{
    pOpcodeLookup.fill(&Intel8080::opILLEGAL);          // Initialize with a dummy opcode
    pOpcodeLookup[0x00] = &Intel8080::opNOP;            // NOP instruction
    pOpcodeLookup[0x01] = &Intel8080::opLXI_B_D16;      // LXI B,D16 instruction
    pOpcodeLookup[0x02] = &Intel8080::opSTAX_B;         // STAX B instruction
    pOpcodeLookup[0x03] = &Intel8080::opINX_B;          // INX B instructrion
    pOpcodeLookup[0x04] = &Intel8080::opINR_B;          // INR B instruction
    pOpcodeLookup[0x05] = &Intel8080::opDCR_B;          // DCR B instruction
    pOpcodeLookup[0x06] = &Intel8080::opMVI_B_D8;       // MVI B,D8 instruction
    pOpcodeLookup[0x09] = &Intel8080::opDAD_B;          // DAD B instruction
    pOpcodeLookup[0x0D] = &Intel8080::opDCR_C;          // DCR C instruction
    pOpcodeLookup[0x0E] = &Intel8080::opMVI_C_D8;       // MVI C,D8 instruction
    pOpcodeLookup[0x0F] = &Intel8080::opRRC;            // RRC instruction
    pOpcodeLookup[0x11] = &Intel8080::opLXI_D_D16;      // LXI D,D16 instruction
    pOpcodeLookup[0x13] = &Intel8080::opINX_D;          // INX D instruction

    pOpcodeLookup[0x19] = &Intel8080::opDAD_D;          // DAD D instruction
    pOpcodeLookup[0x1A] = &Intel8080::opLDAX_D;         // LDAX D instruction

    pOpcodeLookup[0x21] = &Intel8080::opLXI_H_D16;      // LXI H,D16 instruction

    pOpcodeLookup[0x23] = &Intel8080::opINX_H;          // INX H instruction

    pOpcodeLookup[0x26] = &Intel8080::opMVI_H_D8;       // MVI H,D8 instruction

    pOpcodeLookup[0x29] = &Intel8080::opDAD_H;          // DAD H instruction

    pOpcodeLookup[0x31] = &Intel8080::opLXI_SP_D16;     // LXI SP,D16 instruction
    pOpcodeLookup[0x32] = &Intel8080::opSTA;            // STA instruction

    pOpcodeLookup[0x36] = &Intel8080::opMVI_M_D8;       // MVI M,D8 instruction

    pOpcodeLookup[0x3A] = &Intel8080::opLDA;            // LDA instruction

    pOpcodeLookup[0x3E] = &Intel8080::opMVI_A_D8;       // MOV M,A instruction

    pOpcodeLookup[0x56] = &Intel8080::opMOV_D_M;        // MOV D, M instruction


    pOpcodeLookup[0x5E] = &Intel8080::opMOV_E_M;        // MOV E, M instruction
    
    pOpcodeLookup[0x66] = &Intel8080::opMOV_H_M;        // MOV H, M instruction

    pOpcodeLookup[0x6F] = &Intel8080::opMOV_L_A;        // MOV L, A instruction

    pOpcodeLookup[0x77] = &Intel8080::opMOV_M_A;        // MOV M, A instruction

    pOpcodeLookup[0x7A] = &Intel8080::opMOV_A_D;        // MOV A, D instruction
    pOpcodeLookup[0x7B] = &Intel8080::opMOV_A_E;        // MOV A, E instruction
    pOpcodeLookup[0x7C] = &Intel8080::opMOV_A_H;        // MOV A, H instruction

    pOpcodeLookup[0x7E] = &Intel8080::opMOV_A_M;        // MOV A, M instruction

    pOpcodeLookup[0xA7] = &Intel8080::opANA_A;         // ANA A instruction

    pOpcodeLookup[0xAF] = &Intel8080::opXRA_A;          // XRA A instruction

    pOpcodeLookup[0xC1] = &Intel8080::opPOP_B;          // POP B instruction
    pOpcodeLookup[0xC2] = &Intel8080::opJNZ;            // JNZ instruction
    pOpcodeLookup[0xC3] = &Intel8080::opJMP;            // JMP instruction

    pOpcodeLookup[0xC5] = &Intel8080::opPUSH_B;         // PUSH B instruction
    pOpcodeLookup[0xC6] = &Intel8080::opADI_D8;         // ADI D8 instruction


    pOpcodeLookup[0xCD] = &Intel8080::opCALL;           // CALL instruction

    pOpcodeLookup[0xC9] = &Intel8080::opRET;            // RET instruction

    pOpcodeLookup[0xD1] = &Intel8080::opPOP_D;          // POP D instruction

    pOpcodeLookup[0xD3] = &Intel8080::opOUT_D8;         // OUT D8 instruction

    pOpcodeLookup[0xD5] = &Intel8080::opPUSH_D;         // PUSH D instruction

    pOpcodeLookup[0xE1] = &Intel8080::opPOP_H;          // POP H instruction

    pOpcodeLookup[0xE5] = &Intel8080::opPUSH_H;         // PUSH H instruction
    pOpcodeLookup[0xE6] = &Intel8080::opANI_D8;         // ANI D8 instruction

    pOpcodeLookup[0xEB] = &Intel8080::opXCHG;           // XCHG instruction

    pOpcodeLookup[0xF1] = &Intel8080::opPOP_PSW;        // POP PSW instruction

    pOpcodeLookup[0xF5] = &Intel8080::opPUSH_PSW;       // PUSH PSW instruction

    pOpcodeLookup[0xFB] = &Intel8080::opEI;             // EI instruction

    pOpcodeLookup[0xFE] = &Intel8080::opCPI_D8;         // CPI instruction


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
    char blah;
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

    byteData = regs.a & 0x01;
    regs.a = (regs.a >> 1) | (byteData << 7);
    regs.f.cy = byteData & 0xFF ? 1 : 0;
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


void Intel8080::opMOV_L_A()
{
    // Opcode: 0x6F         Mmnemonic: MOV L, A
    // Size: 1  byte        Cycles: 5
    // Description: Move contents of Accumulator into L register
    // Flags: None

    regs.l = regs.a;
    spdlog::debug("MOV L,A -> L: 0x{:02X} A: 0x{:02X}", regs.l, regs.a);
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



void Intel8080::opPOP_B()
{
    // Opcode: 0xC1         Mnemonic: POP B
    // Size: 1 byte         Cycles: 10
    // Description: Pop BC register pair from the stack.
    // Flags: None 

    /*readByte(regs.sp);
    regs.sp++;
    regs.c = byteData;
    readByte(regs.sp);
    regs.b = byteData;
    regs.sp++;*/
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

void Intel8080::opPUSH_B()
{
    // Opcode: 0xC5         Mnemonic: PUSH B
    // Size: 1  byte        Cycles: 11
    // Description: Push BC register pair onto the stack.
    // Flags: None

    /*regs.sp--;
    writeByte(regs.sp, regs.b);
    regs.sp--;
    writeByte(regs.sp, regs.c);*/
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


void Intel8080::opRET()
{
    // Opcode: 0xC9         Mnemonic: RET
    // Size: 1  byte        Cycles: 10
    // Description: Return from subroutine
    // Flags: None  

    // Pop return address from stack
    readWord(regs.sp);
    regs.pc = wordData;
    regs.sp += 2;
    spdlog::debug("RET -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
}

void Intel8080::opCALL()
{
    // Opcode: 0xCD         Mnemonic: CALL addr
    // Size: 3  bytes       Cycles: 17
    // Description: Call subroutine at address
    // Flags: None  

    fetchWord();
    // Push current PC onto stack
    regs.sp -= 2;
    writeWord(regs.sp, regs.pc);
    // Set PC to target address
    regs.pc = wordData;
    spdlog::debug("CALL -> PC: 0x{:04X} SP: 0x{:04X}", regs.pc, regs.sp);
}  

void Intel8080::opPOP_D()
{
    // Opcode: 0xD1         Mnemonic: POP D
    // Size: 1  byte        Cycles: 10
    // Description: Pop DE register pair from the stack.
    // Flags: None  

    /*readByte(regs.sp++);
    regs.e = byteData;
    readByte(regs.sp++);
    regs.d = byteData;*/
    readWord(regs.sp);
    regs.de = wordData;
    regs.sp += 2;
    spdlog::debug("POP D -> SP: 0x{:04X}", regs.sp);
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


void Intel8080::opPUSH_D()
{
    // Opcode: 0xD5         Mnemonic: PUSH D
    // Size: 1  byte        Cycles: 11
    // Description: Push DE register pair onto the stack.
    // Flags: None  

    /*regs.sp--;
    writeByte(regs.sp, regs.d);
    regs.sp--;
    writeByte(regs.sp, regs.e);*/
    regs.sp -= 2;
    writeWord(regs.sp, regs.de);
    spdlog::debug("PUSH D -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opPOP_H()
{
    // Opcode: 0xE1         Mnemonic: POP H
    // Size: 1  byte        Cycles: 10
    // Description: Pop HL register pair from the stack.
    // Flags: None  

    /*readByte(regs.sp);
    regs.l = byteData;
    regs.sp++;
    readByte(regs.sp);
    regs.h = byteData;
    regs.sp++;*/
    readWord(regs.sp);
    regs.hl = wordData;
    regs.sp += 2;
    spdlog::debug("POP H -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opPUSH_H()
{
    // Opcode: 0xE5         Mnemonic: PUSH H
    // Size: 1  byte        Cycles: 11
    // Description: Push HL register pair onto the stack.
    // Flags: None  

    /*regs.sp--;
    writeByte(regs.sp, regs.h);
    regs.sp--;
    writeByte(regs.sp, regs.l);*/
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

void Intel8080::opPOP_PSW()
{
    // Opcode: 0xF1         Mnemonic: POP PSW
    // Size: 1  byte        Cycles: 10
    // Description: Pop stack to PSW
    // Flags: S, Z, AC, P, CY

    /*readByte(regs.sp++);
    regs.f.flags = byteData;
    readByte(regs.sp++);
    regs.a = byteData;

    flags.s = (flagbyte & 0x80) ? 1 : 0;
    flags.z = (flagbyte & 0x40) ? 1 : 0;
    flags.xZero = (flagbyte & 0x20) ? 1 : 0;
    flags.ac = (flagbyte & 0x10) ? 1 : 0;
    flags.yZero = (flagbyte & 0x08) ? 1 : 0;
    flags.p = (flagbyte & 0x04) ? 1 : 0;
    flags.xOne = (flagbyte & 0x02) ? 1 : 0;
    flags.cy = (flagbyte & 0x01) ? 1 : 0;*/
    readWord(regs.sp);
    regs.af = wordData;
    regs.sp += 2;

    spdlog::debug("POP PSW -> A: 0x{:02X} SP: 0x{:04X}", regs.a, regs.sp);
}


void Intel8080::opPUSH_PSW()
{
    // Opcode: 0xF5         Mnemonic: PUSH PSW
    // Size: 1  byte        Cycles: 11
    // Description: Push PSW onto the stack.
    // Flags: None  

    /*writeByte(--regs.sp, regs.a);
    BYTE flagbyte = 0x00;
    flagbyte |= (flags.s << 7);
    flagbyte |= (flags.z << 6);
    flagbyte |= (flags.xZero << 5);
    flagbyte |= (flags.ac << 4);
    flagbyte |= (flags.yZero << 3);
    flagbyte |= (flags.p << 2);
    flagbyte |= (flags.xOne << 1);
    flagbyte |= (flags.cy << 0);
    writeByte(--regs.sp, regs.f.flags);*/
    regs.sp -= 2;
    writeWord(regs.sp, regs.af);
    spdlog::debug("PUSH PSW -> SP: 0x{:04X}", regs.sp);
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

    regs.f.ac = 0x00;


}
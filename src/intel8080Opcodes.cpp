#include <spdlog/spdlog.h>
#include "types.h"
#include "intel8080.h"

void Intel8080::buildOpcodeTable()
{
    pOpcodeLookup.fill(&Intel8080::opILLEGAL); // Initialize with a dummy opcode
    pOpcodeLookup[0x00] = &Intel8080::opNOP; // NOP instruction
    pOpcodeLookup[0x01] = &Intel8080::opLXI_B_D16; // LXI B,D16 instruction
    pOpcodeLookup[0x02] = &Intel8080::opSTAX_B; // STAX B instruction
    pOpcodeLookup[0x03] = &Intel8080::opINX_B; // INX B instructrion
    pOpcodeLookup[0x04] = &Intel8080::opINR_B; // INR B instruction
    pOpcodeLookup[0x05] = &Intel8080::opDCR_B; // DCR B instruction
    pOpcodeLookup[0x06] = &Intel8080::opMVI_B_D8; // MVI B,D8 instruction
    pOpcodeLookup[0x09] = &Intel8080::opDAD_B; // DAD B instruction
    pOpcodeLookup[0x0D] = &Intel8080::opDCR_C; // DCR C instruction
    pOpcodeLookup[0x0E] = &Intel8080::opMVI_C_D8; // MVI C,D8 instruction
    pOpcodeLookup[0x0F] = &Intel8080::opRRC; // RRC instruction
    pOpcodeLookup[0x11] = &Intel8080::opLXI_D_D16; // LXI D,D16 instruction
    pOpcodeLookup[0x13] = &Intel8080::opINX_D; // INX D instruction

    pOpcodeLookup[0x19] = &Intel8080::opDAD_D; // DAD D instruction
    pOpcodeLookup[0x1A] = &Intel8080::opLDAX_D; // LDAX D instruction

    pOpcodeLookup[0x21] = &Intel8080::opLXI_H_D16; // LXI H,D16 instruction

    pOpcodeLookup[0x23] = &Intel8080::opINX_H; // INX H instruction

    pOpcodeLookup[0x26] = &Intel8080::opMVI_H_D8; // MVI H,D8 instruction

    pOpcodeLookup[0x29] = &Intel8080::opDAD_H; // DAD H instruction

    pOpcodeLookup[0x31] = &Intel8080::opLXI_SP_D16; // LXI SP,D16 instruction

    pOpcodeLookup[0x36] = &Intel8080::opMVI_M_D8; // MVI M,D8 instruction

    pOpcodeLookup[0x6F] = &Intel8080::opMOV_L_A; // MOV L,A instruction

    pOpcodeLookup[0x77] = &Intel8080::opMOV_M_A; // MOV M,A instruction

    pOpcodeLookup[0x7C] = &Intel8080::opMOV_A_H; // MOV A,H instruction

    pOpcodeLookup[0xC1] = &Intel8080::opPOP_B; // POP B instruction
    pOpcodeLookup[0xC2] = &Intel8080::opJNZ; // JNZ instruction
    pOpcodeLookup[0xC3] = &Intel8080::opJMP; // JMP instruction

    pOpcodeLookup[0xC5] = &Intel8080::opPUSH_B; // PUSH B instruction

    pOpcodeLookup[0xCD] = &Intel8080::opCALL; // CALL instruction

    pOpcodeLookup[0xC9] = &Intel8080::opRET; // RET instruction

    pOpcodeLookup[0xD3] = &Intel8080::opOUT_D8; // OUT D8 instruction

    pOpcodeLookup[0xD5] = &Intel8080::opPUSH_D; // PUSH D instruction

    pOpcodeLookup[0xE1] = &Intel8080::opPOP_H; // POP H instruction

    pOpcodeLookup[0xE5] = &Intel8080::opPUSH_H; // PUSH H instruction

    pOpcodeLookup[0xEB] = &Intel8080::opXCHG; // XCHG instruction

    pOpcodeLookup[0xFE] = &Intel8080::opCPI_D8; // CPI instruction


    // ... Add other opcode mappings here       

}

void Intel8080::regFlagsAuxCarry(WORD ops)
{
    BYTE operand1 = BYTE((ops & 0xFF00) >> 8);
    BYTE operand2 = (BYTE)(ops & 0x00FF);

    BYTE lowNibbleOp1 = (BYTE)(operand1 & 0x0F);
    BYTE lowNibbleOp2 = (BYTE)(operand2 & 0x0F);



    if (((operand1 & 0x0F) + (operand2 & 0x0F)) > 0x0F) {
        flags.ac = 1;
    } else {
        flags.ac = 0;
    }
}

void Intel8080::regFlagsCarry(WORD ops)
{
    BYTE operand1 = BYTE((ops & 0xFF00) >> 8);
    BYTE operand2 = (BYTE)(ops & 0x00FF);

    if ((operand1 + operand2) > 0xFF) {
        flags.cy = 1;
    } else {
        flags.cy = 0;
    }
}

void Intel8080::regFlagsDoubleCarry(WORD op1, WORD op2)
{   
    uint32_t result = op1 + op2;
    if (result > 0xFFFF) {
        flags.cy = 1;
    } else {
        flags.cy = 0;
    }
}

void Intel8080::regFlagsBasic(BYTE result)
{
    flags.s = (result & 0x80) ? 1 : 0;
    flags.z = (result == 0) ? 1 : 0;
    flags.p = __builtin_popcount(result) % 2 == 0;
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
    regs.b = (wordData >> 8) & 0x00FF;
    regs.c = wordData & 0x00FF;
    spdlog::debug("LXI B, D16 -> B: 0x{:02X} C: 0x{:02X} D16: 0x{:04X}", regs.b, regs.c, wordData);
}

void Intel8080::opSTAX_B()
{
    // Opcode: 0x02         Mnemonic: STAX B
    // Size: 1  byte        Cycles: 7
    // Description: Store Accumulator into memory location pointed by BC register pair
    // Flags: None

    wordData = (static_cast<WORD>(regs.b) << 8) | static_cast<WORD>(regs.c);
    writeByte(wordData, regs.a);
    spdlog::debug("STAX B -> [0x{:04X}] = 0x{:02X}", wordData, regs.a);
}   

void Intel8080::opINX_B()
{
    // Opcode: 0x03         Mnemonic: INX B
    // Size: 1 byte         Cycles: 5
    // Description: Increment the BC register pair
    // Flags: None

    wordData = (static_cast<WORD>(regs.b) << 8) | static_cast<WORD>(regs.c);
    wordData += 1;
    regs.b = (wordData >> 8) & 0xFF;
    regs.c = wordData & 0xFF;
    spdlog::debug("INX B -> B: 0x{:02X} C: 0x{:02X} BC: 0x{:04X}", regs.b, regs.c, wordData);
}

void Intel8080::opINR_B()
{
    // Opcode: 0x04         Mnemonic: INR B
    // Size: 1              Cycles: 5
    // Description: Increment the B register
    // Flags: S, Z, AC, P

    wordData = 0;

    wordData = ((WORD)regs.b << 8) & 0xFF00;
    wordData += 0x01;
    regs.b++;
    regFlagsBasic(regs.b);
    regFlagsAuxCarry(wordData);
    spdlog::debug("INR B -> B: 0x{:02X}", regs.b);
}

void Intel8080::opDCR_B()
{
    // Opcode: 0x05         Mnemonic: DCR B
    // Size: 1              Cycles: 5
    // Description: Decrement the B register
    // Flags: S, Z, AC, P

    wordData = 0;

    wordData = ((WORD)regs.b << 8) & 0xFF00;
    wordData += 0xFF; // 0xFF is -1 two's complement
    regs.b--;
    regFlagsBasic(regs.b);
    regFlagsAuxCarry(wordData);
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

    WORD hl = (static_cast<WORD>(regs.h) << 8) | static_cast<WORD>(regs.l);
    WORD bc = (static_cast<WORD>(regs.b) << 8) | static_cast<WORD>(regs.c);
    WORD result = hl + bc;
    regs.h = (result >> 8) & 0xFF;
    regs.l = result & 0xFF;
    regFlagsDoubleCarry(hl, bc);
    spdlog::debug("DAD B -> HL: 0x{:04X} BC: 0x{:04X} -> 0x{:04X}", hl, bc, result);
}

void Intel8080::opDCR_C()
{
    // Opcode: 0x0D         Mnemonic: DCR C
    // Size: 1              Cycles: 5
    // Description: Decrement the C register
    // Flags: S, Z, AC, P

    wordData = 0;

    wordData = ((WORD)regs.c << 8) & 0xFF00;
    wordData += 0xFF; // 0xFF is -1 in two's complement
    regs.c--;
    regFlagsBasic(regs.c);
    regFlagsAuxCarry(wordData);
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
    flags.cy = byteData & 0xFF ? 1 : 0;
    spdlog::debug("RRC -> A: 0x{:02X}", regs.a);
}   

void Intel8080::opLXI_D_D16()
{
    // Opcode: 0x11         Mnemonic: LXI D,D16
    // Size: 3  bytes       Cycles: 10
    // Description: Load immediate 16-bit data into DE register pair
    // Flags: None

    fetchWord();
    regs.d = (wordData >> 8) & 0x00FF;
    regs.e = wordData & 0x00FF;
    spdlog::debug("LXI D, D16 -> D: 0x{:02X} E: 0x{:02X} D16: 0x{:04X}", regs.d, regs.e, wordData);
}   

void Intel8080::opINX_D()
{
    // Opcode: 0x13         Mnemonic: INX D
    // Size: 1 byte         Cycles: 5
    // Description: Increment the DE register pair
    // Flags: None

    wordData = (static_cast<WORD>(regs.d) << 8) | static_cast<WORD>(regs.e);
    wordData += 1;
    regs.d = (wordData >> 8) & 0xFF;
    regs.e = wordData & 0xFF;
    spdlog::debug("INX D -> D: 0x{:02X} E: 0x{:02X} DE: 0x{:04X}", regs.d, regs.e, wordData);
}

void Intel8080::opDAD_D()
{
    // Opcode: 0x19         Mnemonic: DAD D
    // Size: 1              Cycles: 10
    // Description: Add the contents of the register pair DE to the register pair HL.
    // Flags: CY

    WORD hl = (static_cast<WORD>(regs.h) << 8) | static_cast<WORD>(regs.l);
    WORD de = (static_cast<WORD>(regs.d) << 8) | static_cast<WORD>(regs.e);
    WORD result = hl + de;
    regs.h = (result >> 8) & 0xFF;
    regs.l = result & 0xFF;
    regFlagsDoubleCarry(hl, de);
    spdlog::debug("DAD D -> HL: 0x{:04X} DE: 0x{:04X} -> 0x{:04X}", hl, de, result);
}

void Intel8080::opLDAX_D()
{
    // Opcode: 0x1A         Mnemonic: LDAX D
    // Size: 1              Cycles: 7
    // Description: Load Accumulator from memory location pointed by DE register pair
    // Flags: None

    wordData = (static_cast<WORD>(regs.d) << 8) | static_cast<WORD>(regs.e);
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
    regs.h = (wordData >> 8) & 0x00FF;
    regs.l = wordData & 0x00FF;
    spdlog::debug("LXI H, D16 -> H: 0x{:02X} L: 0x{:02X} D16: 0x{:04X}", regs.h, regs.l, wordData);
}

void Intel8080::opINX_H()
{
    // Opcode: 0x23         Mnemonic: INX H
    // Size: 1 byte         Cycles: 5
    // Description: Increment the HL register pair
    // Flags: None

    wordData = (static_cast<WORD>(regs.h) << 8) | static_cast<WORD>(regs.l);
    wordData += 1;
    regs.h = (wordData >> 8) & 0xFF;
    regs.l = wordData & 0xFF;
    spdlog::debug("INX H -> H: 0x{:02X} L: 0x{:02X} HL: 0x{:04X}", regs.h, regs.l, wordData);
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

    WORD hl = (static_cast<WORD>(regs.h) << 8) | static_cast<WORD>(regs.l);
    WORD result = hl + hl;
    regs.h = (result >> 8) & 0xFF;
    regs.l = result & 0xFF;
    regFlagsDoubleCarry(hl, hl);
    spdlog::debug("DAD H -> HL: 0x{:04X} HL: 0x{:04X} -> 0x{:04X}", hl, hl, result);
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

void Intel8080::opMVI_M_D8()
{
    // Opcode: 0x36         Mnemonic: MVI M, D8
    // Size: 2  bytes       Cycles: 7
    // Description: Move immediate byte data into the memory address pointed by HL register pair
    // Flags: None

    WORD hl = (static_cast<WORD>(regs.h) << 8) | static_cast<WORD>(regs.l);
    fetchByte();
    writeByte(hl, byteData);
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

    WORD hl = (static_cast<WORD>(regs.h) << 8) | static_cast<WORD>(regs.l);
    writeByte(hl, regs.a);
    spdlog::debug("MOV M,A -> [0x{:04X}] = 0x{:02X}", hl, regs.a);
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

void Intel8080::opPOP_B()
{
    // Opcode: 0xC1         Mnemonic: POP B
    // Size: 1 byte         Cycles: 10
    // Description: Pop BC register pair from the stack.
    // Flags: None 

    readByte(regs.sp);
    regs.sp++;
    regs.b = byteData;
    readByte(regs.sp);
    regs.c = byteData;
    regs.sp++;
    spdlog::debug("POP B -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opJNZ()
{
    // Opcode: 0xC2         Mnemonic: JNZ addr
    // Size: 3  bytes       Cycles: 10/11
    // Description: Jump to address if Zero flag is not set
    // Flags: None

    fetchWord();
    if (flags.z == 0) {
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

    regs.sp--;
    writeByte(regs.sp, regs.b);
    regs.sp--;
    writeByte(regs.sp, regs.c);
    spdlog::debug("PUSH B -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opCALL()
{
    // Opcode: 0xCD         Mnemonic: CALL addr
    // Size: 3  bytes       Cycles: 23
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

    regs.sp--;
    writeByte(regs.sp, regs.d);
    regs.sp--;
    writeByte(regs.sp, regs.e);
    spdlog::debug("PUSH D -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opPOP_H()
{
    // Opcode: 0xE1         Mnemonic: POP H
    // Size: 1  byte        Cycles: 10
    // Description: Pop HL register pair from the stack.
    // Flags: None  

    readByte(regs.sp);
    regs.l = byteData;
    regs.sp++;
    readByte(regs.sp);
    regs.h = byteData;
    regs.sp++;
    spdlog::debug("POP H -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opPUSH_H()
{
    // Opcode: 0xE5         Mnemonic: PUSH H
    // Size: 1  byte        Cycles: 11
    // Description: Push HL register pair onto the stack.
    // Flags: None  

    regs.sp--;
    writeByte(regs.sp, regs.h);
    regs.sp--;
    writeByte(regs.sp, regs.l);
    spdlog::debug("PUSH H -> SP: 0x{:04X}", regs.sp);
}

void Intel8080::opXCHG()
{
    // Opcode: 0xEB         Mnemonic: XCHG
    // Size: 1  byte        Cycles: 5
    // Description: Exchange the HL and DE register pairs
    // Flags: None  

    byteData = regs.d;
    regs.d = regs.h;
    regs.h = byteData;
    byteData = regs.e;
    regs.e = regs.l;
    regs.l = byteData;

    spdlog::debug("XCHG -> DE: 0x{:04X} HL: 0x{:04X}", (regs.d << 8) | regs.e, (regs.h << 8) | regs.l);
}



void Intel8080::opCPI_D8()
{
    // Opcode:  0xFE        Mnemonic: CPI D8
    // Size: 2 bytes        Cycles: 7
    // Description: Compare register A with the immediate mode BYTE data. Set flags.
    // Flags: S, Z, AC, P, CY


    fetchByte();
    BYTE result = regs.a - byteData;
    regFlagsBasic(result);
    WORD operands = (static_cast<WORD>(regs.a) << 8) | static_cast<WORD>(byteData);
    regFlagsCarry(operands);
    regFlagsAuxCarry(operands);

}

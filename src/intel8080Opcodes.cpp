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

    pOpcodeLookup[0x1A] = &Intel8080::opLDAX_D; // LDAX D instruction

    pOpcodeLookup[0x21] = &Intel8080::opLXI_H_D16; // LXI H,D16 instruction

    pOpcodeLookup[0x23] = &Intel8080::opINX_H; // INX H instruction

    pOpcodeLookup[0x31] = &Intel8080::opLXI_SP_D16; // LXI SP,D16 instruction

    pOpcodeLookup[0x77] = &Intel8080::opMOV_M_A; // MOV M,A instruction

    pOpcodeLookup[0xC2] = &Intel8080::opJNZ; // JNZ instruction
    pOpcodeLookup[0xC3] = &Intel8080::opJMP; // JMP instruction

    pOpcodeLookup[0xCD] = &Intel8080::opCALL; // CALL instruction

    pOpcodeLookup[0xC9] = &Intel8080::opRET; // RET instruction

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
    if (ops < wordData) {
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
    printf("Opcode: 0x%02X\n", opcode);
    printf("Registers: A: 0x%02X B: 0x%02X C: 0x%02X D: 0x%02X E: 0x%02X H: 0x%02X L: 0x%02X SP: 0x%04X PC: 0x%04X\n",
           regs.a, regs.b, regs.c, regs.d, regs.e, regs.h, regs.l, regs.sp, regs.pc);
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
    wordData = hl;
    WORD result = hl + bc;
    hl = result;
    regs.h = (hl >> 8) & 0xFF;
    regs.l = hl & 0xFF;
    regFlagsCarry(result); // This probably needs to be fixed to take the operands rather than the result.
    spdlog::debug("DAD B -> HL: 0x{:04X} BC: 0x{:04X} -> 0x{:04X}", wordData, bc, result);
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

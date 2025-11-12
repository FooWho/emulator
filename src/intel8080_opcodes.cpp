#include <spdlog/spdlog.h>
#include "types.h"
#include "intel8080.h"

void Intel8080::buildOpcodeTable()
{
    p_opcode_lookup.fill(&Intel8080::op_ILLEGAL); // Initialize with a dummy opcode
    p_opcode_lookup[0x00] = &Intel8080::op_NOP; // NOP instruction
    p_opcode_lookup[0x01] = &Intel8080::op_LXI_B_D16; // LXI B,D16 instruction
    p_opcode_lookup[0x02] = &Intel8080::op_STAX_B; // STAX B instruction
    p_opcode_lookup[0x03] = &Intel8080::op_INX_B; // INX B instructrion
    p_opcode_lookup[0x04] = &Intel8080::op_INR_B; // INR B instruction
    p_opcode_lookup[0x05] = &Intel8080::op_DCR_B; // DCR B instruction
    p_opcode_lookup[0x06] = &Intel8080::op_MVI_B_D8; // MVI B,D8 instruction
    p_opcode_lookup[0x09] = &Intel8080::op_DAD_B; // DAD B instruction
    p_opcode_lookup[0x0D] = &Intel8080::op_DCR_C; // DCR C instruction

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



void Intel8080::op_ILLEGAL()
{
    spdlog::debug("ILLEGAL");
    throw std::runtime_error("Illegal opcode executed");
}

void Intel8080::op_NOP()
{
    // Opcode: 0x00         Mnemonic: NOP
    // Size: 1  byte        Cycles: 4
    // Description: No Operation
    // Flags: None

    spdlog::debug("NOP");
}

void Intel8080::op_LXI_B_D16()
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

void Intel8080::op_STAX_B()
{
    // Opcode: 0x02         Mnemonic: STAX B
    // Size: 1  byte        Cycles: 7
    // Description: Store Accumulator into memory location pointed by BC register pair
    // Flags: None

    WORD address = (static_cast<WORD>(regs.b) << 8) | static_cast<WORD>(regs.c);
    writeByte(address, regs.a);
    spdlog::debug("STAX B -> [0x{:04X}] = 0x{:02X}", address, regs.a);
}   

void Intel8080::op_INX_B()
{
    // Opcode: 0x03         Mnemonic: INX B
    // Size: 1 byte         Cycles: 5
    // Description: Increment the BC register pair
    // Flags: None

    WORD bc = (static_cast<WORD>(regs.b) << 8) | static_cast<WORD>(regs.c);
    bc += 1;
    regs.b = (bc >> 8) & 0xFF;
    regs.c = bc & 0xFF;
    spdlog::debug("INX B -> B: 0x{:02X} C: 0x{:02X} BC: 0x{:04X}", regs.b, regs.c, bc);
}

void Intel8080::op_INR_B()
{
    // Opcode: 0x04         Mnemonic: INR B
    // Size: 1              Cycles: 5
    // Description: Increment the B register
    // Flags: S, Z, AC, P

    WORD operands = 0;

    operands = ((WORD)regs.b << 8) & 0xFF00;
    operands += 0x01;
    regs.b++;
    regFlagsBasic(regs.b);
    regFlagsAuxCarry(operands);
    spdlog::debug("INR B -> B: 0x{:02X}", regs.b);
}

void Intel8080::op_DCR_B()
{
    // Opcode: 0x05         Mnemonic: DCR B
    // Size: 1              Cycles: 5
    // Description: Decrement the B register
    // Flags: S, Z, AC, P

    WORD operands = 0;

    operands = ((WORD)regs.b << 8) & 0xFF00;
    operands += 0xFF; // 0xFF is -1 two's complement
    regs.b--;
    regFlagsBasic(regs.b);
    regFlagsAuxCarry(operands);
    spdlog::debug("DCR B -> B: 0x{:02X}", regs.b);
}

void Intel8080::op_MVI_B_D8()
{
    // Opcode: 0x06          Mnemonic: MVI B, D8
    // Size: 2  bytes       Cycles: 7
    // Description: Move immediate 8-bit data into B
    // Flags: None

    fetchByte();
    regs.b = byteData;
    spdlog::debug("MVI B, D8 -> B: 0x{:02X} D8: 0x{:02X}", regs.b, byteData);
}

void Intel8080::op_DAD_B()
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
    regFlagsCarry(result);
    spdlog::debug("DAD B -> HL: 0x{:04X} BC: 0x{:04X} -> 0x{:04X}", wordData, bc, result);
}

void Intel8080::op_DCR_C()
{
    // Opcode: 0x0D         Mnemonic: DCR C
    // Size: 1              Cycles: 5
    // Description: Decrement the C register
    // Flags: S, Z, AC, P

    WORD operands = 0;

    operands = ((WORD)regs.c << 8) & 0xFF00;
    operands += 0xFF; // 0xFF is -1 in two's complement
    regs.c--;
    regFlagsBasic(regs.c);
    regFlagsAuxCarry(operands);
    spdlog::debug("DCR C -> C: 0x{:02X}", regs.c);
}


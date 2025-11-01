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

}

void Intel8080::regFlagsAuxCarry(BYTE original, BYTE result)
{
    if (original > result) {
        // subtraction
        flags.ac = ((BYTE(original & 0x0f)) <  (BYTE(result & 0x0f)));
    }
    if (original < result) {
        // addition
        flags.ac = ((BYTE(original & 0x0f)) >  (BYTE(result & 0x0f)));
    }
    if (original == result) {
        // Something happened with 0 as an operand
        flags.ac = 0;
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
    BYTE result = regs.b + 1;
    regFlagsBasic(result);
    regFlagsAuxCarry(regs.b, result);
    regs.b = result;
    spdlog::debug("INR B -> B: 0x{:02X}", regs.b);
}

void Intel8080::op_DCR_B()
{
    // Opcode: 0x04         Mnemonic: DCR B
    // Size: 1              Cycles: 5
    // Description: Decrement the B register
    // Flags: S, Z, AC, P
    BYTE result = regs.b - 1;
    regFlagsBasic(result);
    regFlagsAuxCarry(regs.b, result);
    regs.b = result;
    spdlog::debug("INR B -> B: 0x{:02X}", regs.b);
}


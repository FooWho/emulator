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
    BYTE low = fetchByte();
    BYTE high = fetchByte();
    regs.b = high;
    regs.c = low;
    spdlog::debug("LXI B, D16 -> B: 0x{:02X} C: 0x{:02X} D16: 0x{:04X}", regs.b, regs.c, (static_cast<WORD>((high) << 8) | static_cast<WORD>(low)));
}

void Intel8080::op_STAX_B()
{
    // Opcode: 0x02         Mnemonic: STAX B
    // Size: 1  byte        Cycles: 7
    // Description: Store Accumulator into memory location pointed by BC register pair
    // Flags: None
    WORD address = (static_cast<WORD>(regs.b) << 8) | static_cast<WORD>(regs.c);
    writeByte(address, regs.a);
    spdlog::debug("Register A: {0:X}", regs.a);
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
    //spdlog::trace(INX B -> )
}
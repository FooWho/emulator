#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include "intel8080.h"
#include "ram.h"
#include "rom.h"
#include "bus.h"


TEST(emuTest, cpuTestPC) {
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM

    const std::vector<BYTE> buffer = {0xff, 0x00, 0x34, 0x12};
    rom.romLoad(buffer);

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));

    BYTE opcode;
    BYTE byteData;
    WORD wordData;

    EXPECT_EQ(cpu.examineRegisterPC(), 0x0000); // Initial PC should be 0

    opcode = cpu.fetchOpcode(); // Fetch first opcode
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0001); // PC should increment by 1
}

TEST(emuTest, cpuTestIllegalOpcode) {
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM

    const std::vector<BYTE> buffer = {0xff, 0x00, 0x01, 0x34, 0x12};
    rom.romLoad(buffer);
    

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));

    BYTE opcode;
    BYTE byteData;
    WORD wordData;

    opcode = cpu.fetchOpcode(); // Fetch first opcode
    EXPECT_EQ(opcode, 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(0xff), std::runtime_error);
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0001); // PC should increment by 1
}

TEST(emuTest, cpuTestNOP) {
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM

    const std::vector<BYTE> buffer = {0xff, 0x00, 0x01, 0x34, 0x12};
    rom.romLoad(buffer);

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));

    BYTE opcode;
    BYTE byteData;
    WORD wordData;

    opcode = cpu.fetchOpcode(); // Fetch first opcode
    EXPECT_EQ(opcode, 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(0xff), std::runtime_error);
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0001); // PC should increment by 1

    opcode = cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(opcode, 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_NO_THROW(cpu.execute(0x00)); // Execute NOP instruction
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0002); // PC should increment by 1
}

TEST(emuTest, cpuTestLXI_B_D16) {
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM
    const std::vector<BYTE> buffer = {0xff, 0x00, 0x01, 0x34, 0x12};
    rom.romLoad(buffer);

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));

    BYTE opcode;
    BYTE byteData;
    WORD wordData;

    opcode = cpu.fetchOpcode(); // Fetch first opcode
    EXPECT_EQ(opcode, 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(0xff), std::runtime_error);
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0001); // PC should increment by 1

    opcode = cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(opcode, 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_NO_THROW(cpu.execute(0x00)); // Execute NOP instruction
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0002); // PC should increment by 1

    opcode = cpu.fetchOpcode(); // Should fetch LXI B,D16 instruction (0x01)
    EXPECT_EQ(opcode, 0x01);
    cpu.execute(opcode); // Execute LXI B,D16 instruction
    EXPECT_EQ(cpu.examineRegisterBC(), 0x1234); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(cpu.examineRegisterB(), 0x12); // High byte
    EXPECT_EQ(cpu.examineRegisterC(), 0x34); // Low byte
    EXPECT_EQ(cpu.readByte(0x0003), 0x34); // Low byte
    EXPECT_EQ(cpu.readByte(0x0004), 0x12); // High byte
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0005); // PC should point to next instruction
}

TEST(emuTest, emuTest_cpuTestSTAX_B) {
    spdlog::set_level(spdlog::level::trace);
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM
    const std::vector<BYTE> buffer = {0xff, 0x00, 0x01, 0x34, 0x12, 0x02};
    rom.romLoad(buffer);

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));

    BYTE opcode;
    BYTE byteData;
    WORD wordData;

    opcode = cpu.fetchOpcode(); // Fetch first opcode
    EXPECT_EQ(opcode, 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(0xff), std::runtime_error);
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0001); // PC should increment by 1

    opcode = cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(opcode, 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_NO_THROW(cpu.execute(0x00)); // Execute NOP instruction
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0002); // PC should increment by 1

    opcode = cpu.fetchOpcode(); // Should fetch LXI B,D16 instruction (0x01)
    EXPECT_EQ(opcode, 0x01);
    cpu.execute(opcode); // Execute LXI B,D16 instruction
    EXPECT_EQ(cpu.examineRegisterBC(), 0x1234); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(cpu.examineRegisterB(), 0x12); // High byte
    EXPECT_EQ(cpu.examineRegisterC(), 0x34); // Low byte
    EXPECT_EQ(cpu.readByte(0x0003), 0x34); // Low byte
    EXPECT_EQ(cpu.readByte(0x0004), 0x12); // High byte
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0005); // PC should point to next instruction

    opcode = cpu.fetchOpcode(); // Should fetch STAX B instruction (0x02)
    cpu.setRegisterA(0x56); // Set Accumulator to known value
    cpu.setRegisterBC(0x3124); // Set BC to point to test address
    EXPECT_EQ(opcode, 0x02);
    cpu.execute(opcode); // Execute STAX B instruction
    EXPECT_EQ(cpu.readByte(0x3124), cpu.examineRegisterA()); // Check that memory at address BC contains value of Accumulator
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0006); // PC should point to next instruction
}

TEST(emuTest, emuTest_cpuTestINX_B) {
    spdlog::set_level(spdlog::level::trace);
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM
    const std::vector<BYTE> buffer = {0xff, 0x00, 0x01, 0x34, 0x12, 0x02, 0x03};
    rom.romLoad(buffer);

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));

    BYTE opcode;
    BYTE byteData;
    WORD wordData;

    opcode = cpu.fetchOpcode(); // Fetch first opcode
    EXPECT_EQ(opcode, 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(0xff), std::runtime_error);
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0001); // PC should increment by 1

    opcode = cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(opcode, 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_NO_THROW(cpu.execute(0x00)); // Execute NOP instruction
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0002); // PC should increment by 1

    opcode = cpu.fetchOpcode(); // Should fetch LXI B,D16 instruction (0x01)
    EXPECT_EQ(opcode, 0x01);
    cpu.execute(opcode); // Execute LXI B,D16 instruction
    EXPECT_EQ(cpu.examineRegisterBC(), 0x1234); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(cpu.examineRegisterB(), 0x12); // High byte
    EXPECT_EQ(cpu.examineRegisterC(), 0x34); // Low byte
    EXPECT_EQ(cpu.readByte(0x0003), 0x34); // Low byte
    EXPECT_EQ(cpu.readByte(0x0004), 0x12); // High byte
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0005); // PC should point to next instruction

    opcode = cpu.fetchOpcode(); // Should fetch STAX B instruction (0x02)
    cpu.setRegisterA(0x56); // Set Accumulator to known value
    cpu.setRegisterBC(0x3124); // Set BC to point to test address
    EXPECT_EQ(opcode, 0x02);
    cpu.execute(opcode); // Execute STAX B instruction
    EXPECT_EQ(cpu.readByte(0x3124), cpu.examineRegisterA()); // Check that memory at address BC contains value of Accumulator
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0006); // PC should point to next instruction

    opcode = cpu.fetchOpcode(); // Should fetch INX B instruction (0x03)
    EXPECT_EQ(opcode, 0x03);
    cpu.setRegisterBC(0x3124); // Set BC to a test value
    EXPECT_EQ(cpu.examineRegisterBC(), 0x3124);
    cpu.execute(opcode);
    EXPECT_EQ(cpu.examineRegisterBC(), 0x3125);
}

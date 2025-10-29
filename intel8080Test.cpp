#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include "intel8080.h"
#include "ram.h"
#include "rom.h"
#include "bus.h"


TEST(intel8080Test, cpuTestPC) {
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM

    const std::vector<BYTE> buffer = {0x00, 0x00, 0x00, 0x01, 0x34, 0x12, 0x00};
    rom.romLoad(buffer);

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));

    EXPECT_EQ(cpu.examineRegisterPC(), 0x0000); // Initial PC should be 0

    cpu.fetchOpcode(); // Fetch opcode (0x00)
    cpu.execute(); // Execute opcode
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0001); // PC should increment by 1
    cpu.fetchOpcode(); // Fetch opcode (0x00)
    cpu.execute(); // Execute opcode
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0002); // PC should increment by 1
    cpu.fetchOpcode(); // Fetch opcode (0x00)
    cpu.execute();
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0003); // PC should increment by 1 
    cpu.fetchOpcode(); // Fetch opcode (0x01)
    cpu.execute(); // Execute opcode
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0006); // PC should increment by 3
    cpu.fetchOpcode(); // Fetch opcode (0x00)
    cpu.execute(); // Execute opcode
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0007); // PC should increment by 1
}

TEST(intel8080Test, cpuTestFlags) {
    Intel8080 cpu;
    BYTE result;

    result = 0x00;
    EXPECT_EQ(cpu.examineFlags(), 0x02);
}

TEST(intel8080Test, cpuTestIllegalOpcode) {
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM

    const std::vector<BYTE> buffer = {0x00, 0xff};
    rom.romLoad(buffer);
    

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));

    cpu.fetchOpcode(); // Fetch first opcode
    EXPECT_EQ(cpu.examineOpcode(), 0x00); // Should fetch NOP (0x00)
    EXPECT_NO_THROW(cpu.execute());
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0001); // PC should increment by 1
    cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(cpu.examineOpcode(), 0xff);
    EXPECT_THROW(cpu.execute(), std::runtime_error);
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0002); // PC should increment by 1 (because fetching the opcode already moved the PC)
}

TEST(intel8080Test, cpuTestNOP) {
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

    cpu.fetchOpcode(); // Fetch first opcode
    EXPECT_EQ(cpu.examineOpcode(), 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(), std::runtime_error);
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0001); // PC should increment by 1

    cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(cpu.examineOpcode(), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_NO_THROW(cpu.execute()); // Execute NOP instruction
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0002); // PC should increment by 1
}

TEST(intel8080Test, cpuTestLXI_B_D16) {
    Intel8080 cpu;
    Bus bus;
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM
    const std::vector<BYTE> buffer = {0xff, 0x00, 0x01, 0x34, 0x12};
    rom.romLoad(buffer);

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));

    cpu.fetchOpcode(); // Fetch first opcode
    EXPECT_EQ(cpu.examineOpcode(), 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(), std::runtime_error);
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0001); // PC should increment by 1

    cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(cpu.examineOpcode(), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_NO_THROW(cpu.execute()); // Execute NOP instruction
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0002); // PC should increment by 1

    cpu.fetchOpcode(); // Should fetch LXI B,D16 instruction (0x01)
    EXPECT_EQ(cpu.examineOpcode(), 0x01);
    cpu.execute(); // Execute LXI B,D16 instruction
    EXPECT_EQ(cpu.examineRegisterBC(), 0x1234); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(cpu.examineRegisterB(), 0x12); // High byte
    EXPECT_EQ(cpu.examineRegisterC(), 0x34); // Low byte
    EXPECT_EQ(cpu.examineWordData(), 0x1234); // WORD read
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0005); // PC should point to next instruction
}

TEST(intel8080Test, emuTest_cpuTestSTAX_B) {
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

    cpu.fetchOpcode(); // Fetch first opcode
    EXPECT_EQ(cpu.examineOpcode(), 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(), std::runtime_error);
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0001); // PC should increment by 1

    cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(cpu.examineOpcode(), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_NO_THROW(cpu.execute()); // Execute NOP instruction
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0002); // PC should increment by 1

    cpu.fetchOpcode(); // Should fetch LXI B,D16 instruction (0x01)
    EXPECT_EQ(cpu.examineOpcode(), 0x01);
    cpu.execute(); // Execute LXI B,D16 instruction
    EXPECT_EQ(cpu.examineRegisterBC(), 0x1234); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(cpu.examineRegisterB(), 0x12); // High byte
    EXPECT_EQ(cpu.examineRegisterC(), 0x34); // Low byte
    EXPECT_EQ(cpu.examineWordData(), 0x1234); // WORD read
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0005); // PC should point to next instruction

    cpu.setRegisterA(0x56); // Set Accumulator to known value
    EXPECT_EQ(cpu.examineRegisterA(), 0x56); // Verify Accumulator has the correct value
    cpu.setRegisterBC(0x3124); // Set BC to point to test address
    EXPECT_EQ(cpu.examineRegisterBC(), 0x3124); // Verify Extended Register BC contains the correct address
    cpu.fetchOpcode(); // Should fetch STAX B instruction (0x02)
    EXPECT_EQ(cpu.examineOpcode(), 0x02); // Verify we have the correct opcode
    cpu.execute(); // Execute STAX B instruction
    cpu.readByte(cpu.examineRegisterBC()); // Read memory at test address
    EXPECT_EQ(cpu.examineByteData(), 0x56); // Check that memory at address BC contains value of Accumulator
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0006); // PC should point to next instruction
}

TEST(intel8080Test, emuTest_cpuTestINX_B) {
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

    cpu.fetchOpcode(); // Fetch first opcode
    EXPECT_EQ(cpu.examineOpcode(), 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(), std::runtime_error);
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0001); // PC should increment by 1

    cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(cpu.examineOpcode(), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_NO_THROW(cpu.execute()); // Execute NOP instruction
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0002); // PC should increment by 1

    cpu.fetchOpcode(); // Should fetch LXI B,D16 instruction (0x01)
    EXPECT_EQ(cpu.examineOpcode(), 0x01);
    cpu.execute(); // Execute LXI B,D16 instruction
    EXPECT_EQ(cpu.examineRegisterBC(), 0x1234); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(cpu.examineRegisterB(), 0x12); // High byte
    EXPECT_EQ(cpu.examineRegisterC(), 0x34); // Low byte
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0005); // PC should point to next instruction

    cpu.fetchOpcode(); // Should fetch STAX B instruction (0x02)
    cpu.setRegisterA(0x56); // Set Accumulator to known value
    cpu.setRegisterBC(0x3124); // Set BC to point to test address
    EXPECT_EQ(cpu.examineOpcode(), 0x02);
    cpu.execute(); // Execute STAX B instruction
    cpu.readByte(0x3124);
    EXPECT_EQ(cpu.examineByteData(), cpu.examineRegisterA()); // Check that memory at address BC contains value of Accumulator
    EXPECT_EQ(cpu.examineRegisterPC(), 0x0006); // PC should point to next instruction

    cpu.fetchOpcode(); // Should fetch INX B instruction (0x03)
    EXPECT_EQ(cpu.examineOpcode(), 0x03);
    cpu.setRegisterBC(0x3124); // Set BC to a test value
    EXPECT_EQ(cpu.examineRegisterBC(), 0x3124);
    cpu.execute();
    EXPECT_EQ(cpu.examineRegisterBC(), 0x3125);
}

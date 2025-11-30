#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include "intel8080.h"
#include "intel8080TestHelper.h"
#include "ram.h"
#include "rom.h"
#include "bus.h"



TEST(intel8080Test, cpuTestMemoryAccess) {
    //spdlog::set_level(spdlog::level::debug);

    Intel8080 cpu;
    Bus bus;

    std::vector<BYTE> rombuffer({0x01, 0x00, 0x00, 0x02, 0x34, 0x12, 0x00});
    Rom rom(0x2000, rombuffer); // 8KB ROM

    std::vector<BYTE> rambuffer({0x02, 0x03, 0x12, 0x89, 0x67, 0x00});
    Ram ram(0x2000, rambuffer); // 8KB RAM

    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);
    EXPECT_EQ(Intel8080TestHelper::getByteAtAddress(cpu, 0x0000), 0x01);
    EXPECT_EQ(Intel8080TestHelper::getByteAtAddress(cpu, 0x0003), 0x02);
    EXPECT_EQ(Intel8080TestHelper::getWordAtAddress(cpu, 0x0004), 0x1234);

    EXPECT_EQ(Intel8080TestHelper::getByteAtAddress(cpu, 0x2000), 0x02);
    EXPECT_EQ(Intel8080TestHelper::getByteAtAddress(cpu, 0x2001), 0x03);
    EXPECT_EQ(Intel8080TestHelper::getWordAtAddress(cpu, 0x2003), 0x6789);
}

TEST(intel8080Test, cpuTestPC) {
    Intel8080 cpu;
    Bus bus;
    const std::vector<BYTE> buffer = {0x00, 0x00, 0x00, 0x01, 0x34, 0x12, 0x00};
    Rom rom(0x2000, buffer); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM


    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0000); // Initial PC should be 0

    cpu.step(); // Execute NOP (0x00)
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1
    cpu.step(); // Execute NOP (0x00)
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0002); // PC should increment by 1
    cpu.step(); // Execute NOP (0x00)
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0003); // PC should increment by 1
    cpu.step(); // Execute LXI B,D16 (0x01)
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0006); // PC should increment by 3
    cpu.step(); // Execute NOP (0x00)
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0007); // PC should increment by 1
}

TEST(intel8080Test, cpuTestReset) {
    Intel8080 cpu;

    // Check initial state
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0000);
    EXPECT_EQ(Intel8080TestHelper::getFlags(cpu), 0x02); // Only the 'always 1' bit should be set

    // Modify the CPU state
    Intel8080TestHelper::setRegisterA(cpu, 0xFF);
    Intel8080TestHelper::setRegisterBC(cpu, 0x1234);
    Intel8080TestHelper::setRegisterDE(cpu, 0x5678);
    Intel8080TestHelper::setRegisterHL(cpu, 0x9ABC);
    Intel8080TestHelper::setRegisterSP(cpu, 0xDEF0);
    Intel8080TestHelper::setRegisterPC(cpu, 0x1000);
    Intel8080TestHelper::setFlags(cpu, 0xFF);

    // Reset the CPU
    cpu.reset();

    // Verify the state is reset
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0000);
    EXPECT_EQ(Intel8080TestHelper::getFlags(cpu), 0x02);
    EXPECT_EQ(Intel8080TestHelper::getRegisterBC(cpu), 0x0000);
}

TEST(intel8080Test, cpuTestIllegalOpcode) {
    Intel8080 cpu;
    Bus bus;
    const std::vector<BYTE> buffer = {0x00, 0xff};
    Rom rom(0x2000, buffer); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM
    

    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    EXPECT_NO_THROW(cpu.step()); // Execute NOP (0x00)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x00);
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1
    EXPECT_THROW(cpu.step(), std::runtime_error); // Execute ILLEGAL (0xff)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0xff);
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0002); // PC should increment by 1 (because fetching the opcode already moved the PC)
}

TEST(intel8080Test, cpuTestNOP) {
    Intel8080 cpu;
    Bus bus;
    const std::vector<BYTE> buffer = {0x00, 0x01, 0x34, 0x12};
    Rom rom(0x2000, buffer);
    Ram ram(0x2000);

    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    EXPECT_NO_THROW(cpu.step()); // Execute NOP (0x00)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1
}

TEST(intel8080Test, cpuTestLXI_B_D16) {
    Intel8080 cpu;
    Bus bus;
    const std::vector<BYTE> buffer = {0x00, 0x01, 0x34, 0x12};
    Rom rom(0x2000, buffer);
    Ram ram(0x2000);
    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    EXPECT_NO_THROW(cpu.step()); // Execute NOP (0x00)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1

    cpu.step(); // Execute LXI B,D16 (0x01)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x01);
    EXPECT_EQ(Intel8080TestHelper::getRegisterBC(cpu), 0x1234); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(Intel8080TestHelper::getRegisterB(cpu), 0x12); // High byte
    EXPECT_EQ(Intel8080TestHelper::getRegisterC(cpu), 0x34); // Low byte
    EXPECT_EQ(Intel8080TestHelper::getWordData(cpu), 0x1234); // WORD read
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0004); // PC should point to next instruction
}

TEST(intel8080Test, emuTest_cpuTestSTAX_B) {
    Intel8080 cpu;
    Bus bus;
    const std::vector<BYTE> buffer = {0x00, 0x01, 0x12, 0x21, 0x02};
    Rom rom(0x2000, buffer);
    Ram ram(0x2000);
    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    EXPECT_NO_THROW(cpu.step()); // Execute NOP (0x00)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1

    cpu.step(); // Execute LXI B,D16 (0x01)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x01);
    EXPECT_EQ(Intel8080TestHelper::getRegisterBC(cpu), 0x2112); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0004); // PC should point to next instruction

    Intel8080TestHelper::setRegisterA(cpu, 0x56); // Set Accumulator to known value
    cpu.step(); // Execute STAX B (0x02)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x02); // Verify we have the correct opcode
    EXPECT_EQ(Intel8080TestHelper::getByteAtAddress(cpu, Intel8080TestHelper::getRegisterBC(cpu)), 0x56); // Check that memory at address BC contains value of Accumulator
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0005); // PC should point to next instruction
}

TEST(intel8080Test, emuTest_cpuTestINX_B) {
    Intel8080 cpu;
    Bus bus;
    const std::vector<BYTE> buffer = {0x00, 0x01, 0x12, 0x21, 0x03, 0x00};
    Rom rom(0x2000, buffer);
    Ram ram(0x2000);
    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    EXPECT_NO_THROW(cpu.step()); // Execute NOP (0x00)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1

    cpu.step(); // Execute LXI B,D16 (0x01)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x01);
    EXPECT_EQ(Intel8080TestHelper::getRegisterBC(cpu), 0x2112); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0004); // PC should point to next instruction

    cpu.step(); // Execute INX B (0x03)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x03);
    EXPECT_EQ(Intel8080TestHelper::getRegisterBC(cpu), 0x2113);
}

TEST(intel8080Test, emuTest_cpuTestINR_B) {
    Intel8080 cpu;
    Bus bus;
    const std::vector<BYTE> buffer = {0x00, 0x01, 0x12, 0x2F, 0x04, 0x00, 0x0};
    Rom rom(0x2000, buffer);
    Ram ram(0x2000);

    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    EXPECT_NO_THROW(cpu.step()); // Execute NOP (0x00)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x00);
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0001);

    cpu.step(); // Execute LXI B,D16
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x01);
    EXPECT_EQ(Intel8080TestHelper::getRegisterBC(cpu), 0x2F12);
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0004);

    cpu.step(); // Execute INR B
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x04);
    EXPECT_EQ(Intel8080TestHelper::getRegisterB(cpu), 0x30);
    // FLAGS - SZ0A0P1C - 00010110 = 0x16 = 22
    EXPECT_EQ(Intel8080TestHelper::getFlags(cpu), 0x16);
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0005);
}

TEST(intel8080Test, cpuTestFlags) {
    Intel8080 cpu;
    const std::vector<BYTE> buffer = {0x04, 0x00};
    Rom rom(0x2000, buffer);
    Ram ram(0x2000);

    Bus bus;
    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    EXPECT_NO_THROW(cpu.step()); // Execute NOP (0x00)

    cpu.reset();

    EXPECT_EQ(Intel8080TestHelper::getFlags(cpu), 0x02);

    Intel8080TestHelper::setRegisterB(cpu, 0x7f);
    cpu.step();
    EXPECT_EQ(Intel8080TestHelper::getRegisterB(cpu), 0x80);
    // Flags should be S, A = 0x92 b[1001 0010]
    EXPECT_EQ(Intel8080TestHelper::getFlags(cpu), 0x92);
}

TEST(intel8080Test, emuTest_cpuTestDCR_B) {
    Intel8080 cpu;
    Bus bus;
    const std::vector<BYTE> buffer = {0x00, 0x01, 0x12, 0x21, 0x05, 0x00, 0x0};
    Rom rom(0x2000, buffer);
    Ram ram(0x2000);

    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    EXPECT_NO_THROW(cpu.step()); // Execute NOP (0x00)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x00);
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0001);

    cpu.step(); // Execute LXI B,D16
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x01);
    EXPECT_EQ(Intel8080TestHelper::getRegisterBC(cpu), 0x2112);
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0004);

    cpu.step(); // Execute DCR B
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x05);
    EXPECT_EQ(Intel8080TestHelper::getRegisterB(cpu), 0x20);
    EXPECT_EQ(Intel8080TestHelper::getFlags(cpu), 0x02);
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0005);
}

TEST(intel8080Test, emuTest_cpuTestMVI_B_D8) {
    Intel8080 cpu;
    Bus bus;
    const std::vector<BYTE> buffer = {0x06, 0x42, 0x00};
    Rom rom(0x2000, buffer);
    Ram ram(0x2000);

    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    EXPECT_NO_THROW(cpu.step()); // Execute MVI B, D8 (0x06)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x06);
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0002);
    EXPECT_EQ(Intel8080TestHelper::getByteData(cpu), 0x42);
    EXPECT_EQ(Intel8080TestHelper::getRegisterB(cpu), 0x42);
}

TEST(intel8080Test, emuTest_cpuTestDCR_C) {
    Intel8080 cpu;
    Bus bus;
    const std::vector<BYTE> buffer = {0x00, 0x0D, 0x0D, 0x00};
    Rom rom(0x2000, buffer);
    Ram ram(0x2000);

    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    EXPECT_NO_THROW(cpu.step()); // Execute NOP (0x00)
    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x00);
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0001);

    Intel8080TestHelper::setRegisterC(cpu, 0x05); // Set C register to known value
    EXPECT_EQ(Intel8080TestHelper::getRegisterC(cpu), 0x05);

    cpu.step(); // Execute DCR C

    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x0D);
    EXPECT_EQ(Intel8080TestHelper::getRegisterC(cpu), 0x04);
    EXPECT_EQ(Intel8080TestHelper::getFlags(cpu), 0x02);
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0002);

    Intel8080TestHelper::setRegisterC(cpu, 0x00); // Set C register to known value
    EXPECT_EQ(Intel8080TestHelper::getRegisterC(cpu), 0x00);

    cpu.step(); // Execute DCR C

    EXPECT_EQ(Intel8080TestHelper::getOpcode(cpu), 0x0D);
    EXPECT_EQ(Intel8080TestHelper::getRegisterC(cpu), 0xFF);
    EXPECT_EQ(Intel8080TestHelper::getFlags(cpu), 0x96);
    EXPECT_EQ(Intel8080TestHelper::getRegisterPC(cpu), 0x0003);

}

TEST(intel8080Test, emuTest_cpuTestDAD_B)
{
    Intel8080 cpu;
    const std::vector<BYTE> buffer = {0x09, 0x00, 0x00};
    Rom rom(0x2000, buffer);
    Ram ram(0x2000);
    Bus bus;

    cpu.reset();

    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    Intel8080TestHelper::setRegisterHL(cpu, 0xFFFF);
    Intel8080TestHelper::setRegisterBC(cpu, 0x0FFF);

    EXPECT_NO_THROW(cpu.step()); // Execute DAD_B
}





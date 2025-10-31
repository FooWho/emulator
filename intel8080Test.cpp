#include <gtest/gtest.h>
#include <spdlog/spdlog.h>
#include "intel8080.h"
#include "ram.h"
#include "rom.h"
#include "bus.h"


class CPUTestHelper {
 public:
    static BYTE getRegisterA(Intel8080& cpu){
        return cpu.examineRegisterA();
    }

    static void setRegisterA(Intel8080& cpu, BYTE value) {
        cpu.setRegisterA(value);
    }

    static BYTE getRegisterB(Intel8080& cpu) {
        return cpu.examineRegisterB();
    }

    static void setRegisterB(Intel8080& cpu, BYTE value) {
        cpu.setRegisterB(value);
    }

    static BYTE getRegisterC(Intel8080& cpu) {
        return cpu.examineRegisterC();
    }

    static void setRegisterC(Intel8080& cpu, BYTE value) {
        cpu.setRegisterC(value);
    }

    static BYTE getRegisterD(Intel8080& cpu) {
        return cpu.examineRegisterD();
    }

    static void setRegisterD(Intel8080& cpu, BYTE value) {
        cpu.setRegisterD(value);
    }

    static BYTE getRegisterE(Intel8080& cpu) {
        return cpu.examineRegisterE();
    }

    static void setRegisterE(Intel8080& cpu, BYTE value) {
        cpu.setRegisterE(value);
    }

    static BYTE getRegisterH(Intel8080& cpu) {
        return cpu.examineRegisterH();
    }

    static void setRegisterH(Intel8080& cpu, BYTE value) {
        cpu.setRegisterH(value);
    }   

    static BYTE getRegisterL(Intel8080& cpu) {
        return cpu.examineRegisterL();
    }

    static void setRegisterL(Intel8080& cpu, BYTE value) {
        cpu.setRegisterL(value);
    }

    static WORD getRegisterBC(Intel8080& cpu) {
        return cpu.examineRegisterBC();
    }

    static void setRegisterBC(Intel8080& cpu, WORD value) {
        cpu.setRegisterBC(value);
    }

    static WORD getRegisterDE(Intel8080& cpu) {
        return cpu.examineRegisterDE();
    }

    static void setRegisterDE(Intel8080& cpu, WORD value) {
        cpu.setRegisterDE(value);
    }

    static WORD getRegisterHL(Intel8080& cpu) {
        return cpu.examineRegisterHL();
    }

    static void setRegisterHL(Intel8080& cpu, WORD value) {
        cpu.setRegisterHL(value);
    }

    static WORD getRegisterSP(Intel8080& cpu) {
        return cpu.examineRegisterSP();
    }

    static void setRegisterSP(Intel8080& cpu, WORD value) {
        cpu.setRegisterSP(value);
    }

    static WORD getRegisterPC(Intel8080& cpu) {
        return cpu.examineRegisterPC();
    }

    static void setRegisterPC(Intel8080& cpu, WORD value) {
        cpu.setRegisterPC(value);
    }

    static BYTE getFlags(Intel8080& cpu) {
        return cpu.examineFlags();
    }

    static void setFlags(Intel8080& cpu, BYTE value) {
        cpu.setFlags(value);
    }

    static BYTE getOpcode(Intel8080& cpu) {
        return cpu.examineOpcode();
    } 

    static void setOpcode(Intel8080& cpu, BYTE value) {
        cpu.setOpcode(value);
    }   

    static BYTE getByteData(Intel8080& cpu) {
        return cpu.examineByteData();
    }

    static void setByteData(Intel8080& cpu, BYTE value) {
        cpu.setByteData(value);
    }

    static WORD getWordData(Intel8080& cpu) {
        return cpu.examineWordData();
    }

    static void setWordData(Intel8080& cpu, WORD value) {
        cpu.setWordData(value);
    }

    static BYTE getByteAtAddress(Intel8080& cpu, WORD address) {
        cpu.readByte(address);
        return cpu.examineByteData();
    }

    static void setByteAtAddress(Intel8080& cpu, WORD address, BYTE value) {
        cpu.writeByte(address, value);
    }

    static WORD getWordAtAddress(Intel8080& cpu, WORD address) {
        cpu.readWord(address);
        return cpu.examineWordData();
    }

    static void setWordAtAddress(Intel8080& cpu, WORD address, WORD value) {
        cpu.writeWord(address, value);
    }
};

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
    EXPECT_EQ(CPUTestHelper::getByteAtAddress(cpu, 0x0000), 0x01);
    EXPECT_EQ(CPUTestHelper::getByteAtAddress(cpu, 0x0003), 0x02);
    EXPECT_EQ(CPUTestHelper::getWordAtAddress(cpu, 0x0004), 0x1234);

    EXPECT_EQ(CPUTestHelper::getByteAtAddress(cpu, 0x2000), 0x02);
    EXPECT_EQ(CPUTestHelper::getByteAtAddress(cpu, 0x2001), 0x03);
    EXPECT_EQ(CPUTestHelper::getWordAtAddress(cpu, 0x2003), 0x6789);
}

TEST(intel8080Test, cpuTestPC) {
    Intel8080 cpu;
    Bus bus;
    const std::vector<BYTE> buffer = {0x00, 0x00, 0x00, 0x01, 0x34, 0x12, 0x00};
    Rom rom(0x2000, buffer); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM


    bus.attachCpu(&cpu)->attachMemory(&rom, 0x0000, 0x1FFF)->attachMemory(&ram, 0x2000, 0x3FFF);
    cpu.attachBus(&bus);

    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0000); // Initial PC should be 0

    cpu.step(); // Execute NOP (0x00)
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1
    cpu.step(); // Execute NOP (0x00)
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0002); // PC should increment by 1
    cpu.step(); // Execute NOP (0x00)
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0003); // PC should increment by 1
    cpu.step(); // Execute LXI B,D16 (0x01)
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0006); // PC should increment by 3
    cpu.step(); // Execute NOP (0x00)
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0007); // PC should increment by 1
}

TEST(intel8080Test, cpuTestReset) {
    Intel8080 cpu;

    // Check initial state
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0000);
    EXPECT_EQ(CPUTestHelper::getFlags(cpu), 0x02); // Only the 'always 1' bit should be set

    // Modify the CPU state
    CPUTestHelper::setRegisterA(cpu, 0xFF);
    CPUTestHelper::setRegisterBC(cpu, 0x1234);
    CPUTestHelper::setRegisterDE(cpu, 0x5678);
    CPUTestHelper::setRegisterHL(cpu, 0x9ABC);
    CPUTestHelper::setRegisterSP(cpu, 0xDEF0);
    CPUTestHelper::setRegisterPC(cpu, 0x1000);
    CPUTestHelper::setFlags(cpu, 0xFF);

    // Reset the CPU
    cpu.reset();

    // Verify the state is reset
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0000);
    EXPECT_EQ(CPUTestHelper::getFlags(cpu), 0x02);
    EXPECT_EQ(CPUTestHelper::getRegisterBC(cpu), 0x0000);
}

TEST(intel8080Test, cpuTestFlags) {
    Intel8080 cpu;
    BYTE result;

    result = 0x00;
    EXPECT_EQ(CPUTestHelper::getFlags(cpu), 0x02);
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
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x00);
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1
    EXPECT_THROW(cpu.step(), std::runtime_error); // Execute ILLEGAL (0xff)
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0002); // PC should increment by 1 (because fetching the opcode already moved the PC)
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
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1
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
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1

    cpu.step(); // Execute LXI B,D16 (0x01)
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x01);
    EXPECT_EQ(CPUTestHelper::getRegisterBC(cpu), 0x1234); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(CPUTestHelper::getRegisterB(cpu), 0x12); // High byte
    EXPECT_EQ(CPUTestHelper::getRegisterC(cpu), 0x34); // Low byte
    EXPECT_EQ(CPUTestHelper::getWordData(cpu), 0x1234); // WORD read
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0004); // PC should point to next instruction
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
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1

    cpu.step(); // Execute LXI B,D16 (0x01)
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x01);
    EXPECT_EQ(CPUTestHelper::getRegisterBC(cpu), 0x2112); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0004); // PC should point to next instruction

    CPUTestHelper::setRegisterA(cpu, 0x56); // Set Accumulator to known value
    cpu.step(); // Execute STAX B (0x02)
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x02); // Verify we have the correct opcode
    EXPECT_EQ(CPUTestHelper::getByteAtAddress(cpu, CPUTestHelper::getRegisterBC(cpu)), 0x56); // Check that memory at address BC contains value of Accumulator
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0005); // PC should point to next instruction
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
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1

    cpu.step(); // Execute LXI B,D16 (0x01)
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x01);
    EXPECT_EQ(CPUTestHelper::getRegisterBC(cpu), 0x2112); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0004); // PC should point to next instruction

    cpu.step(); // Execute INX B (0x03)
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x03);
    EXPECT_EQ(CPUTestHelper::getRegisterBC(cpu), 0x2113);
}

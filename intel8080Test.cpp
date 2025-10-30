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

    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0000); // Initial PC should be 0

    cpu.fetchOpcode(); // Fetch opcode (0x00)
    cpu.execute(); // Execute opcode
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1
    cpu.fetchOpcode(); // Fetch opcode (0x00)
    cpu.execute(); // Execute opcode
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0002); // PC should increment by 1
    cpu.fetchOpcode(); // Fetch opcode (0x00)
    cpu.execute();
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0003); // PC should increment by 1
    cpu.fetchOpcode(); // Fetch opcode (0x01)
    cpu.execute(); // Execute opcode
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0006); // PC should increment by 3
    cpu.fetchOpcode(); // Fetch opcode (0x00)
    cpu.execute(); // Execute opcode
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0007); // PC should increment by 1
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
    Rom rom(0x2000); // 8KB ROM
    Ram ram(0x2000); // 8KB RAM

    const std::vector<BYTE> buffer = {0x00, 0xff};
    rom.romLoad(buffer);
    

    cpu.attachBus(bus.attachCpu(&cpu));
    rom.attachBus(bus.attachMemory(&rom));
    ram.attachBus(bus.attachMemory(&ram));

    cpu.fetchOpcode(); // Fetch first opcode
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x00); // Should fetch NOP (0x00)
    EXPECT_NO_THROW(cpu.execute());
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1
    cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0xff);
    EXPECT_THROW(cpu.execute(), std::runtime_error);
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0002); // PC should increment by 1 (because fetching the opcode already moved the PC)
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
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(), std::runtime_error);
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1

    cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_NO_THROW(cpu.execute()); // Execute NOP instruction
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0002); // PC should increment by 1
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
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(), std::runtime_error);
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1

    cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_NO_THROW(cpu.execute()); // Execute NOP instruction
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0002); // PC should increment by 1

    cpu.fetchOpcode(); // Should fetch LXI B,D16 instruction (0x01)
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x01);
    cpu.execute(); // Execute LXI B,D16 instruction
    EXPECT_EQ(CPUTestHelper::getRegisterBC(cpu), 0x1234); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(CPUTestHelper::getRegisterB(cpu), 0x12); // High byte
    EXPECT_EQ(CPUTestHelper::getRegisterC(cpu), 0x34); // Low byte
    EXPECT_EQ(CPUTestHelper::getWordData(cpu), 0x1234); // WORD read
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0005); // PC should point to next instruction
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
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(), std::runtime_error);
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1

    cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_NO_THROW(cpu.execute()); // Execute NOP instruction
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0002); // PC should increment by 1

    cpu.fetchOpcode(); // Should fetch LXI B,D16 instruction (0x01)
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x01);
    cpu.execute(); // Execute LXI B,D16 instruction
    EXPECT_EQ(CPUTestHelper::getRegisterBC(cpu), 0x1234); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(CPUTestHelper::getRegisterB(cpu), 0x12); // High byte
    EXPECT_EQ(CPUTestHelper::getRegisterC(cpu), 0x34); // Low byte
    EXPECT_EQ(CPUTestHelper::getWordData(cpu), 0x1234); // WORD read
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0005); // PC should point to next instruction

    CPUTestHelper::setRegisterA(cpu, 0x56); // Set Accumulator to known value
    EXPECT_EQ(CPUTestHelper::getRegisterA(cpu), 0x56); // Verify Accumulator has the correct value
    CPUTestHelper::setRegisterBC(cpu, 0x3124); // Set BC to point to test address
    EXPECT_EQ(CPUTestHelper::getRegisterBC(cpu), 0x3124); // Verify Extended Register BC contains the correct address
    cpu.fetchOpcode(); // Should fetch STAX B instruction (0x02)
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x02); // Verify we have the correct opcode
    cpu.execute(); // Execute STAX B instruction
    EXPECT_EQ(CPUTestHelper::getByteAtAddress(cpu, CPUTestHelper::getRegisterBC(cpu)), 0x56); // Check that memory at address BC contains value of Accumulator
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0006); // PC should point to next instruction
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
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0xff); // Should fetch illegal instruction (0xff)
    EXPECT_THROW(cpu.execute(), std::runtime_error);
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0001); // PC should increment by 1

    cpu.fetchOpcode(); // Fetch second opcode
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x00); // Should fetch NOP instruction (0x00)
    EXPECT_NO_THROW(cpu.execute()); // Execute NOP instruction
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0002); // PC should increment by 1

    cpu.fetchOpcode(); // Should fetch LXI B,D16 instruction (0x01)
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x01);
    cpu.execute(); // Execute LXI B,D16 instruction
    EXPECT_EQ(CPUTestHelper::getRegisterBC(cpu), 0x1234); // Check that BC register pair is loaded correctly;
    EXPECT_EQ(CPUTestHelper::getRegisterB(cpu), 0x12); // High byte
    EXPECT_EQ(CPUTestHelper::getRegisterC(cpu), 0x34); // Low byte
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0005); // PC should point to next instruction

    cpu.fetchOpcode(); // Should fetch STAX B instruction (0x02)
    CPUTestHelper::setRegisterA(cpu, 0x56); // Set Accumulator to known value
    CPUTestHelper::setRegisterBC(cpu, 0x3124); // Set BC to point to test address
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x02);
    cpu.execute(); // Execute STAX B instruction
    EXPECT_EQ(CPUTestHelper::getByteAtAddress(cpu, CPUTestHelper::getRegisterBC(cpu)), CPUTestHelper::getRegisterA(cpu)); // Check that memory at address BC contains value of Accumulator
    EXPECT_EQ(CPUTestHelper::getRegisterPC(cpu), 0x0006); // PC should point to next instruction

    cpu.fetchOpcode(); // Should fetch INX B instruction (0x03)
    EXPECT_EQ(CPUTestHelper::getOpcode(cpu), 0x03);
    CPUTestHelper::setRegisterBC(cpu, 0x3124); // Set BC to a test value
    EXPECT_EQ(CPUTestHelper::getRegisterBC(cpu), 0x3124);
    cpu.execute();
    EXPECT_EQ(CPUTestHelper::getRegisterBC(cpu), 0x3125);
}

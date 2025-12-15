#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <fstream>
#include "intel8080.hpp"
#include "intel8080TestHelper.hpp"
#include "rom.hpp"
#include "ram.hpp"
#include "bus.hpp"

int main(int argc, char *argv[]) {       
    Intel8080 cpu;
    Bus bus;

    Ram ram(0xFFFF); // 8KB RAM

    cpu.attachBus(&bus);

    bus.attachMemory(&ram, 0x0000, 0xFFFE); 

    std::ifstream file("/home/jelison/Workspace/TST8080.BIN", std::ios::binary | std::ios::ate);

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (file.read(buffer.data(), size)) {
        // CRITICAL: Load at 0x0100 (CP/M TPA start address)
        for (int i = 0; i < size; i++) {
            bus.writeByte(0x0100 + i, (uint8_t)buffer[i]);
        }
    }
    
    // Inject "OS" shim at 0x0005
    // When the CPU calls 0x0005, we want it to return immediately 
    // so we can intercept it in the emulator loop.
    // 0xC9 is the opcode for RET
    bus.writeByte(0x0005, 0xC9);

    Intel8080TestHelper::setRegisterPC(cpu, 0x0100);
    Intel8080TestHelper::setRegisterSP(cpu, 0xF000);

    bool finished = false;
    while (!finished) {
        cpu.step();
        // ---------------------------------------------------------
        // CP/M BDOS Shim (The "Magic" Part)
        // ---------------------------------------------------------
        // CPUDiag prints by calling address 0x0005.
        // When we see PC == 0x0005, we look at Register C to see what to do.
        if (Intel8080TestHelper::getRegisterPC(cpu) == 0x0005) {
            
            // C = 9: Print String (DE points to string, ends with '$')
            if (Intel8080TestHelper::getRegisterC(cpu) == 9) {
                WORD addr = Intel8080TestHelper::getRegisterDE(cpu);
                char c = bus.readByte(addr);
                while (c != '$') {
                    std::cout << c;
                    addr++;
                    c = bus.readByte(addr);
                }
            }
            
            // C = 2: Print Character (E contains the char)
            else if (Intel8080TestHelper::getRegisterC(cpu) == 2) {
                std::cout << (char)Intel8080TestHelper::getRegisterE(cpu);
            }
            
            // Note: Since we put a RET (0xC9) at 0x0005 in RAM, 
            // the CPU will automatically execute that RET and go back 
            // to the main program. We just "peeked" to see what to print.
        }
        
        // Check for "JP 0000" (Warm Boot) - This indicates the test is done
        if (Intel8080TestHelper::getRegisterPC(cpu) == 0x0000) {
            std::cout << "\n\n*** Terminated (Jumped to 0x0000) ***" << std::endl;
            finished = true;
        }
    }
}
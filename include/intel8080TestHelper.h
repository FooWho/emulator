#pragma once
#include "intel8080.h"


class Intel8080TestHelper {
 public:
    static inline BYTE getRegisterA(Intel8080& cpu){
        return cpu.regs.a;
    }

    static inline void setRegisterA(Intel8080& cpu, BYTE value) {
        cpu.regs.a = value;
    }

    static inline BYTE getRegisterB(Intel8080& cpu) {
        return cpu.regs.b;
    }

    static inline void setRegisterB(Intel8080& cpu, BYTE value) {
        cpu.regs.b = value;
    }

    static inline BYTE getRegisterC(Intel8080& cpu) {
        return cpu.regs.c;
    }

    static inline void setRegisterC(Intel8080& cpu, BYTE value) {
        cpu.regs.c = value;
    }

    static inline BYTE getRegisterD(Intel8080& cpu) {
        return cpu.regs.d;
    }

    static inline void setRegisterD(Intel8080& cpu, BYTE value) {
        cpu.regs.d = value;
    }

    static inline BYTE getRegisterE(Intel8080& cpu) {
        return cpu.regs.e;
    }

    static inline void setRegisterE(Intel8080& cpu, BYTE value) {
        cpu.regs.e = value;
    }

    static inline BYTE getRegisterH(Intel8080& cpu) {
        return cpu.regs.h;
    }

    static inline void setRegisterH(Intel8080& cpu, BYTE value) {
        cpu.regs.h = value;
    }   

    static inline BYTE getRegisterL(Intel8080& cpu) {
        return cpu.regs.l;
    }

    static inline void setRegisterL(Intel8080& cpu, BYTE value) {
        cpu.regs.l = value;
    }

    static inline WORD getRegisterBC(Intel8080& cpu) {
        return (WORD)(cpu.regs.b << 8) | cpu.regs.c;
    }

    static inline void setRegisterBC(Intel8080& cpu, WORD value) {
        cpu.regs.b = (value >> 8) & 0xFF;
        cpu.regs.c = value & 0xFF;
    }

    static inline WORD getRegisterDE(Intel8080& cpu) {
        return (WORD)(cpu.regs.d << 8) | cpu.regs.e;
    }

    static inline void setRegisterDE(Intel8080& cpu, WORD value) {
        cpu.regs.d = (value >> 8) & 0xFF;
        cpu.regs.e = value & 0xFF;
    }

    static inline WORD getRegisterHL(Intel8080& cpu) {
        return (WORD)(cpu.regs.h << 8) | cpu.regs.l;
    }

    static inline void setRegisterHL(Intel8080& cpu, WORD value) {
        cpu.regs.h = (value >> 8) & 0xFF;
        cpu.regs.l = value & 0xFF;
    }

    static inline WORD getRegisterSP(Intel8080& cpu) {
        return cpu.regs.sp;
    }

    static inline void setRegisterSP(Intel8080& cpu, WORD value) {
        cpu.regs.sp = value;
    }

    static inline WORD getRegisterPC(Intel8080& cpu) {
        return cpu.regs.pc;
    }

    static inline void setRegisterPC(Intel8080& cpu, WORD value) {
        cpu.regs.pc = value;
    }

    static inline BYTE getFlags(Intel8080& cpu) {
        return cpu.flags.s << 7 | cpu.flags.z << 6 | cpu.flags.x_zero << 5 | 
            cpu.flags.ac << 4 | cpu.flags.y_zero << 3 | cpu.flags.p << 2 | 
            cpu.flags.x_one << 1 | cpu.flags.cy;
    }

    static inline void setFlags(Intel8080& cpu, BYTE value) {
        cpu.flags.s = (value & 0x80) ? 1 : 0;
        cpu.flags.z = (value & 0x40) ? 1 : 0;
        cpu.flags.x_zero = 0; // Always 0
        cpu.flags.ac = (value & 0x10) ? 1 : 0;
        cpu.flags.y_zero = 0; // Always 0
        cpu.flags.p = (value & 0x04) ? 1 : 0;
        cpu.flags.x_one = 1;  // Always 1
        cpu.flags.cy = (value & 0x01) ? 1 : 0;
    }

    static inline void auxCarry(Intel8080& cpu, WORD ops)
    {
        cpu.regFlagsAuxCarry(ops);
    }

    static inline BYTE getOpcode(Intel8080& cpu) {
        return cpu.opcode;
    } 

    static inline void setOpcode(Intel8080& cpu, BYTE value) {
        cpu.opcode = value;
    }   

    static inline BYTE getByteData(Intel8080& cpu) {
        return cpu.byteData;
    }

    static inline void setByteData(Intel8080& cpu, BYTE value) {
        cpu.byteData = value;
    }

    static inline WORD getWordData(Intel8080& cpu) {
        return cpu.wordData;
    }

    static inline void setWordData(Intel8080& cpu, WORD value) {
        cpu.wordData = value;
    }

    static inline BYTE getByteAtAddress(Intel8080& cpu, WORD address) {
        cpu.readByte(address);
        return cpu.byteData;
    }

    static inline void setByteAtAddress(Intel8080& cpu, WORD address, BYTE value) {
        cpu.writeByte(address, value);
    }

    static inline WORD getWordAtAddress(Intel8080& cpu, WORD address) {
        cpu.readWord(address);
        return cpu.wordData;
    }

    static inline void setWordAtAddress(Intel8080& cpu, WORD address, WORD value) {
        cpu.writeWord(address, value);
    }
};
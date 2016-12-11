#include "chip8.h"

chip8::chip8()
{
     Initialize(); 
}

chip8::~chip8() { }

void chip8::Initialize()
{
    pc = 0x200; // Start of most CHIP8 program
    opcode = 0;
    I = 0;
    sp = 0;   
}

void chip8::Cycle()
{
    // Emulate CPU cycle
}
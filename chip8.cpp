#include "chip8.h"

#include <stdio.h>

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

void chip8::LoadProgram(const char* filePath)
{
    // Load program into memory
    unsigned char buffer[1];
    FILE* programFile = fopen(filePath, "rb");
    int i = 0;
    while(!feof(programFile))
    {
        fread(buffer, 1, 1, programFile);
        memory[i + 512] = *buffer;
        i++;
    }
    fclose(programFile);
}

void chip8::Draw()
{
    // Draw graphics into screen
}
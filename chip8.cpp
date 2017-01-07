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
    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc+1];

    // Debug display
    printf("Current pc : %#06x\n", pc);
    printf("Current opcode : %#06x\n",opcode);

    switch(opcode & 0xF000)
    {
        // 1NNN : Jump to adress NNN
        case 0x1000:
        {
            pc = opcode & 0x0FFF;
            break;
        }

        // 3XNN : Skips the next instruction if VX equals NN.
        case 0x3000:
        {
            int x = (opcode & 0x0F00) >> 8; 
            if (V[x] == (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
            break;
        } 

        // 4XNN : Skips the next instruction if VX not equal to NN.
        case 0x4000:
        {
            int x = (opcode & 0x0F00) >> 8; 
            if (V[x] != (opcode & 0x00FF))
                pc += 4;
            else
                pc += 2;
            break;
        }         
        
        // 6XNN : Set V[X] to NN
        case 0x6000:
        {
            int x = (opcode & 0x0F00) >> 8;
            V[x] = (opcode & 0x00FF);
            pc += 2;
            break;
        }

         // 7XNN : Add to V[X] the content of NN
        case 0x7000:
        {
            int x = (opcode & 0x0F00) >> 8;
            V[x] += (opcode & 0x00FF);
            pc += 2;
            break;
        }

        default:
            pc += 2;
    }          
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
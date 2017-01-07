#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

chip8::chip8()
{
     Initialize(); 
}

chip8::~chip8() { }

const unsigned char* chip8::getDataRegister()
{
    return V;
}

const unsigned char chip8::getDelay()
{
    return delay_timer;
}

void chip8::Initialize()
{
    pc = 0x200; // Start of most CHIP8 program
    opcode = 0;
    I = 0;
    sp = 0;

    // load fontset into memory
    for (int i = 0; i < 80; i++)
        memory[i] = chip8_fontset[i];

    // Init RNG
    srand(time(NULL));
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

         // 5XYN : Skips the next instruction if VX equals VY.
        case 0x5000:
        {
            int x = (opcode & 0x0F00) >> 8;
            int y = (opcode & 0x00F0) >> 4; 
            if (V[x] == V[y])
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

        // ANNN : Sets I to the address NNN.
        case 0xA000:
        {
            I = (opcode & 0x0FFF);
            pc += 2;
            break;
        }

        // BNNN : Jumps to the address NNN plus V0
        case 0xB000:
        {
            pc = (opcode & 0x0FFF) + V[0];
            break;
        }

        // CXNN : Set VX to bitwise operation AND with a random number
        case 0xC000:
        {
            int x = (opcode & 0x0F00) >> 8;
            V[x] = (rand()%16)&(opcode & 0x0FF);
            pc+=2;
            break;
        }

        // 0x8FFF
        case 0x8000:
        {
            int x = (opcode & 0x0F00) >> 8;
            int y = (opcode & 0x0F00) >> 4;
            switch (opcode & 0x000F)
            {               
                case 0x0000:
                {
                    V[x] = V[y];
                    pc+=2;
                    break;
                }

                case 0x0001:
                {
                    V[x] = V[x]|V[y];
                    pc+=2;
                    break;
                }
                case 0x0002:
                {
                    V[x] = V[x]&V[y];
                    pc+=2;
                    break;
                }
                case 0x0003:
                {
                    V[x] = V[x]^V[y];
                    pc+=2;
                    break;
                }
                case 0x0004:
                {
                    V[x] += V[y];
                    pc+=2;
                    break;
                }
                case 0x0005:
                {
                    V[x] -= V[y];
                    pc+=2;
                    break;
                }
                case 0x0006:
                {
                    V[x] = V[x] >> 1;
                    pc+=2;
                    break;
                }
                case 0x0007:
                {
                    V[x] = V[y] - V[x];
                    pc+=2;
                    break;
                }
                case 0x000E:
                {
                    V[x] = V[x] << 1;
                    pc+=2;
                    break;
                }
            }
            break;
        }

        case 0xF000:
        {            
            switch (opcode & 0x00FF)
            {
                case 0x0007:
                {
                    int x = (opcode & 0x0F00) >> 8;
                    V[x] = delay_timer;
                    pc+=2;
                    break;
                }
                case 0x0015:
                {
                    int x = (opcode & 0x0F00) >> 8;
                    delay_timer = V[x];
                    pc+=2;
                    break;
                }
                case 0x0018:
                {
                    int x = (opcode & 0x0F00) >> 8;
                    sound_timer = V[x];
                    pc+=2;
                    break;
                }
                case 0x001E:
                {
                    int x = (opcode & 0x0F00) >> 8;
                    I += V[x];
                    pc+=2;
                    break;
                }
                case 0x0055:
                {
                    int x = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i <= x; i++)
                    {
                        memory[I + i] = V[i];
                    }
                    pc+=2;
                    break;
                }
                case 0x0065:
                {
                    int x = (opcode & 0x0F00) >> 8;
                    for (int i = 0; i <= x; i++)
                    {
                        V[i] = memory[I + i] ;
                    }
                    pc+=2;
                    break;
                }                
            }
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
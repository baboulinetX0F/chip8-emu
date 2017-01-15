#include "chip8.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

chip8::chip8()
{
     Initialize();
     _renderer = new Renderer();
}

chip8::~chip8() { }

const unsigned char* chip8::getDataRegister()
{
    return V;
}

void chip8::Initialize()
{
    pc = 0x200; // Start of most CHIP8 program
    opcode = 0;
    I = 0;
    sp = 0;

    // initialize memory
    for (int i = 0; i<4096;i++)
        memory[i] = 0;

    // initialize gfx
    for (int i = 0; i<64*32;i++)
        gfx[i] = 0;

    for (int i = 0; i<16; i++)
        input[i] = 0;
    
    // load fontset into memory
    for (int i = 0; i < 80; i++)
        memory[i] = chip8_fontset[i];

    // Init RNG
    srand(time(NULL));
}

void chip8::disp_clear()
{
    for (int i = 0; i <64*32; i++)
    {
        gfx[i] = 0;        
    }
    _drawFlag = true;
}

void chip8::Cycle()
{
    // Fetch opcode
    opcode = memory[pc] << 8 | memory[pc+1];

    // Debug display
    //printf("Current pc : %#06x\n", pc);
    //printf("Current opcode : %#06x\n",opcode);

    switch(opcode & 0xF000)
    {
        case 0x0000:
        {
            // 0x00E0 : clear screen
            if (opcode == 0x00E0)
            {
                disp_clear();
                pc+=2;
                break;
            }
            else if (opcode == 0x00EE) // 0x00EE : Return from subroutine
            {
                --sp;
                pc = stack[sp];
                pc+=2;
                break;
            }
        }
        
        // 1NNN : Jump to adress NNN
        case 0x1000:
        {
            pc = opcode & 0x0FFF;
            break;
        }

        // 0x2NNN : Call a subroutine at adress NNN
        case 0x2000:
        {            
            stack[sp] = pc;
            ++sp;
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
                    if (V[y] > (0xFF - V[x]))
                        V[0xF] = 1;
                    else
                        V[0xF] = 0;

                    V[x] += V[y];
                    pc+=2;
                    break;
                }

                case 0x0005:
                {
                    if (V[y] > V[x])
                         V[0xF] = 1;
                    else
                        V[0xF] = 0;
                    V[x] -= V[y];
                    pc+=2;
                    break;
                }

                case 0x0006:
                {
                    V[0xF] = V[x] & 0x1;
                    V[x] = V[x] >> 1;
                    pc+=2;
                    break;
                }

                case 0x0007:
                {
                    if(V[x] > V[y])	
						V[0xF] = 0; 
					else
						V[0xF] = 1;
                    V[x] = V[y] - V[x];
                    pc+=2;
                    break;
                }

                case 0x000E:
                {
                    V[0xF] = V[x] >> 7;
                    V[x] = V[x] << 1;
                    pc+=2;
                    break;
                }
            }
            break;
        }    

        case 0x9000:
        {
            if (opcode & 0xF00F == 0x9000)
            {
                int x = (opcode & 0x0F00) >> 8;
                int y = (opcode & 0x0F00) >> 4;
                if (V[x] != V[y])
                    pc+=4;
                else
                    pc+=2;
                break;
            }
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
            V[x] = (rand()%0xFF)&(opcode & 0x00FF);
            pc+=2;
            break;
        }

        case 0xD000:
        {
            unsigned short x = V[(opcode & 0x0F00) >> 8];
            unsigned short y = V[(opcode & 0x00F0) >> 4];
            unsigned short height = (opcode & 0x000F);
            unsigned short pixel;

            for (int yline = 0; yline < height; yline++)
            {
                pixel = memory[I + yline];
                for (int xline = 0; xline < 8; xline++)
                {
                    if ((pixel & (0x80 >> xline)) != 0)
                    {
                         if(gfx[(x + xline + ((y + yline) * 64))] == 1)
                            V[0xF] = 1;

                        gfx[x + xline + ((y + yline) * 64)] ^= 1;
                    }
                }
            }
            
            _drawFlag = true;
            pc+=2;
            break;
        }       

        case 0xE000:
        {
            int x = (opcode & 0x0F00) >> 8;
            if (opcode & 0xF0FF == 0xE0A1)
            {
                 if (input[V[x]] == 0)
                    pc+=4;
                else
                    pc+=2;
                break;
            }
            else if (opcode & 0xF0FF == 0xE09E)
            {
                if (input[V[x]] != 0)
                    pc+=4;
                else
                    pc+=2;
                break;
            }
        }

        case 0xF000:
        {            
            int x = (opcode & 0x0F00) >> 8;
            switch (opcode & 0x00FF)
            {                
                case 0x0007:
                {                    
                    V[x] = delay_timer;
                    pc+=2;
                    break;
                }
                case 0x000A:
                {   
                    bool keyPress = false;
                    for (int i = 0; i <16; i++)
                    {
                        if (input[i] != 0)
                        {
                            V[x] = i;
                            keyPress = true;                            
                        }
                    }

                    if (!keyPress)
                        return;                   
                   
                    pc+=2;
                    break;
                }
                case 0x0015:
                {
                    delay_timer = V[x];
                    pc+=2;
                    break;
                }
                case 0x0018:
                {
                    sound_timer = V[x];
                    pc+=2;
                    break;
                }
                case 0x001E:
                {
                    if(I + V[x] > 0xFFF)
						V[0xF] = 1;
					else
						V[0xF] = 0;                    
                    I += V[x];
                    pc+=2;
                    break;
                }

                case 0x0029:
                {
                    I = V[x] * 0x5;
                    pc+=2;
                    break;
                }

                case 0x0033:
                {
                    memory[I] = V[(opcode & 0x0F00) >> 8] / 100;
                    memory[I + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
                    memory[I + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
                    pc+=2;
                    break;
                }
                case 0x0055:
                {
                    for (int i = 0; i <= x; i++)
                    {
                        memory[I + i] = V[i];
                    }
                    I += ((opcode & 0x0F00) >> 8) + 1;
                    pc+=2;
                    break;
                }
                case 0x0065:
                {
                    for (int i = 0; i <= x; i++)
                    {
                        V[i] = memory[I + i] ;
                    }
                    I += ((opcode & 0x0F00) >> 8) + 1;
                    pc+=2;
                    break;
                }                
            }
            break;
        }

        //default:
        //    pc += 2;

        default:
            printf ("Unknown opcode: 0x%X\n", opcode);
    } 

    if (delay_timer > 0)
        delay_timer--;

    if (sound_timer > 0)
    {
        if (sound_timer == 1)
            printf("BEEP !\n");
        sound_timer--;
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
    _renderer->SetWindowTitle(filePath);
}

void chip8::DebugRender()
{
	// Draw
	for(int y = 0; y < 32; ++y)
	{
		for(int x = 0; x < 64; ++x)
		{
			if(gfx[(y*64) + x] == 0) 
				printf("O");
			else 
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

void chip8::Draw()
{
    // Draw graphics into screen
    _drawFlag = false;
    _renderer->Render(gfx);
}

void chip8::PollKeys()
{    
    _renderer->PollKeys(input);
}
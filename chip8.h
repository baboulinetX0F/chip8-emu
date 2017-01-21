#include "Renderer.h"

class chip8 {

private:
    // CHIP-8 has 35 opcodes, which are all two bytes long and stored big-endian.
    unsigned short opcode;

    // Chip-8 4K memory
    // 0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
    // 0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
    // 0x200-0xFFF - Program ROM and work RAM
    unsigned char memory[4096];

    // CHIP-8 has 16 8-bit data registers named from V0 to VF
    unsigned char V[16];

    // The address register, which is named I, is 16 bits wide
    // and is used with several opcodes that involve memory operations.
    unsigned short I;

    unsigned short stack[16];
    unsigned short sp;          // stack pointer

    // Original CHIP-8 Display resolution is 64×32 pixels
    unsigned char gfx[64*32];

    // Delay timer is intended to be used for timing the events of games. 
    // Its value can be set and read.
    unsigned char delay_timer;

    // Sound timer is used for sound effects. When its value != zero, beeping sound is made.
    unsigned char sound_timer;

    // Chip 8 has a HEX based keypad (0x0-0xF)
    unsigned char input[16];

    // program counter
    unsigned short pc;

    unsigned char chip8_fontset[80] =
    { 
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    Renderer* _renderer;
   
    void disp_clear();

public:
    chip8();
    ~chip8();

    // draw flag : if true, update screen with Draw()
    bool _drawFlag = false;     
    
    const unsigned char* getDataRegister();

     // Initialize memory, register etc...    
    void Initialize();

    // Load Program into memory
    void LoadProgram(const char* filePath);

    // Emulate a chip8 cpu cycle
    void Cycle();

    void Draw();

    void PollKeys();

    void DebugRender();
};
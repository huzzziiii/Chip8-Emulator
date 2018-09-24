//
//  chip8.hpp
//  Chip8
//

#ifndef chip8_hpp
#define chip8_hpp

#include <stdio.h>

#define MEMORY_SIZE  4096

class Chip8{
    
    unsigned short pc;          //program counter
    unsigned short sp;          //stack pointer
    unsigned short ip;          //index register
    unsigned short opcode;      //current opcode
    
    unsigned char stack[16];    //stack to store return addresses when subroutines are called
    unsigned char memory[4096]; //4K memory
    
    unsigned char gfx[64*32];   //Display resolution
    unsigned char V[16];        //16 8-bit data registers [V0-VF]
    
    public:
        Chip8();
        ~Chip8();
    
        int getPixels(int index);
        void setPixels(int index, int data);
    
        bool loadApp(const char *file);
};

#endif /* chip8_hpp */

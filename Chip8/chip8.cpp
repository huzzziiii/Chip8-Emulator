//
//  chip8.cpp
//  Chip8
//

#include "chip8.hpp"
#include <iostream>
#include <fstream>

using namespace std;

Chip8::Chip8() : memory{0}, stack{0} {
    pc = 0x200;     //starting address of the program
    sp = 0;         //initializing stack pointer to 0
    I = 0;          //initializing index pointer
    opcode = 0;     //initializing current opcode

}

Chip8::~Chip8(){
    
}

bool Chip8::loadApp(const char *filename){
    printf ("Loading: %s\n", filename);
    
    /* Open the file */
    FILE *file = fopen(filename, "rb");
    if (file == NULL){
        perror("Error opening the file\n");
        return false;
    }
    
    /* Check the file size */
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    cout <<"Size of the file: "<<size<<endl;
    
    rewind(file);       //resetting the pointer to the beginning of the file
    
    // Allocating memory to store the file
    char *buffer = new char[size];
    
    if (buffer == NULL){
        perror ("Couldn't allocate memory!");
        return false;
    }
    
    // Reading data into buffer
    size_t result = fread(buffer, 1, size, file);
    
    if (result != size){
        perror("Couldn't read in correct data from the file!");
        return false;
    }
    cout<<"Value read: "<<result<<endl;
    
    /* Copying buffer to Chip8 Memory */
    for (int i=0; i<size; i++){
        memory[i+512] = buffer[i];  //populating memory at 0x200=512
        //cout <<buffer[i]<<",";
    }
    
    /* Close file */
    fclose(file);
    
    return true;
}

void Chip8::emulateCycle(){
    /*
     - Fetch, Decode, Execute
     - all opcodes are 2 bytes long (big-endian)
     */
    
    /* Fetch opcode */
    opcode = memory[pc] << 8 | memory[pc+1];
    
    /* Process opcode */
    switch(opcode & 0xF000){
        case 0x0000:
            switch(opcode & 0x000F){
                case 0x0000:            //0x00E0: clear the screen
                    memset(gfx,0,sizeof(gfx));
                    // -- pc+=2?
                    break;
                
                case 0x000E:           //0x00EE: return from a subroutine
                    --sp;
                    pc = stack[sp];    //store the return address in PC
                    pc += 2;           //jumping to next instruction
                break;
            }
        case 0x1000:                   /* 0x1NNN: Jump to NNN address */
            pc = opcode & 0x0FFF;      //setting PC to NNN address
        break;
        
        case 0x2000:                   /* 0x2NNN: Calls subroutine at NNN */
            stack[sp] = pc;            //store current/return address in stack
            ++sp;                      //stack grows upward
            pc = opcode & 0x0FFF;      //jump to subroutine at NNN
        break;
        
        case 0x3000:                   /* 0x3XNN: Skip the next instruction if VX equals NN */
            if ( V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF) ){
                pc += 4;                //skip the next instruction
            }
            else{
                pc += 2;                //skip to next instruction
            }
        break;
        
        case 0x4000:                    /* 0x4XNN: Skip the next instruction if VX!=NN */
            if ( (V[opcode & 0x0F00 >> 8]) != (opcode & 0x00FF) ){
                pc += 4;
            }
            else{
                pc += 2;
            }
        break;
        
        case 0x5000:                    /* 0x5XY0: Skips the next instruction if VX equals VY */
            if ( (V[opcode & 0x0F00] >> 8) == V[opcode & 0x00F0] >> 4){
                pc += 4;
            }
            else{
                pc += 2;
            }
        break;
        
        case 0x6000:                    /* 0x6XNN: Set VX to NN */
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            pc += 2;
        
        case 0x7000:                    /* 0x7XNN: Add NN to VX */
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            pc += 2;
        
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000:            /* 0x8XY0: Sets VX to the value of VY */
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                    
                case 0x0001:            /* 0x8XY1: Sets VX to (VX or VY) */
                    V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                
                case 0x0002:            /* 0x8XY2: Sets VX to (VX and VY) */
                    V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                    
                case 0x0003:            /* 0x8XY3: Sets VX to (VX xor VY) */
                    V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                    
                case 0x0004:            /* 0x8XY4: Adds VY to VX: if (VX+VY>255), borrow */
                    if ( V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] > 0xFF ){
                        V[0xF] = 1;
                    }
                    else{
                        V[0xF] = 0;
                    }
                    
                    V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;

                case 0x0005:            /* 0x8XY5: VY is subtracted from VX */
                    if ( V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4] ){
                        V[0xF] = 0;
                    }
                    else{
                        V[0xF] = 1;
                    }
                    V[opcode & 0x0F00] -= V[(opcode & 0x00F0) >> 4];
                    pc += 2;
                break;
                
                case 0x0006:            /* 0x8XY6: Stores the LSb of VX in VF and then shifts VX to the right by 1 */
                    V[0xF] = V[(opcode & 0x0F00) >> 8] & 0x1;   //VX=0x93
                    V[(opcode & 0x0F00) >> 8] >>= 1;
                    pc += 2;
                break;
                
                case 0x0007:            /* 0x8XY7: Sets VX = VY - VX */
                    if ( (V[(opcode & 0x00F0)] >> 4) < V[ (opcode & 0x0F00) >> 8] ){
                        V[0xF] = 0;     //borrow
                    }
                    else{
                        V[0xF] = 1;
                    }
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    pc += 2;
                break;
                
                case 0x000E:            /* 0x8XYE: Stores the MSb of VX in VF - Left shift VX by 1*/
                    V[0xF] = V[ (opcode & 0x0F00) >> 8 ] >> 7;    //V[X]=0x85 -- 1000 0101 >> 7 = 1
                    V[ (opcode & 0x0F00) >> 8] <<= 1;
                    pc += 2;
                break;
                    
                default:
                    cout <<"Unknown opcode"<<endl;
                    break;
            }
            break;
            
        case 0x9000:                /* 9XY0: Skips the next instruction if VX doesn't equal VY */
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]){
                pc += 4;        //skip the next instruction
            }
            else{
                pc += 2;
            }
        break;
        
        case 0xA000:            /* ANNN: Sets I to the address NNN */
            I = opcode & 0x0FFF;
            pc += 2;
        break;
            
        case 0xB000:            /* BNNN: Jumps to the address NNN plus V0 */
            I = (opcode & 0x0FFF) + V[0];
            pc += 2;
        break;
        
        case 0xC000:            /* CXNN: Sets VX = NN & random number */
            V[(opcode & 0x0F00)>>8] = (opcode & 0x00FF) & (rand() & 0xFF);
            pc += 2;
        break;
        
        case 0xD000:        //*************TO BE DONE****************
        
            break;
        
        case 0xE000:            /* */
            switch (opcode & 0x00FF) {
                case 0x0090:              /*EX9E: when VX is pressed, skip the next instruction */
                    if (key[V[(opcode & 0x0F00)>>8] != 0]){
                        pc += 4;
                    }
                    else{
                        pc += 2;
                    }
                break;
                
                case 0x00A0:        /* EXA1: Skips the next instruction if the key stored in VX isn't pressed */
                    if (key[V[opcode & 0x00F0 >> 8]] == 0){
                        pc += 4;
                    }
                    else{
                        pc += 2;
                    }
                default:
                    break;
            }
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x0007:        /* FX07: Sets VX to the value of the delay timer */
                    V[(opcode & 0x0F00) >> 8] = delay_timer;
                    pc += 2;
                break;
                
                case 0x000A:        /* 0xFX0A: key press is awaited, and then stored in VX */
                     //*************TO BE DONE****************
                default:
                    break;
            }
        
        case 0x0015:        /* FX15: Sets the delay timer to VX */
            delay_timer = V[(opcode & 0x0F00) >> 8];
            pc += 2;
        break;
        
        case 0x0018:        /* FX18: Sets the sound timer to VX */
            sound_timer = V[(opcode & 0x0F00) >> 8];
            pc += 2;
        break;

        case 0x001E:        /* FX1E: Adds VX to I */
            if ( (V[(opcode & 0x0F00)>>8] + I)> 0xFFF ){ // VF is set to 1 when range overflows (I+VX>0xFFF), and 0 when there isnt
                V[0xF] = 1;
            }
            else{
                V[0xF] = 0;
            }
            I += V[(opcode & 0x0F00) >> 8];
            pc += 2;
        break;
            
        case 0x0029:        /* FX29: Sets I to the location of the sprite for the character in VX */
            I = V[(opcode & 0x0F00) >> 8] * 0x5;   // each sprite = 5 bytes
            pc += 2;
        break;
            
        case 0x0033:        /* FX33: Stores the BCD representation of VX in I */
            //E.g: 0x32 == 0011 0010 == 050
            memory[I] = V[(opcode & 0x0F00) >> 8]/100;          //0
            memory[I+1] = ( V[(opcode & 0x0F00)>>8]/10 ) % 10;  //5
            memory[I+2] =  V[(opcode & 0x00F00)>>8] % 10;       //0
            pc += 2;
            break;
        
        case 0x0055:       /* 0x0055: Stores V0 to VX (including VX) in memory starting at address I */
            for (int i=0; i<=REGISTER_LENGTH; i++){
                memory[I + i] = V[i];
            }
            I += ((opcode & 0x0F00) >> 8) + 1;
            pc += 2;
        break;
            
        case 0x0065:       /* 0x0065: Fills V0 to VX with values from memory starting at address I */
            for (int i=0; i<=REGISTER_LENGTH; i++){
                V[i] = memory[I+i];
            }
            I += ((opcode & 0x0F00) >> 8) + 1;
            pc += 2;
        break;
            
        default:
            cout <<"Unknown opcode: "<<hex<<opcode<<endl;
    }
}

int Chip8::getPixels(int index){
    return gfx[index];
}

void Chip8::setPixels(int index, int data){
    gfx[index] = data;
}



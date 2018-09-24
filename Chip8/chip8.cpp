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
    ip = 0;          //initializing index pointer
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

int Chip8::getPixels(int index){
    return gfx[index];
}

void Chip8::setPixels(int index, int data){
    gfx[index] = data;
}



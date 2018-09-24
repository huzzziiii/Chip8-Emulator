//
//  main.cpp
//  Chip8
//

#include <iostream>
#include "chip8.hpp"

int main(int argc, const char * argv[]) {
    Chip8 chip8;
    
    // Load the game into the memory
    chip8.loadApp("app.txt");
    
    
    return 0;
}

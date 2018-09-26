//
//  main.cpp
//  Chip8
//

#include <iostream>
#include "chip8.hpp"
using namespace std;
int main(int argc, const char * argv[]) {
    Chip8 chip8;
    
    // Load the game into the memory
    if(!chip8.loadApp("app.txt")){
        cout <<"Error loading the application";
        return 1;
    }
    
    
    return 0;
}

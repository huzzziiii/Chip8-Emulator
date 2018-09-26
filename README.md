# Chip-8 Emulator

Chip 8 is an interpreted programming language (more like a virtual machine), which was initially used on the Telmac 1800 8-bit microcomputers to make game programming easier.

## Memory
- Interpreter occupies the ifrst 512 bytes of memory space. Thus, programs written for the original system begin at memory location 512 (0x200)
- Uppermost 256 Bytes are reserved for display refresh
- 96 bytes below that are reserved for call stack, internal use, and other variables

## Registers
- CHIP-8 has 16 8-bit dat aregisters namely V0-VF.
- VF is for carry flag.
- Address Register (I) is 16-bit wide

## Stack
- 16 levels of stack
- Stack is used to store return addresses when subroutines are called.

## Opcodes

| Oopcode | Description | Implemented |
| ------- | ----------- | ----------- |
| 00E0    | Clears the screen | :white_check_mark:|
| 00EE    | Return from a subroutine | :white_check_mark:|
| 1NNN    | Jump to address NNN | :white_check_mark:|
| 2NNN    | Call subroutine at NNN | :white_check_mark:|
| 3XNN    | Skips the next instruction if VX == NN | :white_check_mark:|
| 4XNN    | Skips the next instruction is VX != NN | :white_check_mark:|
| 5XY0    | Skip the next instruction if VX == VY | :white_check_mark:|
| 6XNN    | VX = NN | :white_check_mark:|
| 7XNN    | Adds NN to VX | :white_check_mark:|
| 8XY0    | VX = VY | :white_check_mark:|
| 8XY1    | VX = VX OR VY | :white_check_mark:|
| 8XY2    | VX &= VY | :white_check_mark:|
| 8XY3    | VX ^= VY | :white_check_mark:|
| 8XY4    | VX += VY | :white_check_mark:|
| 8XY5    | VX -=VY | :white_check_mark:|
| 8XY6    | VX >>= 1 | :white_check_mark:|
| 8XY7    | VX=VY-VX | :white_check_mark:|
| 8XYE    | VX <<= 1 | :white_check_mark:|
| 9XY0    | Skips the next instructions if VX != VY | :white_check_mark:|
| ANNN    | I = NNN | :white_check_mark:|
| BNNN    | PC=V0+NNN | :white_check_mark:|
| CXNN    | VX = rand() & NN | :white_check_mark:|
| DXYN    | Draws a sprite at coordinate (VX,VY) that has a width of 8 pixels and a height of N pixels | :x:|
| EX9E    | Skips the next instruction if the key stored in VX is pressed | :x:|
| EXA1    | Skips the next instruction if the key stored in VX isn't pressed | :x:|
| FX07    | VX = delay_timer | :white_check_mark:|
| FX0A    | A key pressed is awaited, and then stored in VX | :x:|
| FX15    | delay_timer = VX | :white_check_mark:|
| FX18    | sound_timer = VX | :white_check_mark:|
| FX1E    | I += VX | :white_check_mark:|
| FX29    | Sets I to the location of the sprite for the character in VX | :x:|
| FX33    | Stores the BCD representation of VX | :white_check_mark:|
| FX55    | memory = VX (V0-VF) | :white_check_mark:|
| FX65    | VX = memory | :white_check_mark:|

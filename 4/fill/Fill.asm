// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

(initialization)
@8192
D=A
@screen_counter
M=D
@SCREEN
D=A
@screen_pointer
M=D

(KDB_ZERO_LOOP)
@KBD
D=M
@WHITE_FILL_LOOP
D;JEQ
@KDB_NOT_ZERO_LOOP  
0;JMP

(KDB_NOT_ZERO_LOOP)
@KBD
D=M
@BLACK_FILL_LOOP
D;JNE
@KDB_NOT_ZERO_LOOP
0;JMP

(BLACK_FILL_LOOP)
@screen_pointer 
A=M
M=-1
@screen_pointer
M=M+1        
@screen_counter
M=M-1       
D=M        
@BLACK_FILL_LOOP
D;JGT
@initialization
0;JMP

(WHITE_FILL_LOOP)
@screen_pointer 
A=M
M=0 
@screen_pointer
M=M+1        
@screen_counter
M=M-1       
D=M        
@WHITE_FILL_LOOP
D;JGT      
@initialization
0;JMP


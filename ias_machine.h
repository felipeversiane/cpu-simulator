#include <stdio.h>
#include <stdlib.h>
#include <string.h>
///////////////////////////////// DEFINE DOS OPCODES
#define hlt 0b00000
#define nop 0b00001
#define add 0b00010
#define sub 0b00011
#define mul 0b00100
#define div 0b00101
#define cmp 0b00110
#define xchg 0b00111
#define and 0b01000
#define or 0b01001
#define xor 0b01010
#define not 0b01011
#define je 0b01100
#define jne 0b01101
#define jl 0b01110
#define jle 0b01111
#define jg 0b10000
#define jge 0b10001
#define jmp 0b10010
#define lda 0b10011
#define ldb 0b10100
#define sta 0b10101
#define stb 0b10110
#define ldrb 0b10111
#define movial 0b11000
#define moviah 0b11001
#define addia 0b11010
#define subia 0b11011
#define mulia 0b11100
#define divia 0b11101
#define lsh 0b11110
#define rsh 0b11111
#define MEMORY_SIZE 154
#define filename "instructions.txt"




///////////////////////////////////////// FUNCTIONS
void machineInit();
void machineRender();
char* readFile(char* namefile);
void loadFile();
void execute();
void search();
void decode();
unsigned short int foundOpcodeWithAdress(char* token);
unsigned int foundOpcodeWithoutAdress(char *cmd);
unsigned char fragmate(unsigned int positionByte, unsigned char byte);
unsigned short int ifExist(char *str);
int isImediate(unsigned char ir);
unsigned char extractFiveMsb(unsigned short int ibr);
unsigned short int extractElevenLsb(unsigned short int ibr);








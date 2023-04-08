#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ias_machine.h"
#define cols_per_row 7
#define MAX 300
/////////////////////////////////////////////////////STRUCTS
typedef struct {
    unsigned short int  enderecoMemoria;
    unsigned char orientacao;
    unsigned char conteudoTot[40];
    unsigned char conteudoA[20];
    unsigned char comandoA[6];
    unsigned short int enderecoA;
    unsigned char conteudoB[20];
    unsigned char comandoB[6];
    unsigned short int enderecoB;
} InfoLineCommand;

typedef struct {
    unsigned short int enderecoMemoria;
    unsigned int palavra32;
    unsigned short int palavra16;
} Keyword;

////////////////////////////////////////////////////////////////// IAS
    char* instructions;
    char* message="";
    unsigned int mbr;
    unsigned short int mar,ibr,imm,pc,a,b,t;
    unsigned char ir,lr,e,l,g;
    unsigned char memory[MEMORY_SIZE];
/////////////////////////////////////////////////////////////////// INIT
void machineInit()
{
    instructions = 0x00;
    memset(memory, 0x00, MEMORY_SIZE);
    mbr = 0x00;
    mar = 0x00;
    ibr = 0x00;
    imm = 0x00;
    pc = 0x00;
    a = 0x00;
    b = 0x00;
    t = 0x00;
    ir = 0x00;
    lr = 0x00;
    e = 0x00;
    l = 0x00;
    g = 0x00;
}
////////////////////////////////////////////////////////////////// RENDER A MACHINE
void machineRender()
{
        char answer[10];
////////////////////////VArS
      printf("----------------------------------------CPU--------------------------------------------------- \n");
      printf("MBR:   0x%.08x ", mbr );
      printf("\t |");
      printf("MAR:  0x%.04x",mar);
      printf("\t         |");
      printf("IBR: 0x%.04x",ibr );
      printf("\n");
      printf("IMM:   0x%.04x",imm );
      printf("\t         |");
      printf("PC:   0x%.04x",pc);
      printf("\t         |");
      printf("A:   0x%.04x",a );
      printf("\n");
      printf("B:     0x%.04x", b);
      printf("\t         |");
      printf("T:    0x%.04x",t );
      printf("\t         |");
      printf("IR:  0x%.04x",ir);
      printf("\n");
      printf("LR:    0x%.02x",lr );
      printf("\t         |");
      printf("E:    0x%.02x ",e);
      printf("\t         |");
      printf("L:   0x%.02x", l);
      printf("\n");
      printf("G:     0x%.02x", g);
      printf("\n");
///////////////////////// INSTRUCTIOS
      printf("--------------------------------------INSTRUCTIONS----------------------------------------------\n");
      printf("\n");
      if(instructions==0b00000000) // Evitar que toda vez que a fun��o for chamada ele leia o arquivo novamente
      {
         instructions = readFile(filename);
         if (instructions != NULL)
         {
           printf("%s", instructions);
           free(instructions);
         }
      }
      else{
        instructions="The instructions have already been saved in memory\n";
        printf("%s", instructions);
      }

       printf("\n");
//////////////////////////MEMORY

      printf("--------------------------------------MEMORY----------------------------------------------------\n");
      for (int i = 0; i < MEMORY_SIZE; i++)
        {
            if (i % cols_per_row == 0)
            {
              printf("\n");
            }
            printf("[ %02x ] = 0x%.2x", i, memory[i]);
            printf("\t");
         }
        printf("\n");
        printf("-------------------------------------------------------------------------------------------------- \n");
        if(message=="")
        {
            message = "Do you wanna load the instructions into memory ? Press 'ENTER'' to continue or 'CTRL+C' to finalize the app : ";
            printf("%s",message);
            int character = getchar();

                loadFile();
                system("clear");
                machineRender();

        }



}
///////////////////////////////////////////////////////////////////// READ A FILE WITH THE INSTRUCTIONS
char* readFile(char* namefile) {
    FILE* file = fopen(namefile, "r");
    if (file == NULL) {
        printf("We cannot open the file %s\n", namefile);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long lenght = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* content = malloc(lenght + 1);
    fread(content, 1, lenght, file);
    content[lenght] = '\0';
    fclose(file);
    return content;
}
///////////////////////////////////////////////////////////// LOAD THE INSTRUCTIONS ON THE MEMORY


void loadFile()
{
    FILE *arqv;
    char lineCommand[50];
    int linesCount = 0;
    int cont = 0;
    char *token;
    char *ptrAux;
    int tamanho;

////////////////////////// Define o tamanho de linha dos arquivos
    Keyword keyword;
    InfoLineCommand infoLineCommand;


    arqv = fopen(filename, "r");
    if (arqv == NULL) {
        printf("We cannot open the file.\n");
        exit(0);
    }
    while (fgets(lineCommand, 50, arqv) != NULL) {
        linesCount++;
    }
    fclose(arqv);

    arqv = fopen(filename, "r");
    if (arqv == NULL) {
        printf("We cannot open the file.\n");
        exit(0);
    }
    while(fgets(lineCommand, 50, arqv) != NULL) {

        keyword.palavra32 = 0b0;

        token = strtok(lineCommand, ";"); //Divide a string apartir do ";"
        for (int i = 0; token != NULL; i++) {
            if(i == 0)
                infoLineCommand.enderecoMemoria = strtol(token, &ptrAux, 16);
            if(i == 1)
                infoLineCommand.orientacao = token[0];
            if(i == 2)
                strcpy(infoLineCommand.conteudoTot, token);
            token = strtok(NULL, ";");
        }

        keyword.enderecoMemoria = infoLineCommand.enderecoMemoria;
        if (infoLineCommand.orientacao == 'i'){
            token = strtok(infoLineCommand.conteudoTot, "/");//Divide as string a partir da '/'
            for (int i = 0; token != NULL; i++){
                if(i == 0)
                    strcpy(infoLineCommand.conteudoA, token);
                else
                    strcpy(infoLineCommand.conteudoB, token);
                token = strtok(NULL, "/");
            }

            if (ifExist(infoLineCommand.conteudoA) == 0) /*Necessita o endereco de memoria*/{
                token = strtok(infoLineCommand.conteudoTot, " ");
                for (int i = 0; token != NULL; i++) {
                    if(i == 0)
                        strcpy(infoLineCommand.comandoA, token);
                    else
                        infoLineCommand.enderecoA = strtol(token, &ptrAux, 16);
                    token = strtok(NULL, " ");
                }



                keyword.palavra32 = foundOpcodeWithAdress(infoLineCommand.comandoA)<<27;
                keyword.palavra32 = keyword.palavra32 | infoLineCommand.enderecoA<<16;
            }
            else { /*Nao necessita o endeco de memoria*/

                keyword.palavra32 = foundOpcodeWithoutAdress(infoLineCommand.conteudoA) << 27;
                keyword.palavra32 = keyword.palavra32 | 0b0<<16;
            }

            infoLineCommand.conteudoB[strlen(infoLineCommand.conteudoB)-1] = '\0';
            if (ifExist(infoLineCommand.conteudoB) == 0) /*Necessita o endereco de memoria*/{
                token = strtok(infoLineCommand.conteudoB, " ");
                for (int i = 0; token != NULL; i++) {
                    if(i == 0)
                        strcpy(infoLineCommand.comandoB, token);
                    else
                        infoLineCommand.enderecoB = strtol(token, &ptrAux, 16);
                    token = strtok(NULL, " ");
                }

                keyword.palavra32 = keyword.palavra32 | foundOpcodeWithAdress(infoLineCommand.comandoB) <<11;
                keyword.palavra32 = keyword.palavra32 | infoLineCommand.enderecoB<<0;;
            }
            else { /*Nao necessita o endereco de memoria*/

                keyword.palavra32 = keyword.palavra32 | foundOpcodeWithoutAdress(infoLineCommand.conteudoB) <<11;
                keyword.palavra32 = keyword.palavra32 | 0b0<<0b0;
            }

            memory[keyword.enderecoMemoria] = keyword.palavra32 >> 24;
            memory[keyword.enderecoMemoria+1] = keyword.palavra32 >> 16;
            memory[keyword.enderecoMemoria+2] = keyword.palavra32 >> 8;
            memory[keyword.enderecoMemoria+3] = keyword.palavra32 >> 0;

        }
        else { //Para quando a linha de comando ou dado
            keyword.palavra16 = strtol(infoLineCommand.conteudoTot, &ptrAux, 16);
            memory[keyword.enderecoMemoria] = keyword.palavra16>>8;
            memory[keyword.enderecoMemoria+1] = keyword.palavra16>>0;

        }
        cont++;
    }

    //Finalizando!
    fclose(arqv);
}
///////////////////////////////////////////////////////////////////// ifExist
unsigned short int ifExist(char *str) {
    if (strcmp(str, "hlt") == 0 || strcmp(str, "nop") == 0 ||
        strcmp(str, "add") == 0 || strcmp(str, "sub") == 0 ||
        strcmp(str, "mul") == 0 || strcmp(str, "div") == 0 ||
        strcmp(str, "cmp") == 0 || strcmp(str, "xchg") == 0 ||
        strcmp(str, "and") == 0 || strcmp(str, "or") == 0 ||
        strcmp(str, "xor") == 0 || strcmp(str, "not") == 0) {
        return 1; // true
    } else {
        return 0; // false
    }
}
/////////////////////////////////////////////////////////////////// FOUND OPCODE WITH ADRESS
unsigned short int foundOpcodeWithAdress(char* token) {

    if (strcmp(token, "je") == 0) {
        return je;
    }
    if (strcmp(token, "jne") == 0) {
        return jne;
    }
    if (strcmp(token, "jl") == 0) {
        return jl;
    }
    if (strcmp(token, "jle") == 0) {
        return jle;
    }
    if (strcmp(token, "jg") == 0) {
        return jg;
    }
    if (strcmp(token, "jge") == 0) {
        return jge;
    }
    if (strcmp(token, "jmp") == 0) {
        return jmp;
    }
    if (strcmp(token, "lda") == 0) {
        return lda;
    }
    if (strcmp(token, "ldb") == 0) {
        return ldb;
    }
    if (strcmp(token, "sta") == 0) {
        return sta;
    }
    if (strcmp(token, "stb") == 0) {
        return stb;
    }
    if (strcmp(token, "ldrb") == 0) {
        return ldrb;
    }
    if (strcmp(token, "movial") == 0) {
        return movial;
    }
    if (strcmp(token, "moviah") == 0) {
        return moviah;
    }
    if (strcmp(token, "addia") == 0) {
        return addia;
    }
    if (strcmp(token, "subia") == 0) {
        return subia;
    }
    if (strcmp(token, "mulia") == 0) {
        return mulia;
    }
    if (strcmp(token, "divia") == 0) {
        return divia;
    }
    if (strcmp(token, "lsh") == 0) {
        return lsh;
    }
    if (strcmp(token, "rsh") == 0) {
        return rsh;
    }
    return -1;
}
////////////////////////////////////////////////////////////////// FOUND OPCODE WITHOUT ADRESS
unsigned int foundOpcodeWithoutAdress(char *cmd) {
    if(strcmp(cmd, "hlt") == 0) {
        return hlt;
    }
    if(strcmp(cmd, "nop") == 0) {
        return nop;
    }
    if(strcmp(cmd, "add") == 0) {
        return add;
    }
    if(strcmp(cmd, "sub") == 0) {
        return sub;
    }
    if(strcmp(cmd, "mul") == 0) {
        return mul;
    }
    if(strcmp(cmd, "div") == 0) {
        return div;
    }
    if(strcmp(cmd, "cmp") == 0) {
        return cmp;
    }
    if(strcmp(cmd, "xchg") == 0) {
        return xchg;
    }
    if(strcmp(cmd, "and") == 0) {
        return and;
    }
    if(strcmp(cmd, "or") == 0) {
        return or;
    }
    if(strcmp(cmd, "xor") == 0) {
        return xor;
    }
    if(strcmp(cmd, "not") == 0) {
        return not;
    }
    return -1;
}
////////////////////////////////////////////////////////////////// FRAGMATE
unsigned char fragmate(unsigned int positionByte, unsigned char byte) {

    if (positionByte == 0){
        return byte >> 24;
    }
    if (positionByte == 1) {
        return byte >> 16;
    }
    if (positionByte == 2) {
        return byte >> 8;
    }
    if (positionByte == 3) {
        return byte >> 0;
    }
}
/////////////////////////////////////////////////////////////////// THE MBR IS IMEDIATE ?
int isImediate(unsigned char ir)
{
    unsigned char token = ir & 0b00011111;
    if(token==movial)
        return 1;
    if(token==moviah)
        return 1;
    if(token==addia)
        return 1;
    if(token==subia)
        return 1;
    if(token==divia)
        return 1;
    if(token==mulia)
        return 1;
    else
    {
       return 0;
    }

}
////////////////////////////////////////////////////////////////////////RETURN 5MSB
unsigned char extractFiveMsb(unsigned short int ibr) {

    unsigned char ir = (ibr & 0xF800) >> 11;
    return ir;
}
////////////////////////////////////////////////////////////////////  RETURN 11LSB
unsigned short int extractElevenLsb(unsigned short int ibr) {
    unsigned short int mar = ibr & 0x7FF;
    return mar;
}
////////////////////////////////////////////////////////////////// SEARCH
void search() {

    if(lr==0)
    {
       mar = pc;

        int i;
        for(i = mar; i < mar + 4; i++)
        {
            mbr = (mbr << 8) | memory[i];
        }


    }

}

////////////////////////////////////////////////////////////////// DECODE
void decode()
{
    if(lr==0)
    {
        ibr = mbr & 0xFFFF;
        mbr = mbr >> 16;
        ir = (mbr >> 11) & 0x1F;
        mbr = mbr & 0x7FF;
        if(isImediate(ir)==1)
        {
            imm=mbr;
        }
        else
        {
            mar=mbr;
        }
    }
    if(lr==1)
    {
        ir= extractFiveMsb(ibr);

        if(isImediate(ir)==1)
        {
            imm=extractElevenLsb(ibr);
        }
        else
        {
            mar=extractElevenLsb(ibr);
        }
    }

}
//////////////////////////////////////////////////////////////// RUN
void execute()
{
        if(ir==hlt)
            {

                printf("\n\n");
                printf("----------------------------------------------------------- \n");
                printf("    - hlt was found - The app has ben finished \n");
                printf("----------------------------------------------------------- \n");
                printf("\n");
                while(1){

                }
            }
            else if(ir==nop)
            {
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }
            else if(ir==add)
            {
                a = a + b;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }
            else if(ir==sub)
            {
                a = a - b;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }
            else if(ir==mul)
            {
                a=a*b;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }
            else if(ir==div)
            {
                a = a/b;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }else if(ir==cmp)
            {
                if (a == b) {
                    e = 1;
                } else {
                    e = 0;
                }
                if (a < b) {
                    l = 1;
                } else {
                    l = 0;
                }
                if (a > b) {
                    g = 1;
                } else {
                    g = 0;
                }
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;

            }else if(ir==xchg)
            {
                t = a;
                a = b;
                b = t;

                if(lr==1){
                    pc += 4;
                }

                lr = !lr;


            }else if(ir==and)
            {
                a=a&b;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }else if(ir==or)
            {
                a= a | b;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }else if(ir==xor)
            {
                a = a ^ b;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }else if(ir==not)
            {
                a = !a;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
        //////////////////////////// JUMPS
            }else if(ir==je)
            {
                if(e==1)
                {
                    if(lr==0){pc=mar;}
                    if(lr==1){pc=mar;lr=!lr;}
                }else{
                    if(lr==1){
                        pc+=4;
                    }
                    lr=!lr;
                }



            }else if(ir==jne)
            {
                if(e==0)
                {
                    if(lr==0){pc=mar;}
                    if(lr==1){pc=mar;lr=!lr;}
                }else{
                    if(lr==1){
                        pc+=4;

                    }
                    lr=!lr;
                }



            }else if(ir==jl)
            {
                if(l==1)
                {
                    if(lr==0){pc=mar;}
                    if(lr==1){pc=mar;lr=!lr;}
                }
                else{
                    if(lr==1){
                        pc+=4;

                    }
                    lr=!lr;
                }

            }else if(ir==jle)
            {
                if(l==1 || e==1)
                {
                    if(lr==0){pc=mar;}
                    if(lr==1){pc=mar;lr=!lr;}
                }
                else{
                    if(lr==1){
                        pc+=4;

                    }
                    lr=!lr;
                }


            }else if(ir==jg)
            {
                if(g==1)
                {
                    if(lr==0){pc=mar;}
                    if(lr==1){pc=mar;lr=!lr;}

                }
                else{
                    if(lr==1){
                        pc+=4;

                    }
                    lr=!lr;
                }

            }else if(ir==jge)
            {
                if(g==1 || e==1)
                {
                    if(lr==0)
                    {
                      pc = mar;
                      lr=0;
                    }
                    if(lr==1)
                    {
                        pc=mar;
                        lr=!lr;
                    }


                }
                else{
                    if(lr==1){
                        pc+=4;

                    }
                    lr=!lr;
                }


            }else if(ir==jmp)
            {
                if(lr==0)
                {
                    pc=mar;
                    lr=0;
                }
                if(lr==1){
                    pc=mar;
                    lr=!lr;
                }



            }else if(ir==lda)
            {
                mbr = (memory[mar] << 8);
                mbr = mbr |memory[mar+1];
                a = mbr & 0xffff;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }else if(ir==ldb)
            {
                mbr = (memory[mar] << 8);
                mbr = mbr |memory[mar+1];
                b = mbr & 0xffff;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }else if(ir==sta)
            {
                mbr = a;
                memory[mar] = (mbr >> 8) & 0xFF;
                memory[mar+1] = mbr & 0xFF;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;

            }else if(ir==stb)
            {
                mbr = b;
                memory[mar] = (mbr >> 8) & 0xFF;
                memory[mar+1] = mbr & 0xFF;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;

            }else if(ir==ldrb)
            {
                if(b<=154)
                {
                   mbr = memory[b] << 8 ;
                   mbr = mbr | memory[b+1];
                   a=mbr;
                }
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
        /////////////////////////////// IMEDIATO
            }else if(ir==movial)
            {
                a=0;
                a |= (imm & 0xFF);
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;

            }else if(ir==moviah)
            {
                a = (a & 0x00FF) | ((imm & 0x00FF) << 8);
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;

            }else if(ir==addia)
            {
                a = a + imm;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }else if(ir==subia)
            {
                a = a - imm;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }else if(ir==mulia)
            {

                a = a*imm;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }else if(ir==divia)
            {
                a = a/imm;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }else if(ir==lsh)
            {
                a = a << imm;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }else if(ir==rsh)
            {
                a = a >> imm;
                if(lr==1){
                    pc += 4;
                }

                lr = !lr;
            }
            else{
                printf("\n");
                printf("----------------------------------------------------------- \n");
                printf("      The opcode has not been detected \n");
                printf("----------------------------------------------------------- \n");
                printf("\n");
                exit(0);

            }
            system("clear");
}






























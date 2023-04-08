#include <stdio.h>
#include <stdlib.h>
#include "ias_machine.c"
#include "ias_machine.h"

int main()
{
    int character;
    machineInit();
    while(1){
        machineRender();
        printf("Press 'ENTER' to execute the next instruction or 'CTRL+C' to finalize the app :");
        character = getchar();
            search();
            decode();
            execute();


    }

}

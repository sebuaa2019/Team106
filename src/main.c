#include <stdio.h>
#include <pthread.h>
#include "MasterControl.h"



int main()
{
    int * retThread = MasterControl();



    printf("program exit\n");
    return 0;
}
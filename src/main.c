#include <stdio.h>
#include <pthread.h>
#include "MasterControl.h"


extern int programEnd;

int main()
{
    int * retThread = MasterControl();

    while(1) {
        vxsleep(1000);
        if(programEnd == 1) {
            taskDelete(retThread[0]);
            taskDelete(retThread[1]);
            taskDelete(retThread[2]);
            taskDelete(retThread[3]);
            /*
            taskDelete(retThread[4]);
             */
            taskDelete(retThread[5]);
            taskDelete(retThread[6]);
            break;
        }
    }

    printf("program exit\n");
    return 0;
}
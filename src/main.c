#include <stdio.h>
#include <pthread.h>
#include "MasterControl.h"



int main()
{
    pthread_t ** pthreadRetArray = MasterControl();

    pthread_join(*pthreadRetArray[0], NULL);
    pthread_join(*pthreadRetArray[1], NULL);
    pthread_join(*pthreadRetArray[2], NULL);


    return 0;
}
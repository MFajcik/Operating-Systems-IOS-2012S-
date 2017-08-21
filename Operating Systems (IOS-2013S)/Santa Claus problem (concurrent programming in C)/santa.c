#include "baseinfo.h"

pid_t start_santa ()
{
   pid_t pid = fork();
   if ( pid == -1 )
   {
       perror("ERROR(fork)");
       return -1;
   }
   else if ( pid == 0 )
   {
       sem_wait(sem10);
       fprintf(pFile,"%lu: santa: started\n",++elfcount->counter);
       sem_post(sem10);
       sem_post(sem1);//santa has started, parent process can continue
   }
   return pid;
}
int santahelp (long s)
{
    sem_wait(sem10);
    fprintf(pFile,"%lu: santa: checked state: %d: %d\n",++elfcount->counter,elfcount->active,elfcount->waiting);
    fprintf(pFile,"%lu: santa: can help\n",++elfcount->counter);
    sem_post(sem10);
    unsigned int *seedp;
    if ((seedp=malloc(sizeof(unsigned int)))==NULL) //seed for random
        return -1;                                  //
    *seedp=(getpid()^time(NULL));
    if ( s>0 )
        if (elfcount->status==1)
            for (int i=1;i<=3;i++)
                if (serveelf(s,seedp)==-1) //serving 3 elves
                    return -1;
    else
        if (serveelf(s,seedp)==-1) //serving elf
            return -1;


    sem_post(sem2);//elf was served
    if (elfcount->active<=3)
       sem_wait(sem6);//waiting for synch with elf
    return EXIT_SUCCESS;
}

int serveelf(int s,unsigned int * seedp)
{
    if (usleep(((rand_r(seedp)%(s))+1)*1000)==-1)
        return -1;
    return EXIT_SUCCESS;
}

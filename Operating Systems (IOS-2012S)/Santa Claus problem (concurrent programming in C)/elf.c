#include "baseinfo.h"

pid_t start_elf(long a)
{
        pid_t pid = fork();//elf creation
        if ( pid == -1 )
        {
            perror("ERROR(fork)");
            return -1;
        }
        else if ( pid == 0 )
        {
            sem_wait(sem10);
            fprintf(pFile,"%lu: elf: %lu: started\n",a,++elfcount->counter);
            sem_post(sem10);
            sem_post(sem1);//elf has started, next elf can start
        }
   return pid;
}
int elfwork(int h, unsigned int * seedp)
{   //MAN: rand() is not reentrant or thread-safe..using rand_r
    if (usleep(((rand_r(seedp)%(h))+1)*1000)==-1)
        return -1;
    return EXIT_SUCCESS;
}
void askhelp(int elfid,int i, long c)
{
    sem_wait(sem10);
    fprintf(pFile,"%lu: elf: %d: asked for help\n",++elfcount->counter,elfid);
    elfcount->waiting++;
    sem_post(sem10);
    if (elfcount->active>3)
    {
        sem_wait(sem10);
        elfcount->served++;
        sem_post(sem10);

        sem_wait(sem12);//without sem12, two processes may at time go  into next condition, synch neeeded
        if (elfcount->waiting==3)
           {
               sem_post(sem3); ///elf waits for santas help
               sem_wait(sem2);///=elf was served
               sem_wait(sem10);
               elfcount->waiting=0;
               sem_post(sem10);
               sem_post(sem1); //when third process come,
               sem_post(sem1); // hes served(3x longer) and other processes are released
               sem_post(sem12);
           }
        else
        {
            sem_post(sem12);
            sem_wait(sem1);//waiting for third elf
        }
        sem_wait(sem10);
        fprintf(pFile,"%lu: elf: %d: got help\n",++elfcount->counter,elfid);
        sem_post(sem10);
        if (i==c)//in last elf round, elfcount->active will synch with santa (so he can end properly)
        {
           sem_wait(sem10);
           fprintf(pFile,"%lu: elf: %d: got a vacation\n",++elfcount->counter,elfid);
           elfcount->active--;
           sem_post(sem10);
        }
    }
    else
    {
        sem_post(sem3); ///elf waits for santas help
        sem_wait(sem2);///=elf was served
        sem_wait(sem10);
        fprintf(pFile,"%lu: elf: %d: got help\n",++elfcount->counter,elfid);
        elfcount->waiting--;
        sem_post(sem10);

        if (i==c)//in last elf round, elfcount->active will synch with santa (so he can end properly)
        {
           sem_wait(sem10);
           fprintf(pFile,"%lu: elf: %d: got a vacation\n",++elfcount->counter,elfid);
           elfcount->active--;
           sem_post(sem10);
        }
        sem_post(sem6);
    }
}

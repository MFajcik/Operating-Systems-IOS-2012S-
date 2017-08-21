#include "baseinfo.h"

void strtol10_with_errorcheck(char* str,long* var) //string > noumber converting function
{
   errno = 0;
   char* endptr;
   *var=strtol(str,&endptr,10);
   /* Conversion error check */
   if (errno == ERANGE || (errno !=0 && *var == 0))
   {
      perror("ERROR: strtol");
      exit(1);
   }
   return;
}

void cleanup (void)
{
    if (fclose(pFile)==EOF){perror("ERROR(fclose)");}
    SEMCLOSE //macro
    SEMUNLINK //macro
    shmdt((void *) elfcount);
    if (shmctl(shmid, IPC_RMID, NULL)== -1)
    {
       perror("ERROR(shmctl)");
       exit(2);
    }
}

void kill_me(int sig)
{
    if (pid!=0)
    {
        fprintf(stderr,"ERROR: Signal %d trapped!\n Cleaning and exiting... \n",sig);
        cleanup();
        kill(0,SIGTERM);///Parent trapped signal, killing all remaining children..
    }
    exit(2);
}

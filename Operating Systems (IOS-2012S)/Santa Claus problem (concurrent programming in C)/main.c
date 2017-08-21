#include "baseinfo.h"

int main(int argc, char **argv)
{
   const pid_t PPID=getpid();
   if ( argc < 5 )
   {
      fprintf(stderr,"ERROR: Unexpected argument count (less then 5)\n");
      return EXIT_FAILURE;
   }
/** \brief */
   long c;/**je počet návštěv u Santy, které musí skřítek podstoupit, než odjede na dovolenou.
          Pro všechny skřítky je počet návštěv stejný. C > 0*/
   long e;/**je počet skřítků, kteří na začátku pracují na výrobě hraček. E > 0.*/
   long h;/**je maximální hodnota doby (v milisekundách) výroby hraček skřítkem, než se vyskytne problém; poté potřebuje skřítek pomoc. H >= 0.*/
   long s;/**je maximální hodnota doby (v milisekundách) obsluhy skřítků Santou (délka trvání pomoci). S >= 0.*/
   //conversion...
   strtol10_with_errorcheck(argv[1],&c);
   if ( c<=0 )
   {
       fprintf(stderr,"ERROR: Santa must visit elfs at least once\n");
       return EXIT_FAILURE;
   }

   strtol10_with_errorcheck(argv[2],&e);
   if ( e<=0 )
   {
       fprintf(stderr,"ERROR: At least one elf must work\n");
       return EXIT_FAILURE;
   }

   strtol10_with_errorcheck(argv[3],&h);
   if ( h<0 )
   {
       fprintf(stderr,"ERROR: Time until elf needs help must be greater or equal than zero\n");
       return EXIT_FAILURE;
   }

   strtol10_with_errorcheck(argv[4],&s);
   if ( s<0 )
   {
       fprintf(stderr,"ERROR: Time which elfs require from Santa must be greater or equal than zero\n");
       return EXIT_FAILURE;
   }

   //singal handlers == safe program exiting
   signal(SIGTERM, kill_me);
   signal(SIGINT, kill_me);
   if ((pFile=fopen ("santa.out","w"))==NULL)
       {
           perror("ERROR(fopen)");
           return 2;
       }
   setbuf(pFile, NULL);
   if ( (sem1 = sem_open("/semafor1", O_CREAT | O_EXCL, 0666, 0))== SEM_FAILED)
   {
	   perror("ERROR(Semaphore1):");
       sem_unlink("/semafor1");
       return 2;
   }
   if ( (sem2 = sem_open("/semafor2", O_CREAT | O_EXCL, 0666, 0))== SEM_FAILED)
   {
	   perror("ERROR(Semaphore2):");
       sem_unlink("/semafor2");
       return 2;
   }
   if ( (sem3 = sem_open("/semafor3", O_CREAT | O_EXCL, 0666, 0))== SEM_FAILED)
   {
	   perror("ERROR(Semaphore3):");
       sem_unlink("/semafor3");
       return 2;
   }
   if ( (sem5 = sem_open("/semafor5", O_CREAT | O_EXCL, 0666, 1))== SEM_FAILED)
   {
	   perror("ERROR(Semaphore5):");
       sem_unlink("/semafor5");
       return 2;
   }
   if ( (sem6 = sem_open("/semafor6", O_CREAT | O_EXCL, 0666, 0))== SEM_FAILED)
   {
	   perror("ERROR(Semaphore6):");
       sem_unlink("/semafor6");
       return 2;
   }
   if ( (sem7 = sem_open("/semafor7", O_CREAT | O_EXCL, 0666, 1))== SEM_FAILED)
   {
	   perror("ERROR(Semaphore7):");
       sem_unlink("/semafor7");
       return 2;
   }
   if ( (sem8 = sem_open("/semafor8", O_CREAT | O_EXCL, 0666, 0))== SEM_FAILED)
   {
	   perror("ERROR(Semaphore8):");
       sem_unlink("/semafor8");
       return 2;
   }
   if ((sem10 = sem_open("/semafor10", O_CREAT | O_EXCL, 0666, 1))== SEM_FAILED)
   {
	   perror("ERROR(Semaphore10):");
       sem_unlink("/semafor10");
       return 2;
   }
   if ((sem12 = sem_open("/semafor12", O_CREAT | O_EXCL, 0666, 1))== SEM_FAILED)
   {
	   perror("ERROR(Semaphore12):");
       sem_unlink("/semafor12");
       return 2;
   }
   int elfid;//id of an elf
   // allocating shared memory space for noumber of active elfs >var elfcount
   key_t shm_key = ftok ("./santa",2);
   if (shm_key==-1)
   {
       perror("ERROR(ftok)");
       SEMCLOSE //macro
       SEMUNLINK //macro
       return 2;
   }
   if ((shmid=shmget(shm_key,sizeof(unsigned int),IPC_CREAT | IPC_EXCL |0666))==-1)
   {
       perror("ERROR(shmget)");
       SEMCLOSE //macro
       SEMUNLINK //macro
       return 2;
   }
   elfcount= ( data_t*) shmat(shmid,NULL,0);
   if ((void*)elfcount==(void*)-1)
   {
       perror("ERROR(shmat)");
       SEMCLOSE //macro
       SEMUNLINK //macro
       return 2;
   }
   elfcount->served=0;
   if (e>3)
   {
       for (int i=1;i<3;i++) ///sem_value=3
           sem_post(sem5);
       elfcount->status=1;//if elves are more than 3, program changes behaviour
   }

  /**
   ** elves are starting ...
   **/
   for ( long i=1; i<=e; i++)
   {
           pid=start_elf(i);
           if (pid==0)
           {
             elfid=i;
             elfcount->active++;//elf is active
             break;
           }
           else if (pid<0)
           {
             kill(PPID,SIGTERM);
           }
           sem_wait(sem1);//awaiting start of elf
   }
  /**
   ** elf work
   **/
   if ( pid == 0 )
   {
       sem_wait(sem2);
       //SEED for ranr_r function in elfwork(), must be out of cycle
       unsigned int *seedp;
       if ((seedp=malloc(sizeof(unsigned int)))==NULL)
           {
               perror("ERROR(malloc)");
               kill(PPID,SIGTERM);
               return 2;
           }
       *seedp=(getpid());
       //----------------------
       int i;
       for (i=1 ;i<=c;i++)
       {   /// elf work algorithm is simulated by usleep()
          if (h>0){
               if (elfwork(h,seedp)==-1)
               {
                  perror("ERROR(usleep)");
                  free(seedp);
                  kill(PPID,SIGTERM);
                  return 2;
               }
          }
           sem_wait(sem10);
           fprintf(pFile,"%lu: elf: %d: needed help\n",++elfcount->counter,elfid);
           sem_post(sem10);
           sem_wait(sem7); //critical area, switch from serving 3o3 to 1o1
           if (elfcount->active<=3 && elfcount->status==1)
           {
               elfcount->status=2;
               sem_wait(sem5);
               sem_wait(sem5);
           }
           sem_post(sem7);

           sem_wait(sem5);//elf waits b4 asking
           askhelp(elfid,i,c);
           sem_wait(sem7);
           if (elfcount->served==1)
           {
               sem_wait(sem10);
               elfcount->served=0;
               sem_post(sem10);
               for (int i=1;i<=3;i++)//when third elf come out,he will release
                   sem_post(sem5);  // next three can come
           }
           else if (elfcount->served>1)
           {
               sem_wait(sem10);
               elfcount->served--;
               sem_post(sem10);
           }
           else
               sem_post(sem5);
           sem_post(sem7);

       }
       free (seedp);
       ///elfs on vacation
       if (elfcount->active==0)
           for (int i=1;i<=e;i++)
               sem_post(sem8);//last elf got vacation, elfs could finish now
       sem_wait(sem8);//elfs waiting 'on vacation'
       ///----------------
       SEMCLOSE//macro
       if (shmdt((void *) elfcount))
       {
              perror("ERROR(shmdt)");
              kill(getpid(),SIGTERM);
       }
       if (fclose(pFile)==EOF)
              kill(getpid(),SIGTERM);
       return elfid;
   }
  /**
   ** santa
   **/
   if (pid>0)
   {
       if ((pid=start_santa())==2)
           {
               perror("ERROR(fork)");
               kill(getpid(),SIGTERM);
               return 2;
           }
       if (pid>0)
       {
           sem_wait(sem1);//waiting for santa start (elfs do no work)
           for (int i=1;i<=e;i++)
               sem_post(sem2);//santa has started, elfs could start to work now
       }
       //serving of elves
       if (pid==0)
       {
          while (elfcount->active >0)
          {
              sem_wait(sem3);
              if (santahelp(s)==-1)
              {
                  perror("ERROR(santahelp)");
                  kill(getpid(),SIGTERM);
                  return -e-1;
              }
          }
          sem_wait(sem10);
          fprintf(pFile,"%lu: santa: checked state: %d: %d\n",++elfcount->counter,elfcount->active,elfcount->waiting);
          sem_post(sem10);
          SEMCLOSE
          fclose(pFile);
          if (shmdt((void *) elfcount))
          {
              perror("ERROR(shmdt)");
              return (-e-1);
          }
          return 0;
       }
   }
   int status;
   bool err=false;
   for (int i=0; i<=e; i++)//parent is waiting for termination of childs
   {                   //and their status (elf term. status=elfid)
       wait(&status);
       sem_wait(sem10);
       if (WEXITSTATUS(status)==0 )
           fprintf(pFile,"%lu: santa: finished\n",++elfcount->counter);
       else if (WEXITSTATUS(status)>0)
           fprintf(pFile,"%lu:  elf: %d: finished\n",++elfcount->counter,WEXITSTATUS(status));
       else if (WEXITSTATUS(status)==(-e-1))
       {
           fprintf(pFile,"%lu: santa: finished\n",++elfcount->counter);
           err=true;
       }
       else
       {
           fprintf(pFile,"%lu:  elf: %d: finished\n",++elfcount->counter,abs(WEXITSTATUS(status)));
           err=true;
       }
       sem_post(sem10);
   }
   cleanup();
   if (err==true)
       return 2;
   return 0;
}

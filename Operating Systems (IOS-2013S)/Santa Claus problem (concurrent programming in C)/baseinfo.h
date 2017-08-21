#ifndef BASEINFO_H_INCLUDED
#define BASEINFO_H_INCLUDED

#define _SVID_SOURCE // ipc.h requires this
#define _BSD_SOURCE //usleep() requires this
#define _POSIX_SOURCE // rand_r() equires this

#define SEMUNLINK {\
sem_unlink("/semafor1");\
sem_unlink("/semafor2");\
sem_unlink("/semafor3");\
sem_unlink("/semafor5");\
sem_unlink("/semafor6");\
sem_unlink("/semafor7");\
sem_unlink("/semafor8");\
sem_unlink("/semafor10");\
sem_unlink("/semafor12");\
}

#define SEMCLOSE {\
sem_close(sem1);\
sem_close(sem2);\
sem_close(sem3);\
sem_close(sem5);\
sem_close(sem6);\
sem_close(sem7);\
sem_close(sem8);\
sem_close(sem10);\
sem_close(sem12);\
}

#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>
#include <errno.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <signal.h>

#include <semaphore.h>
#include <fcntl.h>
#include <stdbool.h>
typedef struct data{
unsigned int active;
unsigned int waiting;
unsigned short status;
unsigned long counter;
unsigned short served;
}data_t;

pid_t pid; //process ID
data_t *elfcount; // shared memory> pointer
FILE * pFile;//outfile
int shmid;//id of shared memory
sem_t *sem1, *sem2, *sem3, *sem5, *sem6, *sem7, *sem8, *sem10, *sem12;
/**************** QUICK SEMAPHORES GUIDE ***************
 ** sem1 used for starting of elves and santa in proper order (stoppin main process)
 **          and stopping elves from waking up santa, until there is three of them
 ** sem2 used for stopping elves from work until santa starts
 ** sem3 santa waits on this semaphore until elves dont wake him up
 ** sem5 elves who 'needed help' are waiting here
 ** sem6 used for synchronization of santa with elf (so can end properly>
 **          he can see after synch that elfcount->active==0, otherwise he ll enter another cycle
 **          before the value is decremented)
 ** sem7 used for exlusive access to critical area, when santa's way of serving is changed
 **          by elfcount->active value lower or equal than three
 ** sem8 used for elves who are waiting on vacation
 ** sem10 used to allow processes exlusive access to outfile and shared memory
 ** sem12 fixes a bug when two processes can time to time get into condition at once and start santa serving two times
 **/

int serveelf(int s,unsigned int * seedp);
void askhelp(int elfid,int i, long c);
int santahelp (long s);
int elfwork(int h, unsigned int * seedp);
void strtol10_with_errorcheck(char* str,long* var);
pid_t start_elf(long a);
pid_t start_santa ();
void cleanup ();
void kill_me(int sig);
#endif // BASEINFO_H_INCLUDED

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define SIZE 16

#define sync(sem_id)							\
  struct sembuf __temp_sem_buff__;					\
  int __ignore_me_at_all_costs__ = 0;					\
  for(__ignore_me_at_all_costs__ = (((__temp_sem_buff__.sem_num = 0) |	\
				     (__temp_sem_buff__.sem_op = -1) |	\
				     (__temp_sem_buff__.sem_flg = 0) |	\
				     semop(sem_id, &__temp_sem_buff__, 1)) & 0); \
      __ignore_me_at_all_costs__ != 1;					\
      __ignore_me_at_all_costs__ = (((__temp_sem_buff__.sem_num = 0) |	\
				     (__temp_sem_buff__.sem_op =  1) |	\
				     (__temp_sem_buff__.sem_flg = 0) |	\
				     semop(sem_id, &__temp_sem_buff__, 1)) & 0) | 1) \

#define sync2(sem_id)							\
  for(struct sembuf __temp_sem_buff__ = {.sem_num = 0, .sem_op = -1, .sem_flg = 0}; \
      __temp_sem_buff__.sem_op != 1;					\
      (__temp_sem_buff__.sem_op = 1) |  )

struct foo {int a; int b;};
void swap_safe(int sem_id, long int* a, long int* b) {
  sync(sem_id) {
    long int t = *a;
    *a = *b;
    *b = t;
  } 
}
int main (int argc, char* argv[]) 
{ 
   int status; 
   long int i, loop, *shmPtr; 
   int shmId; 
   pid_t pid;

   union semun sem_val;
   int sem_id = semget (IPC_PRIVATE, 1, 00600);
   
   sem_val.val = 1;
   semctl(sem_id, 0, SETVAL, sem_val);

   

   // get value of loop variable (from command-line argument)
   loop = (argc > 1) ? atoi(argv[1]) : -1;
   if (loop <= 0) {
     printf("Bad args\n");
     exit(-1);
   }
   printf("%li\n", loop);
   if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
     perror ("i can't get no..\n"); 
     exit (1); 
   } 
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) { 
     perror ("can't attach\n"); 
     exit (1); 
   }

   shmPtr[0] = 0; 
   shmPtr[1] = 1;

   if (!(pid = fork())) { 
     for (i=0; i<loop; i++) { 
       // swap the contents of shmPtr[0] and shmPtr[1] 
       swap_safe(sem_id, shmPtr, shmPtr+1);
     } 
     if (shmdt (shmPtr) < 0) { 
       perror ("just can't let go\n"); 
       exit (1); 
     } 
     exit(0); 
   } 
   else 
     for (i=0; i<loop; i++) { 
       // swap the contents of shmPtr[1] and shmPtr[0] 
       swap_safe(sem_id, shmPtr+1, shmPtr);
     }

   wait (&status); 
   semctl(sem_id, 0, IPC_RMID);

   printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

   if (shmdt (shmPtr) < 0) { 
     perror ("just can't let go\n"); 
     exit (1); 
   } 
   if (shmctl (shmId, IPC_RMID, 0) < 0) { 
     perror ("can't deallocate\n"); 
     exit(1); 
   }

   return 0; 
} 
 

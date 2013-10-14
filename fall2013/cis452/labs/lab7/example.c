/*
 * sem-mutex.c  - demonstrates the usage of a semaphore as a mutex that
 *                that synchronizes accesses of multiple processes
 *                to a file.
 */

#include <stdio.h> /* standard I/O routines.              */
#include <stdlib.h>      /* rand() and srand() functions        */
#include <unistd.h> /* fork(), etc.                        */
#include <time.h> /* nanosleep(), etc.                   */
#include <sys/types.h>   /* various type definitions.           */
#include <sys/ipc.h>     /* general SysV IPC structures         */
#include <sys/sem.h> /* semaphore functions and structs.    */
#include <sys/wait.h> /* wait(), etc.                        */

#define NUM_PROCS  5 /* number of processes to launch.      */
#define SEM_ID    250 /* ID for the semaphore.               */
#define FILE_NAME "sem_mutex"  /* name of file to manipulate    */
#define DELAY     400000 /* delay between file updates by one process. */


/* this function updates the contents of the file with the given path name. */
void update_file(int sem_set_id, char* file_path, int number)
{
  /* structure for semaphore operations.   */
    struct sembuf sem_op;
    FILE* file;

    /* wait on the semaphore, unless it's value is non-negative. */
    sem_op.sem_num = 0;
    sem_op.sem_op = -1;   /* <-- Comment 1 */
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);

    /* Comment 2 */
    /* we "locked" the semaphore, and are assured exclusive access to file.  */
    /* manipulate the file in some way. for example, write a number into it. */
    file = fopen(file_path, "w");
    if (file) {
      fprintf(file, "%d\n", number);
      printf("%d\n", number);
      fclose(file);
    }

    /* finally, signal the semaphore - increase its value by one. */
    sem_op.sem_num = 0;
    sem_op.sem_op = 1;   /* <-- Comment 3 */
    sem_op.sem_flg = 0;
    semop(sem_set_id, &sem_op, 1);
}

/* this function calls "file_update" several times in a row,  */
/* and waiting a little time between each two calls, in order */
/* to allow other processes time to operate.                  */
void do_child_loop(int sem_set_id, char* file_name)
{
  pid_t pid = getpid();
  int i, j;

  for (i=0; i<3; i++) {
    update_file(sem_set_id, file_name, pid);
    for (j=0; j<400000; j++)
          ;
  }
}

/* finally, the main() function. */
void main()
{
  int sem_set_id;      /* ID of the semaphore set.       */
  union semun sem_val;      /* semaphore value, for semctl(). */
  int child_pid;      /* PID of our child process.      */
  int i;      /* counter for loop operation.    */
  int rc;      /* return value of system calls.  */

  /* create a semaphore set with ID 250, with one semaphore   */
  /* in it, with access only to the owner.                    */
  sem_set_id = semget(SEM_ID, 1, IPC_CREAT | 0600);
  if (sem_set_id == -1) {
    perror("main: semget");
    exit(1);
  }

  /* intialize the first (and single) semaphore in our set to '1'. */
    sem_val.val = 1;
    rc = semctl(sem_set_id, 0, SETVAL, sem_val);
    if (rc == -1) {
      perror("main: semctl");
      exit(1);
    }

    /* create a set of child processes that will compete on the semaphore */
    for (i=0; i<NUM_PROCS; i++) {
      child_pid = fork();
      switch(child_pid) {
      case -1:
	perror("fork");
	exit(1);
      case 0:  /* we're at child process. */
	do_child_loop(sem_set_id, FILE_NAME);
	exit(0);
      default: /* we're at parent process. */
	break;
      }
    }

    /* wait for all children to finish running */
    for (i=0; i<NUM_PROCS; i++) {
      int child_status;

      wait(&child_status);
    }

    printf("main: we're done\n");
    fflush(stdout);
}

#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(void) {
  
  key_t mem_key = ftok("foobar", 0);
  int shmId = 0;
  size_t byte_size = 5540780000;
  size_t max_try = ULONG_MAX;
  for (; byte_size < max_try; byte_size+=1) {
    shmId = shmget(mem_key , byte_size, IPC_CREAT|S_IRUSR|S_IWUSR);
    if (shmId < 0) {
      printf("max size curr: %lu\n", (byte_size - 1));
      return 0;
    } else {
      shmctl(shmId, IPC_RMID, 0);
    }
    
  }
  return 0;
}  










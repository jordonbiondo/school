#ifndef CRITICAL_H
#define CRITICAL_H

#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <errno.h>
#include <string.h>

#if  __STDC_VERSION__ >= 199901L
#else 
#error critical.h requires C99 mode
#endif

/**
 * Create and return  new sembuf, perform op to own sem
 */
struct sembuf* __critical_setup(int sem_id) {
  struct sembuf *this = malloc(sizeof(struct sembuf));
  this->sem_num = 0;
  this->sem_op = -1;
  this->sem_flg = 0;
  if (semop(sem_id, this, 1) != 0) {
    puts ("error syncing: ");
    puts(strerror(errno));
    exit(-1);
  }
  return this;
}

/**
 * Set a sembuf up for posting
 */
void __critical_prepare_for_post(struct sembuf* this) {
  this->sem_num = 0;
  this->sem_op = 1;
  this->sem_flg = 0;
  return;
}

/**
 * If sembuf is set to post, perform op and cleanup, else just return
 */
int __critical_end_if_post(struct sembuf* this, int sem_id) {
  if (this->sem_op == -1) return 1;
  else {
    semop(sem_id, this, 1);
    free(this);
    return 0;
  }
}


#define critical(sem_id)						\
  for(struct sembuf* __temp_sem_buff__ = __critical_setup(sem_id);	\
      __critical_end_if_post(__temp_sem_buff__, sem_id);		\
      __critical_prepare_for_post(__temp_sem_buff__))

#endif

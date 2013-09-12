#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

/* Not technically required, but needed on some UNIX distributions */
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

/*
 * Main
 */
int main() {
  if(fork() == 0) {
    int out = open("crap2.txt", O_RDWR);
    dup2(out, 1);
    printf("Hello from child\n");
    fflush(stdout);
    char* foobar[4] = {"ls", "-l", "-a", NULL};
    execvp("ls", foobar);
    printf("error: %s\n", strerror(errno));
  } else {
    wait(0);
  }
  return 0;
}


#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
  DIR *dirPtr;
  struct dirent *entryPtr;

  dirPtr = opendir (".");

  while ((entryPtr = readdir (dirPtr))) {
    struct stat st;
    stat(entryPtr->d_name, &st);
    size_t size = st.st_size;
    printf ("%-20s size: %d bytes\n", entryPtr->d_name, size);
  }
  closedir (dirPtr);
  return 0;
} 

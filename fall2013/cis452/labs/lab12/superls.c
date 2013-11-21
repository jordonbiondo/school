/* superls.c --- ls like utility
 * 
 * Filename: superls.c
 * Description: ls like utility
 * Author: Jordon Biondo, Doug MacDonald
 * Created: Thu Nov 21 11:57:44 2013 (-0500)
 * Version: 
 * Package-Requires: ()
 * Last-Updated: Thu Nov 21 11:58:15 2013 (-0500)
 *     Update #: 4
 */

/* Commentary: 
 * 
 * Provides ls like functionality
 *
 * Usage: superls -[Fsi]? 
 * -F, show file type
 * -s, show file block size
 * -i, show inode number
 * 
 */

/* Code: */


#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>


/**
 * Program modes
 */
typedef enum {STANDARD_MODE, SIZE_MODE, INODE_NUM_MODE, FILETYPE_MODE} _modes;


/**
 * Read the args and stuff
 */
int get_mode(int argc, char* argv[]) {
  if (argc == 2) {
    if (strcmp(argv[1], "-s") == 0) return SIZE_MODE;
    if (strcmp(argv[1], "-i") == 0) return INODE_NUM_MODE;
    if (strcmp(argv[1], "-F") == 0) return FILETYPE_MODE;
    return -1;
  } else {
    return STANDARD_MODE;
  }
}


/**
 * Character to represent file type
 */
char get_type_char(int mode) {
  if (S_ISDIR(mode)) return '/';
  else if (S_ISREG(mode) && mode & 0111) return '*';
  else if (S_ISFIFO(mode)) return '|';
  else if (S_ISSOCK(mode)) return '=';
  else return ' ';
}


/**
 * Main
 */
int main(int argc, char* argv[]) {

  int mode = get_mode(argc, argv);
  if (mode < 0) {
    printf("Invalid arguments\n");
    exit(-1);
  }
  
  DIR *dirPtr;
  struct dirent *entryPtr;
  dirPtr = opendir (".");

  int blksize_count = 0;
  
  while ((entryPtr = readdir (dirPtr))) {
    struct stat st;
    stat(entryPtr->d_name, &st);
    
    switch(mode) {
    case SIZE_MODE: {
      int blksize = (int)(st.st_blocks / 2);
      if (S_ISLNK(st.st_mode)) blksize = 0;
      blksize_count += blksize;
      printf ("%-20s %-3d blocks\n", entryPtr->d_name, blksize);
      break;
    }
    case INODE_NUM_MODE: {
      printf ("%-20s inode#: %d \n", entryPtr->d_name, (int)st.st_ino);
      break;
    }
    case FILETYPE_MODE: {
      printf ("%s%c\n", entryPtr->d_name, get_type_char(st.st_mode));
      break;
    }
    case STANDARD_MODE: {
      printf ("%s\n", entryPtr->d_name);
      break;
    }
    }
    
  }
  
  if (mode == SIZE_MODE) {
    printf("%d total blocks\n", blksize_count);
  }
  
  closedir (dirPtr);
  return 0;
}

/* superls.c ends here */

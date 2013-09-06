#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#define INPUT_BUFFER_SIZE 1024

/*
 * Strings
 */
#define ERROR_MSG "\nError: EOF encountered in command!\n"
#define PROMPT_MSG "Enter command: "
#define EXIT_MSG "Exitting...\n"
#define SHELL_QUIT_CMD "quit\n"

#define DEBUG 1

/*
 * Prototypes
 */
char* read_input(void);
void prompt_user(void);
void handle_input(void);
void exit_shell(void);
void tokenize_input(void);

/*
 * Globals
 */
char input_buffer[INPUT_BUFFER_SIZE];

char** tokens;


/*
 * Main
 */
int main(int argc, char* argv[]) {
  while(1) {
    prompt_user();
    if(read_input() != NULL) {
      handle_input();
    } else {
      printf(ERROR_MSG);
    }
    if (DEBUG) printf("User entered: %s\n", &input_buffer);
  }

  return 0;
}



/*
 * Prompt User
 */
void prompt_user(void) {
  printf(PROMPT_MSG);
}

/*
 * Handle Input
 */
void handle_input(void) {
  if (strcmp(input_buffer, SHELL_QUIT_CMD) == 0) {
    exit_shell();
  } else {
    tokenize_input();
  }
}

/*
 * Read Input
 */
char* read_input(void) {
  return (char*)fgets(input_buffer, INPUT_BUFFER_SIZE, stdin);
}


/*
 * Tokenize Input
 * http://stackoverflow.com/questions/8106765/using-strtok-in-c
 */
void tokenize_input() {
  int token_count = 0;
  char* first = (char*)strtok(&input_buffer, " ");
  printf("%s\n", first);
}

/*
 * Exit the shell
 */
void exit_shell(void) {
  printf(EXIT_MSG);
  exit(0);
}



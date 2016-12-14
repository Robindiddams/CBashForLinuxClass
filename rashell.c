#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cd(int argc, char* argv[]) {
  char bu[1024];
  int i = 0;
  while (i < 1024) {
    bu[i] = 0;
    i++;
  }
  printf("argc %d\n", argc);

  if (argc == 1) {
    // printf("%s\n", getenv("HOME"));
    if (chdir(getenv("HOME")) != 0) {
      perror("error changing directories");
    } else {
      getcwd(bu, 1024);
      printf("Current directory: %s\n", bu);
    }
  } else if (argc == 2) {
    if (chdir(argv[1]) != 0) {
      perror("error changing directories");
    } else {
      getcwd(bu, 1024);
      printf("Current directory: %s\n", bu);
    }
  } else {
    printf("Error: too many arguments\n");
  }

  return 0;
}

void loop() {
  int quit = 0;
  char *command;

  while(!quit) {
    command = calloc(1024, 1);
    printf("You@rashell: $ ");
    scanf(" %99[^\n]", command);
    // fgets(command, 256, stdin);
    if (strcmp(command, "exit") == 0) {
      quit = 1;
    }

    // printf("command:%s:%d\n", command, strlen(command));
    // exit(1);

    static char *argv[256];
    // printf("%s\n", command);
    char* tokme = command; // because i dont want to mess with command
    char* token = strtok(tokme, " ");// tokenize this
    char baseCommand[256];
    // if (strcmp(baseCommand, "cd") == 0) {
    //   quit = 1;
    // }
    // printf("%d", strlen(tokme));
    sprintf(baseCommand, "./%s", token);
    int argc = 1;
    argv[0] = baseCommand;

    // printf("%d %s\n", 0, argv[0]);
    while (token != NULL){

      token = strtok(NULL, " \n");
      // printf("%d %s\n", argc, token);

      argv[argc] = token;
      argc++;
    }
    argv[argc] = NULL;

    if (strcmp(argv[0], "./cd") == 0) {
      cd(argc - 1, argv);
    } else {

      int pid = fork();

      if (pid == 0) {
        execv(argv[0], argv);
        perror("error: ");
        exit(1); /* only if execv fails */
      }
      else { /* pid!=0; parent process */
        waitpid(pid,0,0); /* wait for child to exit */
      }
    }



    // printf("%s\n", command);
    free(command);
  }

}


int main() {
  loop();

  return 0;
}

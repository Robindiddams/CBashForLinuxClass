// #This code was written by Robin Diddams, Nils Steinbugl, Chiara Piazzolla, and Rachel Lewis
// #nobody cheated
// #CSI-230-03
// this was tested and works on ubuntu(64bit-native), and probably linux mint :/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  char bu[1024];
  int i = 0;
  while (i < 1024) {
    bu[i] = 0;
    i++;
  }

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

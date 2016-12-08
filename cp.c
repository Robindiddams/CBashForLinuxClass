#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int cp(const char* original, const char* predestination) {
  //standard cp for files
  // printf("cp with %s, %s\n", original, predestination);
  int fdo, fdd, sizer, sizew;
  char buffer[4096];
  struct stat st;

  stat(original, &st);
  //check if its a dircectory
  if (S_ISDIR(st.st_mode)) {
    printf("%s is a directory\n", original);
    return 1;
  }

  //make a buffer for the destination path
  char destination[512];

  //clean it out
  for (int i = 0; i < 512; i++) {
    destination[i] = 0;
  }

  //canonical
  strcat(destination, predestination);
  // printf("cp: destination:%s\n", destination);


  stat(predestination, &st);
  //if destination is just a folder we gotta append a name onto the destination
  if (S_ISDIR(st.st_mode)) {
    //name buffer
    char name[256];
    int i = strlen(original) - 1;

    //count back to the /
    while(original[i] != '/' && i > -1) {
      i--;
    }
    i++;
    int j = 0;

    //move forward to the end, filling the name buffer
    while(i < strlen(original)) {
      name[j] = original[i];
      i++;
      j++;
    }
    name[j] = 0;

    //does the dir have a / in it or not
    if (destination[strlen(destination) - 1] == '/') {
      sprintf(destination, "%s%s", destination, name);
    } else {
      sprintf(destination, "%s/%s", destination, name);
    }
    // printf("cp: postdestination:%s\n", destination);
  }

  //now we make it a const :(
  const char* postdestination = destination;

  //open the files
  fdo = open(original, O_RDONLY);
  if (fdo < 0) {
    printf("Error! Cannot open %s\n", original);
    return 1;
  }
  fdd = open(postdestination, O_WRONLY | O_CREAT, 0777);
  if (fdd < 0) {
    printf("Error! cannot open %s\n", postdestination);
    return 1;
  }

  //copy the stuff over
  sizer = read(fdo, buffer, sizeof(buffer));
  sizew = write(fdd, buffer, sizer);
  if (sizer != sizew) {
    //this would be a weird bug, but nobody ever got punished for too much error checking
    return 1;
  }
  return 0;
}

int cpr(const char* file, const char* predestination) {
  // printf("cpr called with: %s, %s\n", file, predestination);
  //this is for directories, i know it says "file", but technically its a file so deal

  //check if its actually a file
  struct stat st;
  stat(file, &st);
  if (!S_ISDIR(st.st_mode)) {
    //if not cp can handle this one
    //logically this shouldnt happen though
    return cp(file, predestination);
  }

  //another destination buffer!
  char destination[512];
  for (int i = 0; i < 512; i++) {
    destination[i] = 0;
  }

  //concat it! make it cool!!!! and also forming to a standard
  sprintf(destination, "%s%s", destination, predestination);
  int len = strlen(destination);
  if (destination[len - 1] != '/') {
    destination[len] = '/';
    destination[len+1] = 0;
  }

  //big ol' if tree
  if (stat(predestination, &st) == -1) {
    // printf("creating %s\n", predestination);

    //if the destination doesnt exist we create it
    mkdir(predestination, 0777);
  } else if (!S_ISDIR(st.st_mode)) {
    //if its a file we get angry
    printf("cannot copy directory into file\n");
    return 1;
  } else {
    //if the destination does exist we enter it and create a folder there
    //we gotta append a name onto the destination

    //same thing from cp, with some modification
    char name[512];
    for (int i = 0; i < 512; i++) {
      name[i] = 0;
    }
    int i = strlen(file) - 1;

    //beacause directories end in / but its optional so thisll catch it
    if (file[i] == '/') {
      i--;
    }
    while(file[i] != '/' && i > -1) {
      i--;
    }
    i++;
    int j = 0;
    while(i < strlen(file)) {
      name[j] = file[i];
      i++;
      j++;
    }
    if (name[j - 1] != '/') {
      name[j] = '/';
      name[j+1] = 0;
    }

    //and then make the dir
    strcat(destination, name);
    mkdir(destination, 0777);
  }
  // printf("destination: %s\n", destination);


  //folder io stuff
  struct dirent * dp;

  DIR* dirp = opendir(file);
  if (dirp == NULL) {
    printf("cp: cannot access '%s': No such file or directory\n", file);
    exit(1);
  }

  //loop through the dir's contents
  while ((dp = readdir(dirp)) != NULL) {
    if (dp->d_name[0] != '.') {
      struct stat fileInfo;

      //make a path buffer
      char appendedPath[512];
      for (int i = 0; i < 512; i++) {
        appendedPath[i] = 0;
      }
      // printf("processing file: %s\n", dp->d_name);

      //construct it properly
      if (file[strlen(file) - 1] == '/') {
        sprintf(appendedPath, "%s%s", file, dp->d_name);
      } else {
        sprintf(appendedPath, "%s/%s", file, dp->d_name);
      }

      // printf("appendedPath: %s\n", appendedPath);

      //if dir then recursion, else we call cp
      stat(appendedPath, &fileInfo);
      if (S_ISDIR(fileInfo.st_mode)) {
        // printf("begin recursion cpr(%s, %s)\n", appendedPath, destination);
        cpr(appendedPath, destination);
      } else {
        // printf("calling cp(%s, %s)\n",appendedPath, destination);
        cp(appendedPath, destination);
      }
      // printf("%s\n", dp->d_name);
    }
  }

  return 0;
}



int main (int argc, char* argv[]) {
  int r = 0;
  char to[512];
  char from[512];
  //parse arguments for options and a path
  if (argc > 2) {
    // printf("argc: %d\n", argc);
    int i = 1;
    for (i; i < argc && argv[i][0] == '-'; i++) {
      // printf("%d, %s\n", i, argv[i]);
      for(int q = 0; argv[i][q] != 0; q++) {
        if(argv[i][q] == 'r') {
          r = 1;
        }
      }
    }
    // printf("%d %d\n", argc, i);
    if (i == argc - 2 && argv[i][0] != '-') {
      //get final arguments
      sprintf(from, "%s", argv[i]);
      i++;
      sprintf(to, "%s", argv[i]);

      //if we made it here then we are good to go!
      // printf("%s, %s\n", from, to);
      if (r) {
        printf("%d", cpr(from, to));
      } else {
        printf("%d", cp(from, to));
        // cp(to, from);
      }

    } else {
      printf("too many arguments\n");
    }
  } else {
    printf("Not enough arguments\n");
  }

  return 0;
}

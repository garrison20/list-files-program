/*
  This program is a simple copy of the terminal's "ls" function.
  Simply typing executing the program with no arguments will print
  all the contents of the folder. Executing the program with a name
  passed in as a parameter will print "Found file" if the name exists. 
  and belongs to a file, and will print the contents of a directory if 
  the name belongs to a directory. If there are two arguments, then the 
  first is the file to be checked for existence and the second is the 
  desired directory to check in. This perform the same function as 
  passing only one argument to the program, but this only works inside 
  one of the current directory's immediate directories. The function 
  can't search inside a directory two levels down, only one.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void print_contents(char *);
int search(char *, char *);
char *make_pathname(char *, char *);

/* This function parses through all the contents of a directory and prints the 
name of every file/dir it finds */
void print_contents(char *dir) {
  DIR *curr_dir = NULL;
  struct dirent *file_parsing = NULL;

  /* If curr_dir is NULL, there is either no dir/file in the current directory
    or we have finished parsing through all files/dirs in the current directory */
  if ((curr_dir = opendir(dir)) == NULL) {
    perror("Error opening directory");
    exit(1);
  }

  while ((file_parsing = readdir(curr_dir)) != NULL) {
    printf("%s\n", file_parsing->d_name);
  }

  closedir(curr_dir);
}

/* This function parses through a directory and prints "File found" if the name being 
searched is a file and prints the contents of a directory if the name is a directory */
/*int search(char *dir, char *desired_name, DIR *curr_dir, struct dirent *file_parsing) {*/
int search(char *dir, char *desired_name) {
  char *path = NULL;
  DIR *curr_dir = NULL, *temp_dir = NULL;
  struct dirent *file_parsing = NULL, *temp_parsing = NULL;
  
  if ((curr_dir = opendir(dir)) == NULL) {
    perror("Error opening directory");
    return 1;
  }

  while ((file_parsing = readdir(curr_dir)) != NULL) {
    if (strcmp(file_parsing->d_name, desired_name) == 0) {
      if (file_parsing->d_type == DT_REG) {
        printf("File found\n");
        exit(0);
      } else if (file_parsing->d_type == DT_DIR) {
        /* Create string that will designate the path necessary for reaching the 
        desired directory for printing */
        path = make_pathname(dir, file_parsing->d_name);

        if (path == NULL)
          return 1;

        print_contents(path);
        free(path);
        return 0;
      } else {
        printf("Unknown file/device\n");
        return 0;
      }
    }
  }

  printf("File/Directory not found\n");

  return 0;
}

char *make_pathname(char *left_side, char *right_side) {
  char *output = NULL;

  output = malloc(sizeof(*left_side) + 1);
  if (output != NULL) {
    strcpy(output, left_side);
    strcat(output, "/");

    output = realloc(output, sizeof(*output) + sizeof(*right_side) + 2);
    if (output != NULL) {
      strcat(output, right_side);
    }
  }

  return output;
}

int main(int argc, char *argv[]) {
  struct dirent *file_parsing;
  DIR *curr_dir;

  /* If no arguments, then print all of the folder's contents*/
  if (argc == 1) {
    print_contents(".");
  /* If there's one argument, then search for a file/dir which has that name */
  } else if (argc == 2) {
    search(".", argv[1]);
  /* If there are three arguments, the first arg is the desired file/dir and 
  the second arg is the directory name */
  } else if (argc == 3) {
    search(argv[2], argv[1]);
  /* If there are more than 3 arguments, then simply alert the user */
  } else {
    printf("Too many arguments\n");
    return 1;
  }

  return 0;
}

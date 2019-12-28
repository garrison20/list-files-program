/*
  This program is a simple copy of the terminal's "ls" function.
  Simply typing executing the program with no arguments will print
  all the contents of the folder. Executing the program with a name
  passed in as a parameter will print "exists" in the terminal to 
  tell the user the file/directory is in the current directory. If 
  there are two arguments, then the first is the file to be checked
  for existence and the second is the desired directory to check in.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

void print_contents(char *, DIR *, struct dirent *);
void search(char *, char *, DIR *, struct dirent *);

/* This function parses through all the contents of a directory and prints the 
name of every file/dir it finds */
void print_contents(char *dir, DIR *curr_dir, struct dirent *file_parsing) {
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
void search(char *dir, char *desired_name, DIR *curr_dir, struct dirent *file_parsing) {
  char path[100];
  
  if ((curr_dir = opendir(dir)) == NULL) {
    perror("Error opening directory");
    exit(1);
  }

  while ((file_parsing = readdir(curr_dir)) != NULL) {
    if (strcmp(file_parsing->d_name, desired_name) == 0) {
      if (file_parsing->d_type == DT_REG) {
        printf("File found\n");
        exit(0);
      } else if (file_parsing->d_type == DT_DIR) {
        /* Create string that will designate the path necessary for reaching the 
        desired directory for printing */
        strcpy(path, dir);
        strcat(path, "/");
        strcat(path, file_parsing->d_name);

        /* Make these variables null so we can use them in the next print call 
        and save space */
        curr_dir = NULL;
        file_parsing = NULL;

        print_contents(path, curr_dir, file_parsing);
        exit(0);
      } else 
        printf("Unknown file/device\n");
    }
  }

  closedir(curr_dir);
}

int main(int argc, char *argv[]) {
  struct dirent *file_parsing;
  DIR *curr_dir;

  /* If no arguments, then print all of the folder's contents*/
  if (argc == 1) {
    print_contents(".", curr_dir, file_parsing);
  /* If there's one argument, then search for a file/dir which has that name */
  } else if (argc == 2) {
    search(".", argv[1], curr_dir, file_parsing);
  /* If there are three arguments, the first arg is the desired file/dir and 
  the second arg is the directory name */
  } else if (argc == 3) {
    search(argv[2], argv[1], curr_dir, file_parsing);
  } else {
    printf("Too many arguments\n");
    return 1;
  }

  return 0;
}

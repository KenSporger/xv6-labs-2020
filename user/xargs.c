/*
 * xargs.c
 * read lines from the standard input and run a command for each line, 
 * supplying the line as arguments to the command. 
 * For example:
 * $ echo hello too | xargs echo bye
 * bye hello too
 * $
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

/**
 * @description: get a line from standard input.
 */
char*
getline(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    if(c == '\n' || c == '\r')
      break;
    buf[i++] = c;
  }
  buf[i] = '\0';
  return buf;
}

int
main(int argc, char* argv[])
{
    if (argc < 2){
        fprintf(2, "Usage: xargs cmd ...\n");
        exit(1);
    }

    char buf[512];
    char* args[MAXARG]; 
    memcpy(args, &argv[1], (argc-1) * sizeof(char*)); // make new arguments

    while (*getline(buf, sizeof(buf)) != '\0')
    {
        args[argc-1] = buf; // add line into arguments
        if (fork() == 0){
            exec(argv[1], args);
        }else{
            wait(0);
        }
    }
    exit(0);
}
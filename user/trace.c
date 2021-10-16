/**
 * trace.c
 * Trac for the process that calls it and any children that it subsequently
 * forks. It takes one argument, an integer "mask", whose bits specify whi-
 * ch system calls to trace. For example, to trace the fork system call, a 
 * program calls trace(1 << SYS_fork), where SYS_fork is a syscall number 
 * from kernel/syscall.h. 
 * The output contains the process id, the name of the system call and the 
 * return value.
 * Here is a example:
 * 
 * trace 32 grep hello README
 * 3: syscall read -> 1023
 * 3: syscall read -> 966
 * 3: syscall read -> 70
 * 3: syscall read -> 0
 * 
 * In this example, trace invokes grep tracing just the read system call. 
 * The 32 is 1<<SYS_read. 
 */
#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;
  char *nargv[MAXARG];

  if(argc < 3 || (argv[1][0] < '0' || argv[1][0] > '9')){
    fprintf(2, "Usage: %s mask command\n", argv[0]);
    exit(1);
  }

  if (trace(atoi(argv[1])) < 0) {
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);
  }
  
  for(i = 2; i < argc && i < MAXARG; i++){
    nargv[i-2] = argv[i];
  }
  exec(nargv[0], nargv);
  exit(0);
}

/*
 * redirect.c
 * run a command with output redirected
 */
#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int
main(int argc, char* argv[])
{
    if (argc < 2){
        fprintf(2, "Usage: redirect file ...\n");
        exit(1);
    }else if (fork() == 0){
        close(1); // make sure 'open' return the file descriptor 1
        open(argv[1], O_WRONLY | O_CREATE); // create file and only for write 

        exec("echo", argv+1);
        fprintf(2, "exec failded!\n");
        exit(1);
    }
    exit(0);
}

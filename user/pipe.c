/*
 * pipe.c
 * pipe test.
 */

#include "kernel/types.h"
#include "user/user.h"

int
main()
{
    char* argv[2];
    argv[0] = "wc";
    argv[1] = 0;

    int p[2];
    pipe(p);
    
    if (fork() == 0){
        close(0);
        dup(p[0]);

        close(p[1]);

        exec("cat", argv);
        fprintf(2, "exec failded!\n");
        exit(1);
    }else{
        write(p[1], "hello world\n", 12);

        close(p[0]);
        close(p[1]);
    }

    exit(0);
}

/*
 * pingpong.c
 * The parent process send a byte to the child; the child print "<pid>: received ping", 
 * where <pid> is its process ID, then write the byte on the pipe to the parent, and e-
 * xit; the parent read the byte from the child, print "<pid>: received pong", and exit. 
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

/*
int
main()
{
    int ptoc[2]; // direction: parent to child
    int ctop[2]; // child to parent

    pipe(ptoc);
    pipe(ctop);

    char buf[2];

    if (fork() == 0){
        read(ptoc[0], buf, sizeof(buf) > 0);
        fprintf(1, "%d: received ping\n", getpid());
        write(ctop[1], "c", 1);
        exit(0);
    }else{
        write(ptoc[1], "p", 1);
        read(ctop[0], buf, sizeof(buf) > 0);
        fprintf(1, "%d: received pong\n", getpid());
        wait(0);
    }
    exit(0);
}
*/

int
main()
{
    int p[2]; // half duplex

    pipe(p);

    char buf[2];

    if (fork() == 0){
        read(p[0], buf, sizeof(buf) > 0);
        fprintf(1, "%d: received ping\n", getpid());
        write(p[1], "c", 1);
        exit(0);
    }else{
        write(p[1], "p", 1);
        wait(0); // don't let parent read that
        read(p[0], buf, sizeof(buf) > 0);
        fprintf(1, "%d: received pong\n", getpid());
    }
    exit(0);
}
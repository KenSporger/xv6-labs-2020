/*
 * primes.c
 * a concurrent version of prime sieve using pipes
 * The first process feeds the numbers 2 through 35 into the pipeline.
 * For each prime number, it creates one process that reads from its 
 * left neighbor over a pipe and writes to its right neighbor over an-
 * other pipe.
 * see details: https://swtch.com/~rsc/thread/
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
pipeline(int leftfd)
{
    int p, n;
    if (read(leftfd, &p, sizeof(p)) == 0){ // write-side of the pipe is closed and nothing to read
        close(leftfd);
        return;
    }
    printf("prime %d\n", p);

    int rightfd[2];
    pipe(rightfd);

    if (fork() == 0){
        close(rightfd[1]); // write fd must be closed
        pipeline(rightfd[0]);
    }else{
        while(read(leftfd, &n, sizeof(n)) == sizeof(n)){ // read number from left neighbor
            if (n % p != 0)
                write(rightfd[1], &n, sizeof(n)); // send number to right neighbor
        }
        close(rightfd[0]);
        close(rightfd[1]);
        wait(0); // wait for right neighbor
    }
    exit(0);
}

int
main()
{
    int rightfd[2];
    pipe(rightfd);
    if (fork() == 0){
        close(rightfd[1]);
        pipeline(rightfd[0]);
    }else{
        for (int i = 2; i <= 35; i++){ 
            write(rightfd[1], &i, sizeof(i));  // feeds the numbers from 2 to 35
        }
        close(rightfd[0]);
        close(rightfd[1]);
        wait(0);
    }
    exit(0);
}
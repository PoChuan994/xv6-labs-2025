#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
    /* 
     * p1 and p2 are used for storage of the file descriptors for the pipes.
     * p1 is the pipe for communication from parent to child, and p2 is the pipe for communication from child to parent.
     */
    int p1[2], p2[2];

    /* If pipe creation fails (file descriptor is -1), print an error message and exit */
    if (pipe(p1) < 0 || pipe(p2) < 0) {
        printf("pipe failed\n");
        exit(1);
    }

    /* Fork a new process */
    int pid = fork();
    if (pid < 0) {
        printf("fork failed\n");
        exit(1);
    }

    char b;
    if (pid == 0) {

        /* 
         * Close write end of p1 and read end of p2 in the child process,
         * since the child will only read from p1 and write to p2.
         */
        close(p1[1]);
        close(p2[0]);

        /* Read a byte from the parent */
        if (read(p1[0], &b, 1) != 1) {
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        
        /* Write a byte back to the parent */
        if (write(p2[1], &b, 1) != 1) {
            exit(1);
        }
        close(p1[0]);
        close(p2[1]);

        exit(0);
    } else {

        /* 
         * Close read end of p1 and write end of p2 in the parent process,
         * since the parent will only write to p1 and read from p2.
         */
        close(p1[0]);
        close(p2[1]);

        /* Write a byte to the child */
        b = 'x';
        
        /* Write a byte to the child */
        if (write(p1[1], &b, 1) != 1) {
            exit(1);
        }
        /* Read a byte from the child */
        if (read(p2[0], &b, 1) != 1) {
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        
        close(p1[1]);
        close(p2[0]);
        
        /* Wait for the child process to exit */
        wait(0);
        exit(0);
    }
}

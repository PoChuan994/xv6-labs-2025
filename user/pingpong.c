#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int p1[2], p2[2];
  if (pipe(p1) < 0 || pipe(p2) < 0) {
    printf("pipe failed\n");
    exit(1);
  }

  int pid = fork();
  if (pid < 0) {
    printf("fork failed\n");
    exit(1);
  }

  char b;
  if (pid == 0) {
    // child: read from parent, print, write back, exit
    close(p1[1]);
    close(p2[0]);
    if (read(p1[0], &b, 1) != 1) {
      exit(1);
    }
    printf("%d: received ping\n", getpid());
    if (write(p2[1], &b, 1) != 1) {
      exit(1);
    }
    close(p1[0]);
    close(p2[1]);
    exit(0);
  } else {
    // parent: write to child, read reply, print, wait, exit
    close(p1[0]);
    close(p2[1]);
    b = 'x';
    if (write(p1[1], &b, 1) != 1) {
      exit(1);
    }
    if (read(p2[0], &b, 1) != 1) {
      exit(1);
    }
    printf("%d: received pong\n", getpid());
    close(p1[1]);
    close(p2[0]);
    wait(0);
    exit(0);
  }
}
// Create pingpong.c

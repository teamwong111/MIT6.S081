#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char const *argv[]) {
  int exit_status = 0;
  char buf = 'P';
  int p1[2], p2[2];
  pipe(p1);
  pipe(p2);
  int child_pid;
  child_pid = fork();
  if (child_pid == 0) {
    close(p1[1]);
    if (read(p1[0], &buf, sizeof(buf)) != sizeof(char)) {
      fprintf(2, "child read() error!\n");
      exit_status = 1;
    } else {
      fprintf(1, "%d: received ping\n", getpid());
    }
    close(p1[0]);
    close(p2[0]);
    if (write(p2[1], &buf, sizeof(buf)) != sizeof(buf)) {
      fprintf(2, "child write() error!\n");
      exit_status = 1;
    }
    close(p2[1]);
  } else {
    close(p1[0]);
    if (write(p1[1], &buf, sizeof(buf)) != sizeof(buf)) {
      fprintf(2, "child write() error!\n");
      exit_status = 1;
    }
    close(p1[1]);
    close(p2[1]);
    if (read(p2[0], &buf, sizeof(buf)) != sizeof(char)) {
      fprintf(2, "parent read() error!\n");
      exit_status = 1;
    } else {
      fprintf(1, "%d: received pong\n", getpid());
    }
    close(p2[0]);
  }
  exit(exit_status);
}

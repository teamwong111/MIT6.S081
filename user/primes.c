#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int lpipe[2]) {
  int first;
  if (read(lpipe[0], &first, sizeof(int)) == sizeof(int)) {
    printf("prime %d\n", first);
    int p[2];
    pipe(p);
    int data;
    while (read(lpipe[0], &data, sizeof(int)) == sizeof(int)) {
      if (data % first != 0) {
        write(p[1], &data, sizeof(int));
      }
    }
    close(p[1]);
    if (fork() == 0) {
      primes(p);
    } else {
      close(p[0]);
      wait(0);
    }
  }
}
int main(int argc, char const *argv[]) {
  int p[2];
  pipe(p);
  for (int i = 2; i <= 35; ++i) {
    write(p[1], &i, sizeof(i));
  }
  close(p[1]);
  if (fork() == 0) {
    primes(p);
  } else {
    close(p[0]);
    wait(0);
  }
  exit(0);
}

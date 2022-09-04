#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    fprintf(2, "number of arguments is wrong");
  }
  char c;
  char buf[1024];
  char *p = buf, *q = buf;
  char *new_argv[MAXARG];
  for (int i = 1; i < argc; ++i) {
    new_argv[i - 1] = malloc(strlen(argv[i]) + 1);
    strcpy(new_argv[i - 1], argv[i]);
  }
  while(read(0, &c, sizeof(c)) == sizeof(c)) {
    if (c == '\n') {
      new_argv[argc - 1] = malloc(q - p + 1);
      strcpy(new_argv[argc - 1], p);
      p = q;
      if (fork() == 0) {
        exec(argv[1], new_argv);
      }
      else {
        wait(0);
      }
      free(new_argv[argc - 1]);
    }
    else {
        *q = c;
        ++q;
    }
  }
  exit(0);
}

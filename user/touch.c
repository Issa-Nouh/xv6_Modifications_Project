#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]) {
  int fd;

  if (argc < 2) {
    fprintf(2, "Usage: touch <file>\n");
    exit(1);
  }

  for (int i = 1; i < argc; i++) {
    fd = open(argv[i], O_RDWR);
    if (fd < 0) {
      fd = open(argv[i], O_CREATE | O_RDWR);
      if (fd < 0) {
        fprintf(2, "touch: cannot create %s\n", argv[i]);
        continue;
      }
    }
    write(fd, "", 0);
    close(fd);
  }

  exit(0);
}

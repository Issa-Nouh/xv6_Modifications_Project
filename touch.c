/* user/touch.c */
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int
main(int argc, char *argv[])
{
  int fd;

  if(argc < 2){
    printf(2, "usage: touch file ...\n");   // 2 = stderr
    exit();
  }

  for(int i = 1; i < argc; i++){
    fd = open(argv[i], O_RDWR);
    if(fd < 0)
      fd = open(argv[i], O_CREATE | O_RDWR);
    if(fd < 0){
      printf(2, "touch: cannot create %s\n", argv[i]);
      continue;
    }
    close(fd);
  }
  exit();
}

#include "types.h"  
#include "stat.h" // File status struct (for fstat if needed)
#include "user.h" // User-space system calls (open, close, etc.)
#include "fcntl.h" // File control flags (O_RDWR, O_CREATE)

int main(int argc, char *argv[])
{
  int fd;

  // Check if filename arguments are provided
  if(argc < 2){
    printf(2, "usage: touch file ...\n"); // Print to stderr (fd 2)
    exit();  // Exit with error status (default 1)
  }

  // Process each filename argument
  for(int i = 1; i < argc; i++){
    // Try opening existing file in read-write mode
    fd = open(argv[i], O_RDWR);
    
    // If file doesn't exist, create it with read-write permissions
    if(fd < 0)
      fd = open(argv[i], O_CREATE | O_RDWR);
    
    // If still fails, print error and skip to next file
    if(fd < 0){
      printf(2, "touch: cannot create %s\n", argv[i]);
      continue;
    }
    
    close(fd);  
  }
  
  exit();
}

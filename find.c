#include "types.h"
#include "stat.h" // File status structure
#include "fcntl.h" // File control options
#include "user.h" // User-space system calls
#include "fs.h" // Filesystem constants and structures

void find(char *path, char *target) {
    char buf[512], *p;       // Buffer for path construction
    int fd;                  // File descriptor
    struct dirent de;        // Directory entry structure for traversal
    struct stat st;          // File status structure

    // Attempt to open the directory
    if((fd = open(path, 0)) < 0) {
        printf(2, "find: cannot open %s\n", path);
        return;
    }

    // Get directory status information
    if(fstat(fd, &st) < 0) {
        printf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // Verify it's actually a directory
    if(st.type != T_DIR) {
        close(fd);
        return;
    }

    // Check path length to prevent buffer overflow
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        printf(2, "find: path too long\n");
        close(fd);
        return;
    }

    // Prepare path buffer for directory traversal
    strcpy(buf, path);       // Copy base path
    p = buf + strlen(buf);    // Pointer to end of path
    *p++ = '/';               // Add directory separator

    // Read directory entries
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        // Skip empty directory entries
        if(de.inum == 0) continue;

        // Append current filename to path
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;        // Null-terminate

        // Get file status
        if(stat(buf, &st) < 0) {
            printf(2, "find: cannot stat %s\n", buf);
            continue;
        }

        // Handle directory entries
        if(st.type == T_DIR) {
            // Skip "." and ".." to prevent infinite recursion
            if(strcmp(de.name, ".") && strcmp(de.name, "..")) {
                find(buf, target);  // Recursive call
            }
        } 
        // Handle regular files
        else if(st.type == T_FILE) {
            // Check if filename matches target
            if(strcmp(de.name, target) == 0) {
                printf(1, "%s\n", buf);  // Print full path (stdout)
            }
        }
    }
    close(fd); 
}
int main(int argc, char *argv[]) {
    // Validate argument count
    if(argc != 3) {
        printf(2, "usage: find path filename\n");  // stderr
        exit();
    }
    
    // Start recursive search
    find(argv[1], argv[2]);  // path, filename
    exit();
}

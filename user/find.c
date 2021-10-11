/*
 * find.c
 * find all the files in a directory tree with a specific name.
 */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"

/**
 * @description: find file in directory
 * @param {path} must be a directory
 * @param {name} filename
 */
void
find(char *path, char* name)
{
    char buf[512], *p;
    int fd;
    struct dirent de; // directory entry
    struct stat st; // file meta information
    
    if ((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }

    if (fstat(fd, &st) < 0){ // get metadata by file descriptor
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }

    if (st.type != T_DIR) {
        fprintf(2, "find: %s is not a directory\n", path);
        close(fd);
        exit(1);
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){ // ensure buf can contain path + '/' + de.name + '\0'
        printf("ls: path too long\n");
        close(fd);
        exit(1);
    }

    
    strcpy(buf, path); // prepare for splicing new paths
    p = buf + strlen(buf);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)){ // traverse all directory entries
        if (de.inum == 0) // invalid entry
            continue;
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) // Don't recurse into "." and ".."
            continue;

        strcpy(p, de.name); // splicing new path
        p[DIRSIZ] = '\0';

        if (stat(buf, &st) < 0) { // get metadata by filename
            fprintf(1, "find: cannot stat %s\n", buf);
            continue;
        }

        if (st.type == T_FILE && strcmp(de.name, name) == 0)
            printf("%s\n", buf);
        else if (st.type == T_DIR)
            find(buf, name);
    }

    close(fd); // must close fd, or it will cause open error
}

int
main(int argc, char* argv[])
{
    if (argc < 2 || argc > 3) {
        fprintf(2, "Usage: find (path) [file]\n");
        exit(1);
    }else if(argc < 3) {
        find(".", argv[1]);
    }else{
        find(argv[1], argv[2]);
    }
    exit(0);
}
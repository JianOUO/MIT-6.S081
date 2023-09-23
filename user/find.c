#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char* filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    } 
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/'; 
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0 || de.inum == 0) 
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        switch(st.type) {
        case T_FILE:
            if (strcmp(filename, de.name) == 0) {
                printf("%s/%s\n", path, filename);
            }
            break;
        case T_DIR:
            find(buf, filename);
            break;
        }
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(2, "usage: find path filename...\n");
    }
    find(argv[1], argv[2]);
    exit(0);
}

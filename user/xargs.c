#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int i, pd[2];
    char *cmd1[32];
    for (i = 0; i < 32; i++) {
        cmd1[i] = malloc(10 * sizeof(char));
    }
    for (i = 0; i < argc-1; i++) {
        strcpy(cmd1[i], argv[i+1]);
        //cmd1[i] = argv[i+1];
        //printf("argv: %s\n", argv[i+1]);
        //printf("cmd1: %s\n", cmd1[i]);
    }
    pipe(pd);
    if (fork()) {
        if (fork()) {
            close(pd[0]);
            close(pd[1]);
            wait((int *)0);
            wait((int *)0);
        } else {
            close(pd[1]);
            char *p = cmd1[i++];
            while (read(pd[0], p, sizeof(char)) == sizeof(char)) {
                //printf("%c", *p);
                if (*p == '\n') {
                    *p = '\0';
                    p = cmd1[i++];
                    continue;
                }
                p++;
            }
            //p = 0;
            cmd1[--i] = 0;
            //cmd1[5] = 0;
            close(pd[0]);
            exec(cmd1[0], cmd1);
            exit(1);
        }
    } else {
        close(1);
        dup(pd[1]);
        close(pd[0]);
        close(pd[1]);
        char *cmd2[] = {"find", ".", "b", 0};
        exec("find", cmd2);
        exit(1);
    }
    exit(0);
}
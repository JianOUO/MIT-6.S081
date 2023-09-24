#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    int i;
    char *cmd1[32];// all elements initial to 0, so we need to alloc memeoy to them
    for (i = 0; i < 32; i++) {
        cmd1[i] = malloc(10 * sizeof(char));
    }
    for (i = 0; i < argc-1; i++) {
        strcpy(cmd1[i], argv[i+1]);//my fault: if cmd1[i] hasn't been alloced memory(above), this line doesn't work
        //cmd1[i] = argv[i+1];
        //printf("argv: %s\n", argv[i+1]);
        //printf("cmd1: %s\n", cmd1[i]);
    }
    char *p = cmd1[i++];
    while (read(0, p, sizeof(char)) == sizeof(char)) {
        //printf("%c", *p);
        if (*p == '\n') {
            *p = '\0';
            p = cmd1[i++];
            continue;
        }
        p++;
    }
    //p = 0; my fault, this doesn't infulence cmd1[5]
    cmd1[--i] = 0; // aka cmd1[5] = 0;
    exec(cmd1[0], cmd1);
    exit(0);
}
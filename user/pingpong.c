#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    char buffer;
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    if (fork() == 0) {
        if (read(p1[0], &buffer, 1) != 1) {
            fprintf(2, "pingpong: child read error\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        if (write(p2[1], &buffer, 1) != 1) {
            fprintf(2, "pingpong: child write error\n");
            exit(1);
        }
        exit(0);
    } else {
        if (write(p1[1], "a", 1) != 1) {
            fprintf(2, "pingpong: parent write error\n");
            exit(1);
        }
        if (read(p2[0], &buffer, 1) != 1) {
            fprintf(2, "pingpong: parent read error\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        exit(0);
    }
}
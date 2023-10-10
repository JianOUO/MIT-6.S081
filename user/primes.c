#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void nextprocess(int *left) {
    close(left[1]);
    int right[2], prime, pid;
    if(read(left[0], &prime, sizeof(prime))) {
        printf("prime %d\n", prime);
    } else {
        exit(0);
    }
    pipe(right);
    if ((pid = fork()) < 0) {
        fprintf(2, "fork failed\n");
        close(right[0]);
        close(right[1]);
        exit(1);
    } else if (pid > 0) {
        int temp;
        close(right[0]);
        while(read(left[0], &temp, sizeof(temp))) {
            if (temp % prime) {
                write(right[1], &temp, sizeof(temp));
            }
        }
        close(left[0]);
        close(right[1]);
        wait(0);
        exit(0);
    } else {
        nextprocess(right);
        exit(0);
    }
}

int main(int argc, char* argv[]) {
    int right[2], pid;
    pipe(right);
    if ((pid = fork()) < 0) {
        fprintf(2, "fork failed\n");
        close(right[0]);
        close(right[1]);
        exit(1);
    } else if (pid > 0) {
        close(right[0]);
        for(int i = 2; i <= 35; i++) {
            write(right[1], &i, sizeof(i));
        }
        close(right[1]);
        wait(0);
        exit(0);
    } else {
        nextprocess(right);
        exit(0);
    }
    return 0;
}
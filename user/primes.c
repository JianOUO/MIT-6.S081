#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void source() {
    int i;
    for(i = 2; i <= 35; i++) {
        write(1, &i, sizeof(i));
    }
}

void redirect(int n, int *fd) {
    close(n);
    dup(fd[n]);
    close(fd[0]);
    close(fd[1]);
}

void cull(int prime) {
    int number;
    while(read(0, &number, sizeof(number)) == sizeof(number)) {
        if (number % prime != 0) {
            write(1, &number, sizeof(number));
        }
    }
}

void sink() {
    int prime;
    int fd[2];    
    while (read(0, &prime, sizeof(prime)) == sizeof(prime)) {
        printf("prime %d\n", prime);
        pipe(fd);
        if (fork()) {
            redirect(0, fd);
        } else {
            redirect(1, fd);
            cull(prime);
        }
    }
}

int main(int argc, char *argv[]) {
    int fd[2];
    pipe(fd);
    if (fork()) {
        redirect(0, fd);
        sink();
    } else {
        redirect(1, fd);
        source();
    }
    exit(0);
}
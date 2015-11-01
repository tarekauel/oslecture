#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int my_value = 42;

int main() {

    pid_t pid;

    pid = fork();
    usleep(150);

    if (pid < 0) {
        fprintf(stderr, "An error occured\n");
        return -1;
    } else if (pid == 0) {
        my_value = 18951;
        fprintf(stderr, "I'm the child, my pid is %d, my_value is %d\n", getpid(), my_value);
        usleep(500);
    } else {
        fprintf(stderr, "I'm the parent, ym pid is %d, my_value is %d\n", getpid(), my_value);
        wait(0);
        fprintf(stderr, "I waited for my child\n");
    }

    return 0;
}
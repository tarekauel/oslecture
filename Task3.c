#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

int my_value = 42;

int main() {

    int fd[2];
    pid_t pid;

    char* string = malloc(100);

    pipe(fd);

    pid = fork();
    usleep(150);

    if (pid < 0) {
        fprintf(stderr, "An error occured\n");
        return -1;
    } else if (pid == 0) {
        close(fd[1]);
        my_value = 18951;
        fprintf(stderr, "I'm the child, my pid is %d, my_value is %d\n", getpid(), my_value);
        usleep(500);
        read(fd[0], string, sizeof(*string));
        fprintf(stderr, "%s", string);
    } else {
        close(fd[0]);
        sprintf(string, "Hi, I am your parent. My PID=%d and my_value=%d\n", getpid(), my_value);
        write(fd[1], string, sizeof(*string));
        wait(0);
        fprintf(stderr, "I waited for my child\n");
    }

    return 0;
}
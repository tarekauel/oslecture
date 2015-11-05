#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define SMO_NAME "/DEEDS_lab1_shm"

int my_value = 42;

int parent = 1;

void check(int error, char* command) {
    if (error < 0) {
        fprintf(stderr, "Error %d (parent: %d) at %s, errno: %d", error, parent, command, errno);
        if (parent) {
            shm_unlink(SMO_NAME);
        }
        exit(error);
    }
}

int main() {

    pid_t pid;

    pid = fork();
    check(pid, "fork");
    usleep(150);

    if (pid < 0) {
        fprintf(stderr, "An error occured\n");
        return -1;
    } else if (pid == 0) {
        parent = 0;
        my_value = 18951;
        fprintf(stderr, "C: I'm the child, my pid is %d, my_value is %d\n", getpid(), my_value);
        usleep(500);
        int fd = shm_open(SMO_NAME, O_RDWR, 0666);
        check(fd, "shm_open");
        char* string = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        fprintf(stderr, "C: %s", string);
        sprintf(string, "Hi, I am your child. My PID=%d and my_value=%d\n", getpid(), my_value);
        free(string);
    } else {
        int fd = shm_open(SMO_NAME, O_RDWR | O_CREAT, 0666);
        check(fd, "shm_open");
        check(ftruncate(fd, 100), "ftruncate");
        char* string = mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        check(string == MAP_FAILED, "mmap");
        sprintf(string, "Hi, I am your parent. My PID=%d and my_value=%d\n", getpid(), my_value);
        usleep(2000);
        fprintf(stderr, "P: %s", string);
        free(string);
        check(munmap(string, 100), "munmap");
        wait(0);
        check(shm_unlink(SMO_NAME), "shm_unlink");
    }
    return 0;
}
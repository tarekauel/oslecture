#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <errno.h>

int my_value = 42;

void check(int error, char* command) {
    if (error < 0) {
        fprintf(stderr, "Error %d at %s, errno: %d", error, command, errno);
        exit(error);
    }
}

void* child_execution(void* args) {
    usleep(150);
    my_value = 18951;
    fprintf(stderr, "I'm the child, my pid is %d, my_value is %d\n", getpid(), my_value);
    usleep(500);
    int *res = malloc(sizeof(int));
    *res = 0;
    pthread_exit((void*)res);
}

int main() {

    pthread_t thread;

    check(pthread_create(&thread, NULL, child_execution, NULL), "pthread_create");
    usleep(150);
    fprintf(stderr, "I'm the parent, my pid is %d, my_value is %d\n", getpid(), my_value);

    int* thread_res;
    check(pthread_join(thread, (void**) &thread_res), "pthread_join");
    fprintf(stderr, "I waited for my child\n");
    return 0;
}
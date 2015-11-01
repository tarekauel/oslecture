#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

int my_value = 42;

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

    int err = pthread_create(&thread, NULL, child_execution, NULL);
    if (err != 0) {
        fprintf(stderr, "An error occured\n");
        return -1;
    }
    usleep(150);
    fprintf(stderr, "I'm the parent, ym pid is %d, my_value is %d\n", getpid(), my_value);

    int* thread_res;
    err = pthread_join(thread, (void**) &thread_res);
    if (err != 0 || *thread_res != 0) {
        fprintf(stderr, "An error occured\n");
        return -1;
    }
    fprintf(stderr, "I waited for my child\n");
    return 0;
}
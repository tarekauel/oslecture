#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueuq.h>
#include <fcntl.h>

#define QUEUE_NAME "/DEEDS_lab1_mq"

int my_value = 42;

int main() {

    mqd_t queue;
    pid_t pid;

    char* string = malloc(100);

    pid = fork();
    usleep(150);

    int err;

    if (pid < 0) {
        fprintf(stderr, "An error occured\n");
        return -1;
    } else if (pid == 0) {
        fprintf(stderr, "I'm the child, my pid is %d, my_value is %d\n", getpid(), my_value);
        usleep(500);
        queue = mq_open(QUEUE_NAME, O_RDONLY);
        if (queue == -1) {
            fprintf(stderr, "Child couldn't connect to mq.");
            return -1;
        }
        ssize_t size;
        size = mq_receive(QUEUE_NAME, string, sizeof(*string), 0);
        if (size >= 0) {
            fprintf(stderr, "%s", string);
        } else {
            fprintf(stderr, "Error while receiving message");
            err = mq_close(QUEUE_NAME);
            if (err < 0) {
                fprintf(stderr, "Error while closing message queue");
            }
            return -1;
        }
        err = mq_close(QUEUE_NAME);
        if (err < 0) {
            fprintf(stderr, "Error while closing message queue");
            return -1;
        }
    } else {
        queue = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY);
        sprintf(string, "Hi, I am your parent. My PID=%d and my_value=%d\n", getpid(), my_value);
        err = mq_send(QUEUE_NAME, string, sizeof(*string), 0);
        if (err == -1) {
            fprintf(stderr, "Couldn't send message from parent");
        }
        err = mq_close(QUEUE_NAME);
        wait(0);
        err = mq_unlink(QUEUE_NAME);
    }

    return 0;
}
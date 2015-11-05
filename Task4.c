#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>

#define QUEUE_NAME "/DEEDS_lab1_mq"

int my_value = 42;

int parent = 1;

void check(int error, char* command) {
    if (error < 0) {
        fprintf(stderr, "Error %d (parent: %d) at %s, errno: %d\n", error, parent, command, errno);
        if (parent) {
            mq_unlink(QUEUE_NAME);
        }
        exit(error);
    }
}

int main() {

    pid_t pid;
    mqd_t mqd;
    ssize_t bytes_read;
    char* string = malloc(100);

    pid = fork();
    check(pid, "fork");
    usleep(150);

    struct mq_attr attr;
    attr.mq_msgsize = 100;
    attr.mq_maxmsg = 1;

    if (pid == 0) {
        parent = 0;
        my_value = 18951;
        fprintf(stderr, "C: I'm the child, my pid is %d, my_value is %d\n", getpid(), my_value);
        usleep(500);
        mqd = mq_open(QUEUE_NAME, O_RDWR, 0644, &attr);
        check(mqd, "mq_open");

        bytes_read = mq_receive(mqd, string, 100, 0);
        check(bytes_read, "mq_receive");
        fprintf(stderr, "C: %s", string);
        sprintf(string, "Hi, I am your child. My PID=%d and my_value=%d\n", getpid(), my_value);
        check(mq_send(mqd, string, 100, 0), "mq_send");
        check(mq_close(mqd), "mq_close");
    } else {
        mqd = mq_open(QUEUE_NAME, O_RDWR | O_CREAT, 0644, &attr);
        check(mqd, "mq_open");
        sprintf(string, "Hi, I am your parent. My PID=%d and my_value=%d\n", getpid(), my_value);
        check(mq_send(mqd, string, 100, 0), "mq_send");

        usleep(2000);
        bytes_read = mq_receive(mqd, string, 100, 0);
        check(bytes_read, "mq_receive");
        fprintf(stderr, "P: %s", string);
        check(mq_close(mqd), "mq_close");
        wait(0);
        check(mq_unlink(QUEUE_NAME), "mq_unlink");
    }
    return 0;
}
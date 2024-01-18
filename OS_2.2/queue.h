#ifndef LAB_2_QUEUE_H
#define LAB_2_QUEUE_H


#define _GNU_SOURCE

#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <bits/unistd_ext.h>

typedef struct _QueueNode {
    int val;
    struct _QueueNode *next;
} qnode_t;

typedef struct _Queue {
    // start of the queue
    qnode_t *first;
    // end of the queue
    qnode_t *last;


    // prints the information about queue
    pthread_t qmonitor_tid;

    int count;
    int max_count;

    // queue statistics
    long add_attempts;
    long get_attempts;
    long add_count;
    long get_count;
} queue_t;


queue_t *queue_init(int max_count);

void queue_destroy(queue_t *q);

int queue_add(queue_t *q, int val);

int queue_get(queue_t *q, int *val);

void queue_print_stats(queue_t *q);

void print_queue(queue_t *q);

void signal_handler(int sig);

#endif //LAB_2_QUEUE_H

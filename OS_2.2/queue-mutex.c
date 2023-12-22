#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

#include <pthread.h>
#include <sched.h>
#include <signal.h>
#include <stdatomic.h>

#include "queue.h"

#define RED "\033[41m"
#define NOCOLOR "\033[0m"


pthread_mutex_t mutex;

void *qmonitor(void *arg) {
    queue_t *q = (queue_t *) arg;

    printf("qmonitor: [%d %d %d]\n", getpid(), getppid(), gettid());

    while (1) {
        queue_print_stats(q);
        sleep(1);
    }

    return NULL;
}

// initializes the queue (not nodes)!
queue_t *queue_init(int max_count) {
    pthread_mutex_init(&mutex, NULL);
    int err;
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        printf("Cannot allocate memory for a queue\n");
        abort();
    }

    q->first = NULL;
    q->last = NULL;
    q->max_count = max_count;
    q->count = 0;

    q->add_attempts = q->get_attempts = 0;
    q->add_count = q->get_count = 0;


    // starting the printer
    err = pthread_create(&q->qmonitor_tid, NULL, qmonitor, q);
    if (err) {
        printf("queue_init: pthread_create() failed: %s\n", strerror(err));
        abort();
    }

    return q;
}

void queue_destroy(queue_t *q) {
    qnode_t *tmp = q->first;
    while (tmp) {
        qnode_t *del = tmp;
        tmp = tmp->next;
        free(del);
    }
    pthread_mutex_destroy(&mutex);
}

int queue_add(queue_t *q, int val) {
    pthread_mutex_lock(&mutex);
    q->add_attempts++;


    assert(q->count <= q->max_count);


    // check for fullness of the queue
    if (q->count == q->max_count) {
        pthread_mutex_unlock(&mutex);
        return 0;
    }

    // creating a new node
    qnode_t *new = malloc(sizeof(qnode_t));
    if (!new) {
        printf("Cannot allocate memory for new node\n");
        abort();
    }


    // initializing a new node
    new->val = val;
    new->next = NULL;

    // case of empty queue
    if (!q->first) {
        q->first = q->last = new;
    } else {
        q->last->next = new;
        q->last = new;
    }


    // successful add
    q->count++;
    q->add_count++;
    pthread_mutex_unlock(&mutex);
    return 1;
}


int queue_get(queue_t *q, int *val) {
    pthread_mutex_lock(&mutex);
    q->get_attempts++;

    assert(q->count >= 0);

    // check queue for emptiness
    if (q->count == 0) {
        pthread_mutex_unlock(&mutex);
        return 0;
    }

    // getting the very first node of the queue
    if (q->first == NULL) {
        puts("a");
    }
    qnode_t *tmp = q->first;
    *val = tmp->val;
    q->first = q->first->next;

//    printf("%p\n", tmp);
    // successful get
    free(tmp);
    q->count--;
    q->get_count++;
    pthread_mutex_unlock(&mutex);
    return 1;
}

void queue_print_stats(queue_t *q) {
    printf("queue stats: current size %d; attempts: (add: %ld, get: %ld, %ld); success (add: %ld, get: %ld, %ld)\n",
           q->count,
           q->add_attempts, q->get_attempts, q->add_attempts - q->get_attempts,
           q->add_count, q->get_count, q->add_count - q->get_count);
}


void print_queue(queue_t *q) {
    qnode_t *tmp = q->first;
    printf("queue: [");
    while (tmp) {
        printf("%d ", tmp->val);
        tmp = tmp->next;
    }
    puts("]");
}



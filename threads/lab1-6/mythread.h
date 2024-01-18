#ifndef MYTHREAD_H
#define MYTHREAD_H

#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#define MY_STACK_SIZE 4096 * 8

typedef unsigned long int mythread_t;

typedef void *(*start_routine_t)(void *);

typedef struct mythread_struct_t mythread_struct_t;


int mythread_create(mythread_t *thread, void *(*start_routine)(void *),
                    void *arg);

int mythread_join(mythread_t mytid, void **retval);

#endif /* MYTHREAD_H */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void* new_thread(void* arg) {
    int err;
    // сам убивает свои ресруры, его не надо join
    err = pthread_detach(pthread_self());
    if (err) {
        printf("pthread_detach failed: %s\n", strerror(err));
        return NULL;
    }
    printf("New thread: [%d]\n", gettid());
    return NULL;
}

int main() {
    pthread_t thread1;
    int err;

    while (1) {
        err = pthread_create(&thread1, NULL, new_thread, NULL);
        if (err) {
            printf("main: pthread_create() failed: %s\n", strerror(err));
            return -1;
        }
    }
}
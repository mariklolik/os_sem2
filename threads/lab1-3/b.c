#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
    int int_val;
    char *string;
} my_struct;

void *new_thread(void *arg) {
    my_struct *get_item = (my_struct *) arg;
    printf("New thread: [%d]\nStruct info:\nInt num: %d\nString: %s\n", gettid(), get_item->int_val, get_item->string);
    free(get_item); // тут их убираем
    return NULL;
}

int main() {
    pthread_t thread1;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int err;
    // выделяем данные на куче, чтобы они не зависили от main потока
    my_struct *item = (my_struct *) malloc(sizeof(my_struct));
    if (!item) {
        perror("Malloc failed.\n");
        return -1;
    }
    item->int_val = 666;
    item->string = "string";

    err = pthread_create(&thread1, &attr, new_thread, (void *) item);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        free(item);
        return -1;
    }
    printf("Main thread: [%d]\n", gettid());
    pthread_exit(0);
}
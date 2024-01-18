#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void cleanup_handler(void *arg) {
    // Это функция очистки, которая будет вызвана при отмене потока
    char *str = (char *) arg;
    free(str);
}

void *new_thread(void *arg) {
    char *hello_str = (char *) malloc(12);
    if (hello_str == NULL) {
        perror("Memory allocation failed");
        pthread_exit(NULL);
    }

    strcpy(hello_str, "hello world");
    pthread_cleanup_push(cleanup_handler,
                         hello_str); // Устанавливаем функцию очистки, которая вызовется при отмене потока

    while (1) {
        printf("%s\n", hello_str);
    }

    pthread_cleanup_pop(
            1); // Вызываем функцию очистки, если поток не прервался, 1 - cleanup_handler вызывыается, 0 не вызывается
    return NULL;
}

int main() {
    pthread_t thread1;
    int err;

    err = pthread_create(&thread1, NULL, new_thread, NULL);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    sleep(5);
    err = pthread_cancel(thread1);
    if (err) {
        printf("main: pthread_cancel() failed: %s\n", strerror(err));
        return -1;
    }

    err = pthread_join(thread1, NULL);
    if (err) {
        printf("main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }

    printf("Main thread: [%d]\n", gettid());
}
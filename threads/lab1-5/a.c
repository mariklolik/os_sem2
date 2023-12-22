#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

void *thread1(void *arg) {
    sigset_t mask; // маска какие сигналы принимать (всего 31 сигнал)
    sigfillset(&mask); // все сигналы в 1 (т.е блокируем все сигналы)
    pthread_sigmask(SIG_SETMASK, &mask, NULL); // устанавливаем маску на этот поток
    // тут все blocked
    printf("Thread 1: Blocking all signals\n");

    while (1) {
        // Infinite loop to keep the thread running
    }

    return NULL;
}

void sigint_handler(int sig) {
    write(0, "Received SIGINT in Thread 2\n", 28);
}

void *thread2(void *arg) {
    // этот поток будет принимать sigint (ctrl+c)
    struct sigaction sa; // структура, которая описывает поведение потока при получении сигналов
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask); // все сигналы в 0
    sa.sa_flags = 0;
    // при помощи обработчика сигнала
    sigaction(SIGINT, &sa, NULL); // установить действие при получении сигнала
    printf("Thread 2 is waiting for SIGINT\n");
    // SIGINT pending
    while (1) {
        // Infinite loop to keep the thread running
    }

    return NULL;
}

void *thread3(void *arg) {
    // то же самое что второй только через маску
//    sigset_t mask;
//    int sig;
//    sigemptyset(&mask);
//
//    // слушать сигквит
//    sigaddset(&mask, SIGQUIT);
//
//    printf("Thread 3 is waiting for SIGQUIT using sigwait()\n");
//
//    // блокирую поток пока не получу сигнал
//    // SIGQUIT pending
//    if (sigwait(&mask, &sig) == 0) { // ожидаем сигнал
//        // ctrl + \
//
//        printf("Thread 3 received SIGQUIT\n");
//    }


    return NULL;
}

int main() {
    // pending ignored blocked
    // gcc -lpthread a.c -o a.o
    // cmake .
    // make
    // ./lab1_5
    // если хочется чтобы после ctrl + \ продолжил работать
//    sigset_t mask;
//    sigfillset(&mask);
//    pthread_sigmask(SIG_SETMASK, &mask, NULL);
//    sigdelset(&mask, SIGQUIT);
    pthread_t t1, t2, t3;
    int err;

    // Create threads
    err = pthread_create(&t1, NULL, thread1, NULL);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_create(&t2, NULL, thread2, NULL);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_create(&t3, NULL, thread3, NULL);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    pthread_kill(t3, SIGINT);
    // Wait for threads to finish
    err = pthread_join(t1, NULL);
    if (err) {
        printf("main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_join(t2, NULL);
    if (err) {
        printf("main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }

    err = pthread_join(t3, NULL);
    if (err) {
        printf("main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }

    return 0;
}

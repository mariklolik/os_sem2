#define _GNU_SOURCE

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int global_var = 42;
static int static_var = 11;

void *mythread(void *arg) {
    int local_var = 99;
    const int const_var = 101;
    printf("mythread [pid: %d\tppid: %d\ttid: %d\tpthread_self(): %ld]: Hello from mythread!\n", getpid(), getppid(),
           gettid(), pthread_self());
    printf("local var: %p\tstatic var: %p\tconst var: %p\tglobal var: %p\n", &local_var, &static_var, &const_var,
           &global_var);

    // global_var static_var - data segment
    // local_var const_var - thread stack
    global_var = 666;
    local_var = pthread_self(); // у всех разная т.к у каждого потока свой стек
    printf("Local var after change: %d\n", local_var);
    printf("Static var after change: %d\n",
           static_var); // у всех одинаковый т.к статическая перменная общая для всех потоков в одном процессе (лежит в области глоабльной памяти)
    return NULL;
}

int main() {
    pthread_t *threads = (pthread_t *) malloc(sizeof(pthread_t) * 5);
    int err;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    printf("Static var before change: %d\n", static_var);
    static_var = 150;
    for (int i = 0; i < 5; ++i) {
        err = pthread_create(&threads[i], NULL, mythread, NULL);
        if (err) {
            printf("main: pthread_create() failed: %s\n", strerror(err));
            return -1;
        }
    }
    sleep(5);
    free(threads);
    printf("main: global_var: %d\n", global_var);
    return 0;
}

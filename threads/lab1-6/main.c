#include "mythread.h"
#include <stdio.h>

void * my_thread_function(void * arg){
    printf("Hello from new thread\n");
    return NULL;
}
int main() {
    // gcc mythread.c main.c -o main.o
    mythread_t thread;

    int flag = mythread_create(&thread, my_thread_function, NULL); // -1 создание потока не удалось; 0 удалось
    void * retval;
    mythread_join(thread, &retval);
    return 0;
}

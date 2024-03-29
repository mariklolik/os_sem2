#include "mythread.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

int thread_num = 0;

const int mythread_clone_flags =
        (CLONE_VM | CLONE_FS | CLONE_SIGHAND | CLONE_FILES | CLONE_THREAD |
         CLONE_SYSVSEM | /* CLONE_SETTLS | */ CLONE_PARENT_SETTID |
         CLONE_CHILD_CLEARTID);

struct mythread_struct_t {
    // Управляющая структура чтобы контроилировать поток из main
    int mythread_id;
    start_routine_t start_routine;
    void *arg;
    void *retval;
    // optimize flags (bit operations)
    volatile int joined;
    volatile int exited;
};

static void *create_stack(off_t size, int thread_num) {
    // будем хранить стек в файле
    int stack_fd;
    void *stack;

    char filename[64];
    snprintf(filename, 64, "thread_stack-%d", thread_num);

    stack_fd = open(filename, O_RDWR | O_CREAT, 0660);
    if (stack_fd == -1) {
        printf("error opening stack file\n");
        return NULL;
    }
    int err = ftruncate(stack_fd, 0);
    if (err == -1) {
        printf("error truncating in stack file\n");
        close(stack_fd);
        return NULL;
    }
    // делаем файлу размер size
    err = ftruncate(stack_fd, size);
    if (err == -1) {
        printf("error truncating in stack file\n");
        close(stack_fd);
        return NULL;
    }

    // маплю стек из файла в адресное пространство процесса
    stack = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_STACK | MAP_SHARED,
                 stack_fd, 0);
    if (stack == MAP_FAILED) {
        printf("error mapping stack to file\n");
        return NULL;
    }
    // файл больше не нужен, закрываю
    err = close(stack_fd);
    // почистить память
    memset(stack, 0x00, size);
    return stack;
}

static int mythread_startup(void *arg) {
    // обертка над поточной функцией передаем ей указатель на mythread
    mythread_struct_t *mythread = (mythread_struct_t *) arg;
    if (mythread == NULL) {
        printf("mythread startup error: thread is null\n");
        return -1;
    }

    mythread->retval = mythread->start_routine(mythread->arg);
    // когда выполнили поточную функцию ставим exited
    mythread->exited = 1;

    // каждую сек проверям заджойинили нас или нет
    while (!mythread->joined) {
        sleep(1);
    }

    return 0;
}

int mythread_create(mythread_t *tid, start_routine_t start_routine, void *arg) {
    // Надо выделить стек (mmap) (create_stack)
    // Создать таску (clone)
    // Запустить поточную функцию в новом потоке и передать ей аргументы (clone)
    void *mem_allocated =
            create_stack(MY_STACK_SIZE + sizeof(mythread_struct_t), thread_num);
    if (mem_allocated == NULL) {
        return -1;
    }
    mythread_struct_t *mythread = mem_allocated;
    // на дно стека кладем структуру, все что после структуры это и есть стек процесса
    void *child_stack = mem_allocated + sizeof(mythread_struct_t);
    *tid = mythread;
    // Инициализируем управлюящую структуру
    mythread->mythread_id = thread_num;
    mythread->start_routine = start_routine;
    mythread->joined = 0;
    mythread->exited = 0;
    mythread->arg = arg;

    // func stack flags mythread
    pid_t child_pid = clone(mythread_startup, child_stack + MY_STACK_SIZE,
                            mythread_clone_flags, (void *) mythread);
    if (child_pid == -1) {
        printf("create_thread: clone failed: %s\n", strerror(errno));
        return -1;
    }
    thread_num++;
    return 0;
}

int mythread_join(mythread_t mytid, void **retval) {
    mythread_struct_t *mythread = mytid;
    // ждем пока завершится
    while (!mythread->exited) {
        sleep(1);
    }
    if (retval != NULL) {
        *retval = mythread->retval;
    }
    mythread->joined = 1;
    return 0;
}

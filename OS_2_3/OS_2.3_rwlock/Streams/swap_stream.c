#include "swap_stream.h"

void* swap_stream_routine(void* arg) {
    printf(RED "started swap\n" RESET);
    fflush(stdout);
    spawn_context* spawn_ctx = arg;

    while (1) {
        Node* cur = spawn_ctx->storage->start;

        while (1) {
            pthread_rwlock_wrlock(&cur->rwlock);

            if (!cur->next || !cur->next->next) {
                pthread_rwlock_unlock(&cur->rwlock);
                break;
            }

            pthread_rwlock_wrlock(&cur->next->rwlock);
            pthread_rwlock_wrlock(&cur->next->next->rwlock);

            if (rand() % 2 == 1) {
                swap(cur);
                spawn_ctx->stream_context.count_of_pairs++;
            }

            Node* prev_cur = cur;
            cur = cur->next;

            pthread_rwlock_unlock(&prev_cur->next->next->rwlock);
            pthread_rwlock_unlock(&prev_cur->next->rwlock);
            pthread_rwlock_unlock(&prev_cur->rwlock);
        }
    }
}

void swap(Node* cur) {
    Node* first = cur;
    Node* second = first->next;
    Node* third = second->next;
    first->next = third;
    second->next = third->next;
    third->next = second;
}

pthread_t spawn_swap_stream(spawn_context* s) {
    pthread_t tid;
    pthread_attr_t attr;

    if (-1 == pthread_attr_init(&attr)) {
        perror("failed to init thread's attribute!");
        return (pthread_t) - 1;
    }

    if (-1 == pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) {
        perror("failed to set joinable state for thread!");
        return (pthread_t) - 1;
    }

    if (-1 == pthread_create(&tid, &attr, swap_stream_routine, s)) {
        perror("failed to create thread!");
        return (pthread_t) - 1;
    }

    return tid;
}

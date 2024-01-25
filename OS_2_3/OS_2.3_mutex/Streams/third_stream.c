#include "third_stream.h"


void* third_stream_routine(void* arg) {
    puts("third stream start! (eq)");
    fflush(stdout);
    spawn_context* spawn_ctx = (spawn_context *)arg;
    while (1) {
        Node* cur = spawn_ctx->storage->start;
        while (1) {
            pthread_mutex_lock(&cur->mutex);
            if (!cur->next) {
                pthread_mutex_unlock(&cur->mutex);
                break;
            }
            pthread_mutex_lock(&cur->next->mutex);
            if (strlen(cur->str) == strlen(cur->next->str)) {
                spawn_ctx->stream_context.count_of_pairs++;
            }
            Node* prev_cur = cur;
            cur = cur->next;
            pthread_mutex_unlock(&prev_cur->next->mutex);
            pthread_mutex_unlock(&prev_cur->mutex);
        }
        spawn_ctx->stream_context.iterations++;
    }
}

pthread_t spawn_third_stream(spawn_context* s) {
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
    if (-1 == pthread_create(&tid, &attr, third_stream_routine, s)) {
        perror("failed to create thread!");
        return (pthread_t) - 1;
    }
    return tid;
}



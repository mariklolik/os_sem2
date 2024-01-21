#include "second_stream.h"

void* second_stream_routine(void* arg) {
    puts("second stream start! (asc)");
    fflush(stdout);
    spawn_context* spawn_ctx = (spawn_context*)arg;

    while (1) {
        Node* cur = spawn_ctx->storage->start;

        while (1) {
            pthread_rwlock_rdlock(&cur->rwlock);

            if (!cur->next) {
                pthread_rwlock_unlock(&cur->rwlock);
                break;
            }

            pthread_rwlock_rdlock(&cur->next->rwlock);

            if (strlen(cur->str) > strlen(cur->next->str)) {
                spawn_ctx->stream_context.count_of_pairs++;
            }

            Node* prev_cur = cur;
            cur = cur->next;

            pthread_rwlock_unlock(&prev_cur->next->rwlock);
            pthread_rwlock_unlock(&prev_cur->rwlock);
        }

        spawn_ctx->stream_context.iterations++;
    }
}

pthread_t spawn_second_stream(spawn_context* s) {
    pthread_t tid;
    pthread_attr_t attr;

    if (-1 == pthread_attr_init(&attr)) {
        perror("failed to init thread's attribute!");
        return NULL;
    }

    if (-1 == pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) {
        perror("failed to set joinable state for thread!");
        return NULL;
    }

    if (-1 == pthread_create(&tid, &attr, second_stream_routine, s)) {
        perror("failed to create thread!");
        return NULL;
    }

    return tid;
}

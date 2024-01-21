#include "monitor.h"

void* print_info(const void* arg) {
    const monitor_ctx* monitor_ctx = arg;
    while (1) {
        printf(GREEN "number of string pairs in ascending order: %d\n" RESET, monitor_ctx->first_ctx.count_of_pairs);
        printf(CYAN "number of string pairs in descending order: %d\n" RESET, monitor_ctx->second_ctx.count_of_pairs);
        printf(YELLOW "number of string pairs with equal len: %d\n" RESET, monitor_ctx->third_ctx.count_of_pairs);
        printf(RED "number of swaps: %d\n" RESET, monitor_ctx->swap_ctx.count_of_pairs);
        sleep(1);
    }
}

pthread_t spawn_monitor(void* ctx) {
    pthread_t tid;
    pthread_attr_t attr;
    if (-1 == pthread_attr_init(&attr)) {
        perror("failed to init thread's attribute!");
        return EXIT_FAILURE;
    }
    if (-1 == pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE)) {
        perror("failed to set joinable state for thread!");
        return EXIT_FAILURE;
    }
    if (-1 == pthread_create(&tid, &attr, print_info, ctx)) {
        perror("failed to create thread!");
        return EXIT_FAILURE;
    }
    return tid;
}

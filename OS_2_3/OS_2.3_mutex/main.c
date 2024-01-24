#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "LinkedListStorage/Storage.h"
#include "Streams/first_stream.h"
#include "Streams/second_stream.h"
#include "Streams/swap_stream.h"
#include "Streams/third_stream.h"
#include "Streams/stream_structs.h"

int main() {
    srand(time(NULL));
    int storage_size = 0;
    printf("size: ");
    if (-1 == scanf("%d", &storage_size)) {
        return EXIT_FAILURE;
    }
    if (storage_size <= 1) {
        fprintf(stderr, "size must be greater than 1");
        return EXIT_FAILURE;
    }
    Storage *s = init_storage(storage_size);

    spawn_context *first_spawn = malloc(sizeof(spawn_context));
    first_spawn->storage = s;
    memset(&first_spawn->stream_context, 0, sizeof(stream_context));
    pthread_t first_s = spawn_first_stream(first_spawn);

    if (((pthread_t) - 1) == first_s) {
        destroy_storage(s);
        return EXIT_FAILURE;
    }

    spawn_context *second_spawn = malloc(sizeof(spawn_context));
    second_spawn->storage = s;
    memset(&second_spawn->stream_context, 0, sizeof(stream_context));
    pthread_t second_s = spawn_second_stream(second_spawn);
    if (((pthread_t) - 1) == second_s) {
        destroy_storage(s);
        return EXIT_FAILURE;
    }

    spawn_context *third_spawn = malloc(sizeof(spawn_context));
    third_spawn->storage = s;
    memset(&third_spawn->stream_context, 0, sizeof(stream_context));
    pthread_t third_s = spawn_third_stream(third_spawn);
    if (((pthread_t) - 1) == third_s) {
        destroy_storage(s);
        return EXIT_FAILURE;
    }

    spawn_context *swap_spawn = malloc(sizeof(spawn_context));
    swap_spawn->storage = s;
    memset(&swap_spawn->stream_context, 0, sizeof(stream_context));
    pthread_t swap_stream = spawn_swap_stream(swap_spawn);
    if (((pthread_t) - 1) == swap_stream) {
        destroy_storage(s);
        return EXIT_FAILURE;
    }

    spawn_context *swap_spawn1 = malloc(sizeof(spawn_context));
    swap_spawn1->storage = s;
    memset(&swap_spawn1->stream_context, 0, sizeof(stream_context));
    pthread_t swap_stream1 = spawn_swap_stream(swap_spawn1);
    if (((pthread_t) - 1) == swap_stream1) {
        destroy_storage(s);
        return EXIT_FAILURE;
    }
    spawn_context *swap_spawn2 = malloc(sizeof(spawn_context));
    swap_spawn2->storage = s;
    memset(&swap_spawn2->stream_context, 0, sizeof(stream_context));
    pthread_t swap_stream2 = spawn_swap_stream(swap_spawn2);
    if (((pthread_t) - 1) == swap_stream2) {
        destroy_storage(s);
        return EXIT_FAILURE;
    }

    while (1){
        printf(GREEN "number of string pairs in ascending order: %d\n" RESET, first_spawn->stream_context.count_of_pairs);
        printf(CYAN "number of string pairs in descending order: %d\n" RESET, second_spawn->stream_context.count_of_pairs);
        printf(YELLOW "number of string pairs with equal len: %d\n" RESET, third_spawn->stream_context.count_of_pairs);
        printf(RED "number of swaps: %d\n" RESET, swap_spawn->stream_context.count_of_pairs);
        sleep(1);
    }
    pthread_join(first_s, NULL);
    pthread_join(second_s, NULL);
    pthread_join(third_s, NULL);
    pthread_join(swap_stream, NULL);
    pthread_join(swap_stream1, NULL);
    pthread_join(swap_stream2, NULL);
    destroy_storage(s);
    return 0;
}

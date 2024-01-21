#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "LinkedListStorage/Storage.h"
#include "StreamMonitor/monitor.h"
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
    Storage* s = init_storage(storage_size);

    spawn_context* first_spawn = malloc(sizeof(spawn_context));
    first_spawn->storage = s;
    memset(&first_spawn->stream_context, 0, sizeof(stream_context));
    pthread_t first_s = spawn_first_stream(first_spawn);

    if (NULL == first_s) {
        destroy_storage(s);
        return EXIT_FAILURE;
    }

    spawn_context* second_spawn = malloc(sizeof(spawn_context));
    second_spawn->storage = s;
    memset(&second_spawn->stream_context, 0, sizeof(stream_context));
    pthread_t second_s = spawn_second_stream(second_spawn);
    if (NULL == second_s) {
        destroy_storage(s);
        return EXIT_FAILURE;
    }

    spawn_context* third_spawn = malloc(sizeof(spawn_context));
    third_spawn->storage = s;
    memset(&third_spawn->stream_context, 0, sizeof(stream_context));
    pthread_t third_s = spawn_third_stream(third_spawn);
    if (NULL == third_s) {
        destroy_storage(s);
        return EXIT_FAILURE;
    }

    spawn_context* swap_spawn = malloc(sizeof(spawn_context));
    swap_spawn->storage = s;
    memset(&swap_spawn->stream_context, 0, sizeof(stream_context));
    pthread_t swap_stream = spawn_swap_stream(swap_spawn);
    if (NULL == swap_stream) {
        destroy_storage(s);
        return EXIT_FAILURE;
    }

    spawn_context* swap_spawn1 = malloc(sizeof(spawn_context));
    swap_spawn1->storage = s;
    memset(&swap_spawn1->stream_context, 0, sizeof(stream_context));
    pthread_t swap_stream1 = spawn_swap_stream(swap_spawn1);
    if (NULL == swap_stream1) {
        destroy_storage(s);
        return EXIT_FAILURE;
    }
    spawn_context* swap_spawn2 = malloc(sizeof(spawn_context));
    swap_spawn2->storage = s;
    memset(&swap_spawn2->stream_context, 0, sizeof(stream_context));
    pthread_t swap_stream2 = spawn_swap_stream(swap_spawn2);
    if (NULL == swap_stream2) {
        destroy_storage(s);
        return EXIT_FAILURE;
    }
//    monitor_ctx mon_ctx = {
//        first_spawn->stream_context, second_spawn->stream_context, third_spawn->stream_context, swap_spawn->stream_context
//    };
//    const pthread_t monitor = spawn_monitor(&mon_ctx);
//    if (NULL == monitor) {
//        destroy_storage(s);
//        return EXIT_FAILURE;
//    }
    pthread_join(first_s, NULL);
    pthread_join(second_s, NULL);
    pthread_join(third_s, NULL);
    pthread_join(swap_stream, NULL);
    pthread_join(swap_stream1, NULL);
    pthread_join(swap_stream2, NULL);
//    pthread_join(monitor, NULL);
    destroy_storage(s);
    return 0;
}

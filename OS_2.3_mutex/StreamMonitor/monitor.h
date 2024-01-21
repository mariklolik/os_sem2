#pragma once
#include <unistd.h>
#include "../Streams/first_stream.h"
#include "../Streams/second_stream.h"
#include "../Streams/third_stream.h"

typedef struct {
    stream_context first_ctx;
    stream_context second_ctx;
    stream_context third_ctx;
    stream_context swap_ctx;
} monitor_ctx;

void* print_info(const void* arg);

pthread_t spawn_monitor(void* ctx);

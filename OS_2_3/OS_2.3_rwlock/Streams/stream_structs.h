#pragma once

typedef struct {
    volatile int count_of_pairs;
    volatile int iterations;
} stream_context;



typedef struct {
    stream_context stream_context;
    Storage* storage;
} spawn_context;

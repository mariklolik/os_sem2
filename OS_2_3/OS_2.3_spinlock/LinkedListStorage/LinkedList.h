#pragma once

#define MAX_STRING_SIZE 100

#include <pthread.h>
#include <stdlib.h>

typedef struct _Node {
    char str[MAX_STRING_SIZE];
    struct _Node* next;
    pthread_spinlock_t spinlock;
} Node;

Node* create_node();

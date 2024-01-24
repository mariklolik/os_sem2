#pragma once

#define MAX_STRING_SIZE 100

#include <pthread.h>
#include <stdlib.h>


typedef struct _Node {
    char str[100];
    struct _Node* next;
    pthread_mutex_t mutex;
} Node;

Node* create_node();

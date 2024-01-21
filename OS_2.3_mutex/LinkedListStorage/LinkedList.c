#include "LinkedList.h"

#include <limits.h>

Node* create_node() {
    Node* node = malloc(sizeof(Node));
    const int str_length = rand() % (MAX_STRING_SIZE + 1);
    for (int i = 0; i < str_length; ++i) {
        const int abs_rand = rand() % (CHAR_MAX + 1);
        node->str[i] = rand() % 2 ? abs_rand : -abs_rand;
    }
    node->next = NULL;
    pthread_mutex_t pthread_mutex;
    pthread_mutex_init(&pthread_mutex, NULL);
    node->mutex = pthread_mutex;
    return node;
}

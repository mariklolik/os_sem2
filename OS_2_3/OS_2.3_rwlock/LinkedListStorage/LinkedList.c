#include "LinkedList.h"

#include <limits.h>

Node* create_node() {
    Node* node = malloc(sizeof(Node));

    if (node == NULL) {
        return NULL;
    }

    const int str_length = rand() % (MAX_STRING_SIZE + 1);
    for (int i = 0; i < str_length; ++i) {
        const int abs_rand = rand() % (CHAR_MAX + 1);
        node->str[i] = rand() % 2 ? abs_rand : -abs_rand;
    }

    node->next = NULL;

    pthread_rwlock_t pthread_rwlock;
    if (pthread_rwlock_init(&pthread_rwlock, NULL) != 0) {
        free(node);
        return NULL;
    }
    node->rwlock = pthread_rwlock;

    return node;
}

#include "Storage.h"

Storage* init_storage(const int size) {
    Storage* storage = malloc(sizeof(Storage));
    for (int i = 0; i < size; ++i) {
        Node* n = create_node();
        add_node(storage, n);
    }
    return storage;
}

void print_storage(const Storage* s) {
    const Node* cur = s->start;
    while (cur) {
        if (cur->next)
            printf("%lu->", strlen(cur->str));
        else
            printf("%lu", strlen(cur->str));
        cur = cur->next;
    }
    printf("\n");
    fflush(stdout);
}

void add_node(Storage* s, Node* node) {
    if (!s->start) {
        s->start = node;
        return;
    }
    Node* cur = s->start;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = node;
}

void destroy_storage(Storage* s) {
    Node* cur = s->start;
    while (cur) {
        Node* next = cur->next;
        pthread_spin_destroy(&cur->spinlock);
        free(cur);
        cur = next;
    }
    free(s);
}

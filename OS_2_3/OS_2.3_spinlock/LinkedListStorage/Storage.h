#pragma once
#include <stdio.h>
#include <string.h>
#include "LinkedList.h"

typedef struct {
    Node* start;
} Storage;


Storage* init_storage(int size);
void print_storage(const Storage* s);
void add_node(Storage* s, Node* node);
void destroy_storage(Storage* s);

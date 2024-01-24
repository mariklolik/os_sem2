// ReSharper disable CppUnusedIncludeDirective
#pragma once
#include <string.h>
#include <stdio.h>
#include "../LinkedListStorage/Storage.h"
#include "../Colors.h"
#include "stream_structs.h"




void* swap_stream_routine(void* arg);
void swap(Node* cur);
pthread_t spawn_swap_stream(spawn_context* s);

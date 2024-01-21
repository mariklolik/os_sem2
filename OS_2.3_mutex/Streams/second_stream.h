// ReSharper disable CppUnusedIncludeDirective
#pragma once
#include <string.h>
#include <stdio.h>
#include "../LinkedListStorage/Storage.h"
#include "../Colors.h"
#include "stream_structs.h"

void* second_stream_routine(void* arg);
pthread_t spawn_second_stream(spawn_context* s);
// ReSharper disable CppUnusedIncludeDirective
#pragma once
#include <string.h>
#include <stdio.h>
#include "../LinkedListStorage/Storage.h"
#include "../Colors.h"
#include "stream_structs.h"


void* third_stream_routine(void* arg);
pthread_t spawn_third_stream(spawn_context* s);
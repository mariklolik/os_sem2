cmake_minimum_required(VERSION 3.16)
project(2_3 C)


# cmake .
# make
# ./2_3

set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_FLAGS "-O3 -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread")

add_executable(2_3 main.c
        LinkedListStorage/LinkedList.h
        LinkedListStorage/LinkedList.c
        LinkedListStorage/Storage.h
        LinkedListStorage/Storage.c
        Streams/first_stream.c
        Streams/first_stream.h
        Colors.h
        Streams/second_stream.c
        Streams/second_stream.h
        Streams/third_stream.c
        Streams/third_stream.h
        Streams/swap_stream.c
        Streams/swap_stream.h
        StreamMonitor/monitor.c
        StreamMonitor/monitor.h
        Streams/stream_structs.h)

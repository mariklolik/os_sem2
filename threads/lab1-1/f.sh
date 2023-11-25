gcc e.c -lpthread -o e.out
strace ./e.out

# mmap - map стека потока
# clone - клонировать текущий процесс
# clone3({flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID
# CLONE_VM - clone virtual memory
# CLONE_SIGHAND - вместе обрабатывают сигналы
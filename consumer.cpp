#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>

const int SIZE = 4096;
const char* name = "shared.txt";

void* consume(void* arg) {
    int shm_fd;
    char* ptr;

    shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return NULL;
    }

    ptr = (char*)mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return NULL;
    }

    sem_t* sem_empty = sem_open("/sem_empty", 0);
    sem_t* sem_full = sem_open("/sem_full", 0);

    for (int i = 0; i < 10; ++i) {
        sem_wait(sem_full);

        printf("Consumed: %s\n", ptr);

        sem_post(sem_empty);
        sleep(1); // Simulate some work
    }

    sem_close(sem_empty);
    sem_close(sem_full);

    munmap(ptr, SIZE);
    close(shm_fd);
    shm_unlink(name);
    sem_unlink("/sem_empty");
    sem_unlink("/sem_full");

    return NULL;
}

int main() {
    pthread_t consumer_thread;

    // Create consumer thread
    pthread_create(&consumer_thread, NULL, consume, NULL);

    // Wait for consumer thread to finish
    pthread_join(consumer_thread, NULL);

    return 0;
}

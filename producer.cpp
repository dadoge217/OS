#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <iostream>
#include <random>

const int SIZE = 4096;
const char* name = "shared.txt";

void* produce(void* arg) {
    int shm_fd;
    char* ptr;
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return NULL;
    }

    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate");
        close(shm_fd);
        return NULL;
    }

    ptr = (char*)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return NULL;
    }

    sem_t* sem_empty = sem_open("/sem_empty", O_CREAT, 0666, 1);
    sem_t* sem_full = sem_open("/sem_full", O_CREAT, 0666, 0);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < 10; ++i) {
        sem_wait(sem_empty);

        int random_number1 = dis(gen);
        int random_number2 = dis(gen);
        sprintf(ptr, "Random number 1: %d, Random number 2: %d", random_number1, random_number2);
        std::cout << "Produced: " << random_number1 << ", " << random_number2 << std::endl;

        sem_post(sem_full);
        sleep(1); // Simulate some work
    }

    sem_close(sem_empty);
    sem_close(sem_full);
    sem_unlink("/sem_empty");
    sem_unlink("/sem_full");

    munmap(ptr, SIZE);
    close(shm_fd);

    return NULL;
}

int main() {
    pthread_t producer_thread;

    // Create producer thread
    pthread_create(&producer_thread, NULL, produce, NULL);

    // Wait for producer thread to finish
    pthread_join(producer_thread, NULL);

    return 0;
}

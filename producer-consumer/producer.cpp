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

    // Create shared memory object
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Set the size of the shared memory object
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    // Map the shared memory object into the process's address space
    ptr = (char*)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    // Create semaphores
    sem_t* sem_empty = sem_open("/sem_Empty", O_CREAT, 0666, 1);
    if (sem_empty == SEM_FAILED) {
        perror("sem_open (sem_Empty)");
        munmap(ptr, SIZE);
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    sem_t* sem_full = sem_open("/sem_Full", O_CREAT, 0666, 0);
    if (sem_full == SEM_FAILED) {
        perror("sem_open (sem_Full)");
        sem_close(sem_empty);
        munmap(ptr, SIZE);
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < 10; ++i) {
        sem_wait(sem_empty);

        int random_number1 = dis(gen);
        int random_number2 = dis(gen);
        sprintf(ptr, "%d, %d", random_number1, random_number2);
        std::cout << "Produced: " << random_number1 << ", " << random_number2 << std::endl;

        sem_post(sem_full);
        sleep(1); // Simulate some work
    }

    sem_close(sem_empty);
    sem_close(sem_full);

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

    std::cout << "Producer process has terminated." << std::endl;
    exit(EXIT_SUCCESS);
}

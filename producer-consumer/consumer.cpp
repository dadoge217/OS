#include "header.hpp"

// Initializes shared memory values;
const int SIZE = 4096;
const char* name = "shared.txt";

void* consume(void* arg) {
    sleep(1); // Waits for the producer to initialize the semaphores

    int shm_fd;
    char* ptr;

    // Opens the shared memory file
    shm_fd = shm_open(name, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return NULL;
    }

    // Maps the shared memory object into the process's address space
    ptr = (char*)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return NULL;
    }

    // Opens the semaphores
    sem_t* sem_empty = sem_open("/sem_Empty", 0);
    sem_t* sem_full = sem_open("/sem_Full", 0);

    if (sem_empty == SEM_FAILED || sem_full == SEM_FAILED) {
        perror("sem_open");
        munmap(ptr, SIZE);
        close(shm_fd);
        return NULL;
    }

    for (int i = 0; i < 10; ++i) {
        sem_wait(sem_full); // Checks to see if sem_full = 1. If not, it busy waits.

        printf("Consumed: %s\n", ptr);

        memset(ptr, 0, SIZE); // Clears the shared memory after consuming

        sem_post(sem_empty);  // Signals the sem_empty semaphore to increment to 1, allowing the producer to enter the CS
        sleep(1); // Simulate some work
    }

    // Unmaps and closes everything after the final loop is completed.
    sem_close(sem_empty);
    sem_close(sem_full);

    munmap(ptr, SIZE);
    close(shm_fd);

    // Fully unlinks semaphores and shared file to avoid repeated semaphores across multiple executions
    shm_unlink(name);
    sem_unlink("/sem_Empty");
    sem_unlink("/sem_Full");

    return NULL;
}

int main() {
    pthread_t consumer_thread;

    // Creates the consumer thread
    pthread_create(&consumer_thread, NULL, consume, NULL);

    // Waits for the consumer thread to finish
    pthread_join(consumer_thread, NULL);

    std::cout << "Consumer process has terminated." << std::endl;
    return 0;
}

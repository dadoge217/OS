#include "header.hpp"

// Initializes shared memory values;
const int SIZE = 4096;
const char* name = "shared.txt";

void* produce(void* arg) {
    int shm_fd;
    char* ptr;

    // Create shared memory file
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    // Sets the size of the shared memory file
    if (ftruncate(shm_fd, SIZE) == -1) {
        perror("ftruncate");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    // Maps the shared memory object into the process's address space
    ptr = (char*)mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        exit(EXIT_FAILURE);
    }

    // Creates the semaphores
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

    // Initializes random number seed for producer values
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < 10; ++i) {
        sem_wait(sem_empty); // Checks to see if sem_empty = 1. If not, it busy waits.

        int random_number1 = dis(gen);
        int random_number2 = dis(gen);
        sprintf(ptr, "%d, %d", random_number1, random_number2);
        std::cout << "Produced: " << random_number1 << ", " << random_number2 << std::endl;

        sem_post(sem_full); // Signals the sem_full semaphore to increment to 1, allowing the consumer to enter the CS
        sleep(1); // Simulates some work
    }

    // Unmaps and closes everything after the final loop is completed.
    sem_close(sem_empty);
    sem_close(sem_full);

    munmap(ptr, SIZE);
    close(shm_fd);

    return NULL;
}

int main() {
    pthread_t producer_thread;

    // Creates the producer thread
    pthread_create(&producer_thread, NULL, produce, NULL);

    // Waits for the producer thread to finish
    pthread_join(producer_thread, NULL);

    std::cout << "Producer process has terminated." << std::endl;
    exit(EXIT_SUCCESS);
}

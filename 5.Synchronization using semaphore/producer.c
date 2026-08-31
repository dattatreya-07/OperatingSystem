// producer.c
#include "common.h"

int main() {
    // 1. Create or open shared memory
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(shared_data_t));
    shared_data_t *shared = (shared_data_t *)mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Initialize buffer pointers
    shared->in = 0;
    shared->out = 0;

    // 2. Initialize Semaphores
    sem_t *mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);
    sem_t *empty = sem_open(SEM_EMPTY, O_CREAT, 0666, BUFFER_SIZE);
    sem_t *full = sem_open(SEM_FULL, O_CREAT, 0666, 0);

    printf("Producer started. (Enter -1 to exit)\n\n");

    int input_item;
    while (1) {
        printf("Enter an integer to produce: ");
        if (scanf("%d", &input_item) != 1) {
            printf("Invalid input. Please enter numbers only.\n");
            // Clear input buffer
            while (getchar() != '\n');
            continue;
        }

        // Check for termination signal
        if (input_item == -1) {
            // Send exit signal (-1) to the consumer through the buffer
            sem_wait(empty);
            sem_wait(mutex);
            shared->buffer[shared->in] = -1;
            shared->in = (shared->in + 1) % BUFFER_SIZE;
            sem_post(mutex);
            sem_post(full);
            break;
        }

        printf("[Waiting] Checking if buffer has empty space...\n");
        sem_wait(empty); // Blocks here if the buffer is completely full
        sem_wait(mutex); // Enter critical section

        // Place user item into the buffer
        shared->buffer[shared->in] = input_item; 
        printf(">> [Success] Produced: %d at index %d\n\n", shared->buffer[shared->in], shared->in);
        shared->in = (shared->in + 1) % BUFFER_SIZE;

        sem_post(mutex); // Leave critical section
        sem_post(full);  // Notify consumer
    }

    // Clean up local handles
    munmap(shared, sizeof(shared_data_t));
    close(shm_fd);
    sem_close(mutex);
    sem_close(empty);
    sem_close(full);

    printf("Producer terminated successfully.\n");
    return 0;
}


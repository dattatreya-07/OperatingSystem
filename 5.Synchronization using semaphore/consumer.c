// consumer.c
#include "common.h"

int main() {
    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Shared memory opening failed. Run producer first");
        exit(1);
    }
    shared_data_t *shared = (shared_data_t *)mmap(NULL, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_t *mutex = sem_open(SEM_MUTEX, 0);
    sem_t *empty = sem_open(SEM_EMPTY, 0);
    sem_t *full = sem_open(SEM_FULL, 0);

    printf("Consumer started. Waiting for items...\n\n");

    while (1) {
        sem_wait(full);  // Blocks here if the buffer is completely empty
        sem_wait(mutex); // Enter critical section

        int item = shared->buffer[shared->out];
        
        // Break out of the loop if producer sent exit code
        if (item == -1) {
            sem_post(mutex);
            break;
        }

        printf("<< [Success] Consumed: %d from index %d\n", item, shared->out);
        shared->out = (shared->out + 1) % BUFFER_SIZE;

        sem_post(mutex); // Leave critical section
        sem_post(empty); // Notify producer that a slot freed up

        sleep(1); // Simulate consumption processing time
    }

    // Clean up and unlink shared system resources completely
    munmap(shared, sizeof(shared_data_t));
    close(shm_fd);
    sem_close(mutex);
    sem_close(empty);
    sem_close(full);

    shm_unlink(SHM_NAME);
    sem_unlink(SEM_MUTEX);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);

    printf("\nConsumer detected exit signal. Resources cleared.\n");
    return 0;
}


// common.h
#ifndef COMMON_H
#define COMMON_H

// Add these two lines at the absolute top for POSIX compatibility in C99
#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define SHM_NAME "/shm_producer_consumer"
#define SEM_MUTEX "/sem_mutex"
#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
} shared_data_t;

#endif


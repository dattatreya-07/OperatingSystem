#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 50

void displayResults(int partitions[], int num_partitions, int processes[], int num_processes,
                    int alloc[], const char *title) {
    int i;
    int internal_frag = 0, external_frag = 0;
    int part_used[MAX] = {0};

    for (i = 0; i < num_processes; i++) {
        if (alloc[i] != -1) {
            part_used[alloc[i]] = 1;
            internal_frag += (partitions[alloc[i]] - processes[i]);
        }
    }

    for (i = 0; i < num_partitions; i++) {
        if (!part_used[i]) {
            external_frag += partitions[i];
        }
    }

    printf("\n--- %s ---\n", title);
    printf("Process\tSize\tBlock\tBlock Size\tFragment\n");
   
    for (i = 0; i < num_processes; i++) {
        if (alloc[i] != -1) {
            printf("P%d\t%d\tB%d\t%d\t\t%d\n",
                   i + 1, processes[i], alloc[i] + 1, partitions[alloc[i]],
                   partitions[alloc[i]] - processes[i]);
        } else {
            printf("P%d\t%d\tNot Allocated\n", i + 1, processes[i]);
        }
    }

    printf("Internal Fragmentation: %d\n", internal_frag);
    printf("External Fragmentation: %d\n", external_frag);
}

void firstFit(int partitions[], int num_partitions, int processes[], int num_processes) {
    int i, j;
    int alloc[MAX];
    int part_used[MAX] = {0};

    for (i = 0; i < num_processes; i++) {
        alloc[i] = -1;
    }

    for (i = 0; i < num_processes; i++) {
        for (j = 0; j < num_partitions; j++) {
            if (!part_used[j] && partitions[j] >= processes[i]) {
                alloc[i] = j;
                part_used[j] = 1;
                break;
            }
        }
    }

    displayResults(partitions, num_partitions, processes, num_processes, alloc, "First Fit");
}

void bestFit(int partitions[], int num_partitions, int processes[], int num_processes) {
    int i, j;
    int alloc[MAX];
    int part_used[MAX] = {0};

    for (i = 0; i < num_processes; i++) {
        alloc[i] = -1;
    }

    for (i = 0; i < num_processes; i++) {
        int best_idx = -1;
        int min_diff = INT_MAX;

        for (j = 0; j < num_partitions; j++) {
            if (!part_used[j] && partitions[j] >= processes[i]) {
                int diff = partitions[j] - processes[i];
                if (diff < min_diff) {
                    min_diff = diff;
                    best_idx = j;
                }
            }
        }

        if (best_idx != -1) {
            alloc[i] = best_idx;
            part_used[best_idx] = 1;
        }
    }

    displayResults(partitions, num_partitions, processes, num_processes, alloc, "Best Fit");
}

void worstFit(int partitions[], int num_partitions, int processes[], int num_processes) {
    int i, j;
    int alloc[MAX];
    int part_used[MAX] = {0};

    for (i = 0; i < num_processes; i++) {
        alloc[i] = -1;
    }

    for (i = 0; i < num_processes; i++) {
        int worst_idx = -1;
        int max_diff = -1;

        for (j = 0; j < num_partitions; j++) {
            if (!part_used[j] && partitions[j] >= processes[i]) {
                int diff = partitions[j] - processes[i];
                if (diff > max_diff) {
                    max_diff = diff;
                    worst_idx = j;
                }
            }
        }

        if (worst_idx != -1) {
            alloc[i] = worst_idx;
            part_used[worst_idx] = 1;
        }
    }

    displayResults(partitions, num_partitions, processes, num_processes, alloc, "Worst Fit");
}

void displayInputOrder(int partitions[], int num_partitions, int processes[], int num_processes) {
    int i;

    printf("\nInput Summary\n");
    printf("Partitions: ");
    for (i = 0; i < num_partitions; i++) {
        printf("%d ", partitions[i]);
    }
    printf("\nProcesses:  ");
    for (i = 0; i < num_processes; i++) {
        printf("%d ", processes[i]);
    }
    printf("\n");
}

int main() {
    int partitions[MAX], processes[MAX];
    int num_partitions, num_processes;
    int i, choice;

    printf("Contiguous Memory Allocation\n");

    printf("Enter number of partitions: ");
    scanf("%d", &num_partitions);

    printf("Enter partition sizes: ");
    for (i = 0; i < num_partitions; i++) {
        scanf("%d", &partitions[i]);
    }

    printf("Enter number of processes: ");
    scanf("%d", &num_processes);

    printf("Enter process sizes: ");
    for (i = 0; i < num_processes; i++) {
        scanf("%d", &processes[i]);
    }

    displayInputOrder(partitions, num_partitions, processes, num_processes);

    while (1) {
        printf("\n1. First Fit\n");
        printf("2. Best Fit\n");
        printf("3. Worst Fit\n");
        printf("4. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                firstFit(partitions, num_partitions, processes, num_processes);
                break;
            case 2:
                bestFit(partitions, num_partitions, processes, num_processes);
                break;
            case 3:
                worstFit(partitions, num_partitions, processes, num_processes);
                break;
            case 4:
                printf("Exiting.\n");
                exit(0);
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

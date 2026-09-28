#include <stdio.h>
#include <stdlib.h>

struct Process
{
    int allocation[10];
    int maximum[10];
    int need[10];
    int finish;
};

// Function to check if the system is in a safe state and print work vector updates
int checkSafeState(struct Process p[], int available[], int n, int m, int safeSequence[])
{
    int work[10];
    int count = 0;

    // Initialize work vector
    for (int j = 0; j < m; j++)
    {
        work[j] = available[j];
    }
    
    // Use a local finish array tracking to prevent corrupting structural data state
    int tempFinish[10] = {0};

    printf("\n--- Safety Algorithm Execution Steps ---");
    printf("\nInitial Work Vector: [ ");
    for (int j = 0; j < m; j++) printf("%d ", work[j]);
    printf("]\n");

    // Banker's Safety Algorithm
    while (count < n)
    {
        int found = 0;

        for (int i = 0; i < n; i++)
        {
            if (tempFinish[i] == 0)
            {
                int possible = 1;

                for (int j = 0; j < m; j++)
                {
                    if (p[i].need[j] > work[j])
                    {
                        possible = 0;
                        break;
                    }
                }

                if (possible)
                {
                    // Allocate allocated assets back and update tracking vector
                    for (int j = 0; j < m; j++)
                    {
                        work[j] += p[i].allocation[j];
                    }

                    safeSequence[count] = i;
                    tempFinish[i] = 1;
                    count++;
                    found = 1;

                    // Visual tracking of the changing work vector
                    printf("Process P%d executed. New Work Vector: [ ", i);
                    for (int j = 0; j < m; j++)
                    {
                        printf("%d ", work[j]);
                    }
                    printf("]\n");
                }
            }
        }

        if (found == 0)
        {
            break;
        }
    }
    printf("----------------------------------------\n");

    return (count == n); // Returns 1 if safe, 0 if unsafe
}

// Function to print matrices in a clean side-by-side table layout
void printSystemTable(struct Process p[], int n, int m)
{
    printf("\n================================= SYSTEM STATE TABLE =================================\n");
    
    // Table Header Setup
    printf("Process | ");
    printf("Allocation     | ");
    printf("Maximum        | ");
    printf("Need           \n");
    printf("--------+----------------+----------------+----------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("  P%d    | ", i);
        
        // Print Allocation
        for (int j = 0; j < m; j++) printf("%d ", p[i].allocation[j]);
        for (int s = 0; s < (15 - (m * 2)); s++) printf(" ");
        printf("| ");

        // Print Maximum
        for (int j = 0; j < m; j++) printf("%d ", p[i].maximum[j]);
        for (int s = 0; s < (15 - (m * 2)); s++) printf(" ");
        printf("| ");

        // Print Need
        for (int j = 0; j < m; j++) printf("%d ", p[i].need[j]);
        printf("\n");
    }
    printf("======================================================================================\n");
}

int main()
{
    int n, m;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    printf("Enter number of resources: ");
    scanf("%d", &m);

    struct Process p[10];
    int available[10];
    int totalInstances[10];
    int safeSequence[10];

    printf("\nEnter Allocation Matrix:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            scanf("%d", &p[i].allocation[j]);
        }
    }

    printf("\nEnter Maximum Matrix:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            scanf("%d", &p[i].maximum[j]);
        }
    }

    // Menu Option for Available Vector Setup
    int choice;
    printf("\nChoose option for Available Resources:\n");
    printf("1. Enter Available Matrix Manually\n");
    printf("2. Calculate Available from Total Resource Instances\n");
    printf("Enter choice (1 or 2): ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("\nEnter Available Resources:\n");
        for (int j = 0; j < m; j++)
        {
            scanf("%d", &available[j]);
        }
    }
    else if (choice == 2)
    {
        printf("\nEnter Total Instances for each Resource Type:\n");
        for (int j = 0; j < m; j++)
        {
            scanf("%d", &totalInstances[j]);
        }

        // Calculate Available = Total Instances - Sum(Allocated Instances)
        for (int j = 0; j < m; j++)
        {
            int totalAllocated = 0;
            for (int i = 0; i < n; i++)
            {
                totalAllocated += p[i].allocation[j];
            }
            available[j] = totalInstances[j] - totalAllocated;
        }
    }
    else
    {
        printf("Invalid choice! Exiting program.\n");
        return 0;
    }

    /* Calculate Need Matrix */
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            p[i].need[j] = p[i].maximum[j] - p[i].allocation[j];
        }
    }

    int menuOption;
    
    // Main Control Loop Structure
    do
    {
        printf("\n================================= MAIN MENU =================================\n");
        printf("1. View Current System Table & Available Resources\n");
        printf("2. Check If Current State is Safe\n");
        printf("3. Make a New Resource Request\n");
        printf("4. Exit Program\n");
        printf("Enter your choice: ");
        scanf("%d", &menuOption);

        switch (menuOption)
        {
            case 1:
                printSystemTable(p, n, m);
                printf("Current Available Resources: [ ");
                for (int j = 0; j < m; j++) printf("%d ", available[j]);
                printf("]\n");
                break;

            case 2:
                printf("\nChecking system safety status...");
                if (checkSafeState(p, available, n, m, safeSequence))
                {
                    printf("\nRESULT: System is in a SAFE STATE.");
                    printf("\nSafe Sequence: ");
                    for (int i = 0; i < n; i++)
                    {
                        printf("P%d", safeSequence[i]);
                        if (i != n - 1) printf(" -> ");
                    }
                    printf("\n");
                }
                else
                {
                    printf("\nRESULT: System is in an UNSAFE STATE!\n");
                }
                break;

            case 3:
                {
                    /* Resource Request Block */
                    int reqPID;
                    int request[10];

                    printf("\nEnter the process ID making a request (0 to %d): ", n - 1);
                    scanf("%d", &reqPID);

                    printf("Enter the request vector for P%d:\n", reqPID);
                    for (int j = 0; j < m; j++)
                    {
                        scanf("%d", &request[j]);
                    }

                    // Step 1: Check if Request <= Need
                    int exceedsNeed = 0;
                    for (int j = 0; j < m; j++)
                    {
                        if (request[j] > p[reqPID].need[j])
                        {
                            exceedsNeed = 1;
                            break;
                        }
                    }

                    if (exceedsNeed)
                    {
                        printf("\nREJECTED: Process P%d has exceeded its maximum claim (Need Matrix)!\n", reqPID);
                        break;
                    }

                    // Step 2: Check if Request <= Available
                    int exceedsAvailable = 0;
                    for (int j = 0; j < m; j++)
                    {
                        if (request[j] > available[j])
                        {
                            exceedsAvailable = 1;
                            break;
                        }
                    }

                    if (exceedsAvailable)
                    {
                        printf("\nREJECTED: Process P%d must wait. Resources are not available.\n", reqPID);
                        break;
                    }

                    // Step 3: Tentatively allocate resources
                    for (int j = 0; j < m; j++)
                    {
                        available[j] -= request[j];
                        p[reqPID].allocation[j] += request[j];
                        p[reqPID].need[j] -= request[j];
                    }

                    // Show matrix details during execution
                    printf("\nTentative System State for P%d's Request evaluation:", reqPID);
                    printSystemTable(p, n, m);

                    // Step 4: Run safety algorithm on tentative state
                    if (checkSafeState(p, available, n, m, safeSequence))
                    {
                        printf("\nSUCCESS: Request can be granted safely immediately!");
                        printf("\nNew Safe Sequence: ");
                        for (int i = 0; i < n; i++)
                        {
                            printf("P%d", safeSequence[i]);
                            if (i != n - 1) printf(" -> ");
                        }
                        printf("\n");
                    }
                    else
                    {
                        printf("\nREJECTED: Request causes an UNSAFE STATE. Rollback executed.\n");
                        
                        // Rollback structural metrics if state path proves hazardous
                        for (int j = 0; j < m; j++)
                        {
                            
                            available[j] += request[j];
                            p[reqPID].allocation[j] -= request[j];
                            p[reqPID].need[j] += request[j];
                        }
                    }
                }
                break;
            case 4:
                printf("\nExiting program. Goodbye!\n");
                break;
            default:
                printf("\nInvalid option selection! Please try again.\n");
            }
        } while (menuOption != 4);
        return 0;
}

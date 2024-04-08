#include <stdio.h>
#include <stdlib.h>

// Global variables
int numResources;
int numProcesses;
int *resource;
int *available;
int **maxClaim;
int **allocated;
int **need;

// Function prototypes
void enterClaimGraph();
void requestResource();
void releaseResource();
void determineSafeSequence();
void quitProgram();

void enterClaimGraph() {
    printf("Enter number of resources: ");
    scanf("%d", &numResources);

    // Allocate memory for resource array
    resource = (int *)malloc(numResources * sizeof(int));

    printf("Enter number of units for resources (r0 to r%d): ", numResources - 1);
    for (int i = 0; i < numResources; i++) {
        scanf("%d", &resource[i]);
    }

    printf("Enter number of processes: ");
    scanf("%d", &numProcesses);

    // Allocate memory for available, maxClaim, allocated, and need
    available = (int *)malloc(numResources * sizeof(int));
    maxClaim = (int **)malloc(numProcesses * sizeof(int *));
    allocated = (int **)malloc(numProcesses * sizeof(int *));
    need = (int **)malloc(numProcesses * sizeof(int *));

    for (int i = 0; i < numProcesses; i++) {
        maxClaim[i] = (int *)malloc(numResources * sizeof(int));
        allocated[i] = (int *)malloc(numResources * sizeof(int));
        need[i] = (int *)malloc(numResources * sizeof(int));
    }

    // Initialize available array with resource values
    for (int i = 0; i < numResources; i++) {
        available[i] = resource[i];
    }
}

void requestResource() {
    // Implement this function
}

void releaseResource() {
    // Implement this function
}

void determineSafeSequence() {
    // Implement this function
}

void quitProgram() {
    // Free dynamically allocated memory
    free(resource);
    free(available);
    for (int i = 0; i < numProcesses; i++) {
        free(maxClaim[i]);
        free(allocated[i]);
        free(need[i]);
    }
    free(maxClaim);
    free(allocated);
    free(need);

    printf("Quitting program...\n");
}



int main() {
    int choice;

    while (1) {
        printf("\nBanker's Algorithm\n");
        printf("------------------\n");
        printf("1) Enter claim graph\n");
        printf("2) Request resource\n");
        printf("3) Release resource\n");
        printf("4) Determine safe sequence\n");
        printf("5) Quit program\n\n");

        printf("Enter selection: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                enterClaimGraph();
                break;
            case 2:
                requestResource();
                break;
            case 3:
                releaseResource();
                break;
            case 4:
                determineSafeSequence();
                break;
            case 5:
                quitProgram();
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
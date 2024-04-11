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
void determineSafeSequence();
void releaseResource();
void quitProgram();
void printState();
void printState_ANN();
int isSafe();

void enterClaimGraph() {
    // Prompt user for number of resources
    do {
        printf("Enter number of resources: ");
        scanf("%d", &numResources);
        // Check if number of resources is valid
        if (numResources < 2 || numResources > 50) {
            numResources = 0;
            printf("Invalid number of resources. Please try again.\n");
        }

    } while (numResources < 2 || numResources > 50);

    // Allocate memory for resource array
    resource = (int *)malloc(numResources * sizeof(int));

    printf("Enter number of units for resources (r0 to r%d): ", numResources - 1);
    for (int i = 0; i < numResources; i++) {
        scanf("%d", &resource[i]);
    }

    // Prompt user for number of processes
    do {
        printf("Enter number of processes: ");
        scanf("%d", &numProcesses);
        // Check if number of processes is valid
        if (numProcesses < 1 || numProcesses > 50) {
            numProcesses = 0;
            printf("Invalid number of processes. Please try again.\n");
        }
    } while (numProcesses < 1 || numProcesses > 50);

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

    // Prompt user for maximum claim matrix
    for (int i = 0; i < numProcesses; i++) {
        printf("Enter maximum number of units process p%d will claim from each resource (r0 to r%d): ", i, numResources - 1);
        for (int j = 0; j < numResources; j++) {
            scanf("%d", &maxClaim[i][j]);
        }
    }
    // Prompt user for allocated matrix
    for (int i = 0; i < numProcesses; i++) {
        printf("Enter number of units of each resource (r0 to r%d) currently allocated to process p%d: ", numResources - 1, i);
        for (int j = 0; j < numResources; j++) {
            scanf("%d", &allocated[i][j]);
        }
    }

    // Update available array based on allocated resources
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            available[j] -= allocated[i][j];
        }
    }

    // Calculate need matrix
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            need[i][j] = maxClaim[i][j] - allocated[i][j];
        }
    }

    // Print state of claim graph
    printState();
}

void requestResource() {
    int process, resourceIndex, unitsRequested;
    char processName[10], resourceName[10];  // Adjusted size for larger inputs

    // Prompt user for the requesting process
    printf("Enter requesting process: ");
    scanf("%s", processName);
    process = atoi(&processName[1]);  // Extract the process number from the input

    // Prompt user for the requested resource
    printf("Enter requested resource: ");
    scanf("%s", resourceName);
    resourceIndex = atoi(&resourceName[1]);  // Extract the resource number from the input

    // Prompt user for the number of units requested
    printf("Enter number of units process %s is requesting from resource %s: ", processName, resourceName);
    scanf("%d", &unitsRequested);

    // Check if the request is valid
    if (unitsRequested > need[process][resourceIndex]) {
        printf("Error: Process %s is requesting more units than it needs.\n", processName);
        return;
    }

    if (unitsRequested <= available[resourceIndex]) {
        // Temporarily grant the request
        available[resourceIndex] -= unitsRequested;
        allocated[process][resourceIndex] += unitsRequested;
        need[process][resourceIndex] -= unitsRequested;

        // Check if the system is still in a safe state
        if (isSafe()) {
            // Request can be granted
            printf("Request granted.\n");
        } else {
            // Revert the temporary changes if the system is not safe
            available[resourceIndex] += unitsRequested;
            allocated[process][resourceIndex] -= unitsRequested;
            need[process][resourceIndex] += unitsRequested;
            printf("Error: Granting the request would lead to an unsafe state.\n");
        }
    } else {
        printf("Error: Not enough available resources.\n");
    }

    // Print the updated state of the system
    printState_ANN();
}


int isSafe() {
    int work[numResources];
    int finish[numProcesses];
    int safeSequence[numProcesses];
    int count = 0;

    // Initialize work array and finish array
    for (int i = 0; i < numResources; i++) {
        work[i] = available[i];
    }
    for (int i = 0; i < numProcesses; i++) {
        finish[i] = 0;
    }

    // Find a safe sequence
    while (count < numProcesses) {
        int found = 0;
        for (int i = 0; i < numProcesses; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < numResources; j++) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }
                if (j == numResources) {
                    for (int k = 0; k < numResources; k++) {
                        work[k] += allocated[i][k];
                    }
                    safeSequence[count++] = i;
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) {
            break;  // No safe sequence found
        }
    }

    // Check if all processes are finished
    if (count < numProcesses) {
        return 0;  // Not safe
    } else {
        // Print the safe sequence
        printf("Safe sequence: ");
        for (int i = 0; i < numProcesses; i++) {
            printf("p%d ", safeSequence[i]);
        }
        printf("\n");
        return 1;  // Safe
    }
}

void printState() {
    printf("\nResources: \n");
    printf("    ");
    for (int i = 0; i < numResources; i++) {
        printf("r%d      ", i);
    }
    printf("\n    ");
    for (int i = 0; i < numResources; i++) {
        printf("%d      ", resource[i]);
    }
    printf("\n\nAvailable: \n");
    printf("    ");
    for (int i = 0; i < numResources; i++) {
        printf("r%d      ", i);
    }
    printf("\n    ");
    for (int i = 0; i < numResources; i++) {
        printf("%d      ", available[i]);
    }
    printf("\n\nMax Claim: \n");
    printf("    ");
    for (int i = 0; i < numResources; i++) {
        printf("r%d      ", i);
    }
    printf("\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("p%d  ", i);
        for (int j = 0; j < numResources; j++) {
            printf("%d      ", maxClaim[i][j]);
        }
        printf("\n");
    }

    printf("\nAllocated: \n");
    printf("    ");
    for (int i = 0; i < numResources; i++) {
        printf("r%d      ", i);
    }
    printf("\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("p%d  ", i);
        for (int j = 0; j < numResources; j++) {
            printf("%d      ", allocated[i][j]);
        }
        printf("\n");
    }

    printf("\nNeed: \n");
    printf("    ");
    for (int i = 0; i < numResources; i++) {
        printf("r%d      ", i);
    }
    printf("\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("p%d  ", i);
        for (int j = 0; j < numResources; j++) {
            printf("%d      ", need[i][j]);
        }
        printf("\n");
    }
}

void printState_ANN() {
    printf("\nAvailable: \n");
    printf("    ");
    for (int i = 0; i < numResources; i++) {
        printf("r%d      ", i);
    }

    printf("\n    ");
    for (int i = 0; i < numResources; i++) {
        printf("%d      ", available[i]);
    }
    printf("\n");

        printf("\nAllocated: \n");
    printf("    ");
    for (int i = 0; i < numResources; i++) {
        printf("r%d      ", i);
    }
    printf("\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("p%d  ", i);
        for (int j = 0; j < numResources; j++) {
            printf("%d      ", allocated[i][j]);
        }
        printf("\n");
    }

    printf("\nNeed: \n");
    printf("    ");
    for (int i = 0; i < numResources; i++) {
        printf("r%d      ", i);
    }
    printf("\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("p%d  ", i);
        for (int j = 0; j < numResources; j++) {
            printf("%d      ", need[i][j]);
        }
        printf("\n");
    }
} 

void releaseResource() {
    int process, resourceIndex, unitsReleased;
    char processName[50], resourceName[50];  // Adjusted size for larger inputs

    // Prompt user for the releasing process
    printf("Enter releasing processor: ");
    scanf("%s", processName);
    process = atoi(&processName[1]);  // Extract the process number from the input

    // Prompt user for the released resource
    printf("Enter released resource: ");
    scanf("%s", resourceName);
    resourceIndex = atoi(&resourceName[1]);  // Extract the resource number from the input

    // Prompt user for the number of units released
    printf("Enter number of units process %s is releasing from resource %s: ", processName, resourceName);
    scanf("%d", &unitsReleased);

    // Check if the release is valid
    if (unitsReleased > allocated[process][resourceIndex]) {
        printf("Error: Process %s is releasing more units than it has allocated.\n", processName);
        return;
    }

    // Release the resource
    available[resourceIndex] += unitsReleased;
    allocated[process][resourceIndex] -= unitsReleased;
    need[process][resourceIndex] += unitsReleased;

    // Print the updated state of the system
    printState_ANN();

}

void determineSafeSequence() {
    int work[numResources];
    int finish[numProcesses];
    int safeSequence[numProcesses];
    int count = 0;

    // Initialize work array and finish array
    for (int i = 0; i < numResources; i++) {
        work[i] = available[i];
    }
    for (int i = 0; i < numProcesses; i++) {
        finish[i] = 0;
    }

    // Find a safe sequence
    while (count < numProcesses) {
        int found = 0;
        for (int i = 0; i < numProcesses; i++) {
            if (!finish[i]) {
                int j;
                printf("Comparing: < ");
                for (j = 0; j < numResources; j++) {
                    printf("%d ", need[i][j]);
                }
                printf("> <= < ");
                for (j = 0; j < numResources; j++) {
                    printf("%d ", work[j]);
                }
                printf("> : ");

                for (j = 0; j < numResources; j++) {
                    if (need[i][j] > work[j]) {
                        break;
                    }
                }
                if (j == numResources) {
                    for (int k = 0; k < numResources; k++) {
                        work[k] += allocated[i][k];
                    }
                    safeSequence[count++] = i;
                    finish[i] = 1;
                    found = 1;
                    printf("Process p%d can be sequenced\n", i);
                } else {
                    printf("Process p%d cannot be sequenced\n", i);
                }
            }
        }
        if (!found) {
            break;  // No safe sequence found
        }
    }

    // Check if all processes are finished
    if (count < numProcesses) {
        printf("No safe sequence found.\n");
    } else {
        // Print the safe sequence
        printf("Safe sequence of processes: ");
        for (int i = 0; i < numProcesses; i++) {
            printf("p%d ", safeSequence[i]);
        }
        printf("\n");
    }
}


void quitProgram() {
    // Free dynamically allocated memory
    printf("Freeing memory...\n");
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
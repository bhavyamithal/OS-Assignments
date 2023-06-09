#include <stdio.h>

#define MAX_PROCESS 10
#define MAX_RESOURCES 10

int n; // number of processes
int m; // number of resource types

int Available[MAX_RESOURCES]; // number of available resources of each type
int Max[MAX_PROCESS][MAX_RESOURCES]; // maximum number of instances of each resource that a process can request
int Allocation[MAX_PROCESS][MAX_RESOURCES]; // number of resources of each type currently allocated to each process
int Need[MAX_PROCESS][MAX_RESOURCES]; // remaining resource needs of each process

int Work[MAX_RESOURCES]; // working array for safety algorithm
int Finish[MAX_PROCESS]; // array to keep track of finished processes

int safeSequence[MAX_PROCESS]; // array to store the safe sequence

int isSafe() {
    int i, j, k;
    int count = 0;

    // initialize Work and Finish arrays
    for (i = 0; i < m; i++) {
        Work[i] = Available[i];
    }
    for (i = 0; i < n; i++) {
        Finish[i] = 0;
    }

    // find an index i such that both Finish[i] == 0 and Need[i] <= Work
    for (k = 0; k < n; k++) {
        for (i = 0; i < n; i++) {
            if (Finish[i] == 0) {
                int safe = 1;
                for (j = 0; j < m; j++) {
                    if (Need[i][j] > Work[j]) {
                        safe = 0;
                        break;
                    }
                }
                if (safe) {
                    for (j = 0; j < m; j++) {
                        Work[j] += Allocation[i][j];
                    }
                    Finish[i] = 1;
                    safeSequence[count++] = i;
                }
            }
        }
    }

    // check if all processes are finished
    for (i = 0; i < n; i++) {
        if (Finish[i] == 0) {
            return 0;
        }
    }

    return 1;
}

int requestResources(int process, int request[]) {
    int i;

    // check if request is valid
    for (i = 0; i < m; i++) {
        if (request[i] > Need[process][i] || request[i] > Available[i]) {
            return 0;
        }
    }

    // temporarily allocate resources to process and check if system is still safe
    for (i = 0; i < m; i++) {
        Available[i] -= request[i];
        Allocation[process][i] += request[i];
        Need[process][i] -= request[i];
    }
    int safe = isSafe();
    if (!safe) {
        // restore old resource-allocation state
        for (i = 0; i < m; i++) {
            Available[i] += request[i];
            Allocation[process][i] -= request[i];
            Need[process][i] += request[i];
        }
    }

    return safe;
}


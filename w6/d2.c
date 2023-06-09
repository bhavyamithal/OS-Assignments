#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_PROCESSES 100
#define MAX_RESOURCES 100

int available[MAX_RESOURCES];
int max[MAX_PROCESSES][MAX_RESOURCES];
int allocation[MAX_PROCESSES][MAX_RESOURCES];
int need[MAX_PROCESSES][MAX_RESOURCES];
bool finish[MAX_PROCESSES];

int num_processes, num_resources;

void print_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void print_matrix(int mat[][MAX_RESOURCES], int num_rows, int num_cols) {
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

bool is_safe() {
    int work[num_resources];
    for (int i = 0; i < num_resources; i++) {
        work[i] = available[i];
    }
    for (int i = 0; i < num_processes; i++) {
        finish[i] = false;
    }

    bool found;
    do {
        found = false;
        for (int i = 0; i < num_processes; i++) {
            if (!finish[i]) {
                bool can_be_allocated = true;
                for (int j = 0; j < num_resources; j++) {
                    if (need[i][j] > work[j]) {
                        can_be_allocated = false;
                        break;
                    }
                }
                if (can_be_allocated) {
                    for (int j = 0; j < num_resources; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    found = true;
                    break;
                }
            }
        }
    } while (found);

    for (int i = 0; i < num_processes; i++) {
        if (!finish[i]) {
            return false;
        }
    }
    return true;
}

bool is_request_grantable(int process_id, int *request) {
    for (int j = 0; j < num_resources; j++) {
        if (request[j] > need[process_id][j] || request[j] > available[j]) {
            return false;
        }
    }
    return true;
}

bool try_grant_request(int process_id, int *request) {
    if (!is_request_grantable(process_id, request)) {
        return false;
    }

    // Pretend to allocate the resources
    for (int j = 0; j < num_resources; j++) {
        available[j] -= request[j];
        allocation[process_id][j] += request[j];
        need[process_id][j] -= request[j];
    }

    if (is_safe()) {
        return true;
    } else {
        // Undo the pretended allocation
        for (int j = 0; j < num_resources; j++) {
            available[j] += request[j];
            allocation[process_id][j] -= request[j];
            need[process_id][j] += request[j];
        }
        return false;
    }
}

int* getSafeSequence(int num_processes, int num_resources, int* available,
                     int allocation[][MAX_RESOURCES], int need[][MAX_RESOURCES]) {
    int work[num_resources];
    for (int i = 0; i < num_resources; i++) {
        work[i] = available[i];
    }

    bool finish[num_processes];
    for (int i = 0; i < num_processes; i++) {
        finish[i] = false;
    }

    int* sequence = (int*) malloc(num_processes * sizeof(int));
    int num_finished = 0;
    while (num_finished < num_processes) {
        bool found = false;
        for (int i = 0; i < num_processes; i++) {
            if (!finish[i]) {
                bool can_be_allocated = true;
                for (int j = 0; j < num_resources; j++) {
                    if (need[i][j] > work[j]) {
                        can_be_allocated = false;
                        break;
                    }
                }
                if (can_be_allocated) {
                    for (int j = 0; j < num_resources; j++) {
                        work[j] += allocation[i][j];
                    }
                    finish[i] = true;
                    sequence[num_finished] = i;
                    num_finished++;
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            free(sequence);
            return NULL;  // no safe sequence exists
        }
    }
    return sequence;
}


int main() {
    int i, j;

    printf("Enter the number of processes: ");
    scanf("%d", &num_processes);

    printf("Enter the number of resources: ");
    scanf("%d", &num_resources);

    int available[num_resources];
    printf("Enter the number of available instances of each resource type:\n");
    for (i = 0; i < num_resources; i++) {
        scanf("%d", &available[i]);
    }

    int allocation[num_processes][num_resources], max[num_processes][num_resources], need[num_processes][num_resources];
    printf("Enter the maximum demand of each process for each resource type:\n");
    for (i = 0; i < num_processes; i++) {
        for (j = 0; j < num_resources; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    printf("Enter the number of instances of each resource type currently allocated to each process:\n");
    for (i = 0; i < num_processes; i++) {
        for (j = 0; j < num_resources; j++) {
            scanf("%d", &allocation[i][j]);
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    if (is_safe(num_processes, num_resources, available, allocation, max)) {
        printf("Safe sequence: ");
        int* sequence = getSafeSequence(num_processes, num_resources, available, allocation, need);
        for (i = 0; i < num_processes; i++) {
            printf("%d ", sequence[i]);
        }
        free(sequence);
    } else {
        printf("Unsafe state, no safe sequence found.");
    }

    return 0;
}

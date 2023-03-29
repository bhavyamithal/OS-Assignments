#include<stdio.h>
#include<stdlib.h>

struct process {
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int rt;
    int completed;
};

// Function to find the process with the highest response ratio
int find_highest_response_ratio_process(struct process *p, int n, int current_time) {
    int highest_ratio_process = -1;
    float highest_ratio = -1;
    for (int i = 0; i < n; i++) {
        if (!p[i].completed) {
            float ratio = (float)(p[i].wt + p[i].bt) / p[i].bt;
            if (p[i].at <= current_time && ratio > highest_ratio) {
                highest_ratio = ratio;
                highest_ratio_process = i;
            }
        }
    }
    return highest_ratio_process;
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct process p[n];
    for (int i = 0; i < n; i++) {
        printf("Enter arrival time and burst time for process %d: ", i+1);
        scanf("%d %d", &p[i].at, &p[i].bt);
        p[i].pid = i+1;
        p[i].completed = 0;
        p[i].rt = p[i].bt;
    }

    int current_time = 0;
    int completed = 0;
    while (completed != n) {
        int highest_ratio_process = find_highest_response_ratio_process(p, n, current_time);
        if (highest_ratio_process == -1) {
            current_time++;
            continue;
        }
        p[highest_ratio_process].rt--;
        if (p[highest_ratio_process].rt == 0) {
            p[highest_ratio_process].completed = 1;
            completed++;
            p[highest_ratio_process].ct = current_time + 1;
            p[highest_ratio_process].tat = p[highest_ratio_process].ct - p[highest_ratio_process].at;
            p[highest_ratio_process].wt = p[highest_ratio_process].tat - p[highest_ratio_process].bt;
        }
        current_time++;
    }

    float avg_tat = 0, avg_wt = 0;
    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\n", p[i].pid, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
        avg_tat += p[i].tat;
        avg_wt += p[i].wt;
    }
    avg_tat /= n;
    avg_wt /= n;
    printf("\nAverage Turn Around Time: %0.2f", avg_tat);
    printf("\nAverage Waiting Time: %0.2f\n", avg_wt);
    return 0;
}

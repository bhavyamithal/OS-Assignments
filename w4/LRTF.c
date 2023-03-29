#include <stdio.h>

// process structure
struct process {
    int pid; // process id
    int at; // arrival time
    int bt; // burst time
    int rt; // remaining time
};

// function to find the process with the longest remaining time
int findLRT(struct process proc[], int n, int time) {
    int max = -1;
    int index = -1;
    for (int i = 0; i < n; i++) {
        if (proc[i].at <= time && proc[i].rt > max) {
            max = proc[i].rt;
            index = i;
        }
    }
    return index;
}

// function to perform LRTF scheduling
void LRTF(struct process proc[], int n) {
    int time = 0;
    int completed = 0;
    int wt_total = 0, tat_total = 0;

    // loop until all processes are completed
    while (completed < n) {
        int i = findLRT(proc, n, time);
        if (i == -1) {
            time++;
            continue;
        }
        proc[i].rt--;

        if (proc[i].rt == 0) {
            completed++;

            // calculate completion time, turnaround time and waiting time
            int ct = time + 1;
            int tat = ct - proc[i].at;
            int wt = tat - proc[i].bt;
            tat_total += tat;
            wt_total += wt;

            // print process details
            printf("P%d\t%d\t%d\t%d\t%d\t%d\n", proc[i].pid, proc[i].at, proc[i].bt, ct, tat, wt);
        }

        time++;
    }

    // print average waiting time and turnaround time
    float awt = (float) wt_total / n;
    float atat = (float) tat_total / n;
    printf("Average Waiting Time: %.2f\n", awt);
    printf("Average Turnaround Time: %.2f\n", atat);
}

int main() {
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct process proc[n];
    printf("Enter the arrival time and burst time for each process:\n");
    for (int i = 0; i < n; i++) {
        proc[i].pid = i + 1;
        printf("Process %d: ", i + 1);
        scanf("%d %d", &proc[i].at, &proc[i].bt);
        proc[i].rt = proc[i].bt;
    }

    // printf("Gantt Chart:\n");
    printf("PID\tAT\tBT\tWT\tTAT\tCT\n");

    LRTF(proc, n);

    return 0;
}

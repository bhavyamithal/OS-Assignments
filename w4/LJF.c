#include<stdio.h>

struct process{
    int pid; // process ID
    int burst_time; // burst time of process
    int arrival_time; // arrival time of process
};

// function to sort processes in descending order of burst time
void sort_processes(struct process p[], int n){
    for(int i=0; i<n-1; i++){
        for(int j=0; j<n-i-1; j++){
            if(p[j].burst_time < p[j+1].burst_time){
                // swap the processes
                struct process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }
}

// function to implement Longest Job First scheduling algorithm
void LJF(struct process p[], int n){
    // sort the processes in descending order of burst time
    sort_processes(p, n);

    int waiting_time[n], turnaround_time[n], completion_time[n];

    // first process starts at its arrival time
    completion_time[0] = p[0].burst_time + p[0].arrival_time;
    turnaround_time[0] = completion_time[0] - p[0].arrival_time;
    waiting_time[0] = turnaround_time[0] - p[0].burst_time;

    // calculate completion time, waiting time, and turnaround time for each process
    for(int i=1; i<n; i++){
        completion_time[i] = completion_time[i-1] + p[i].burst_time;
        turnaround_time[i] = completion_time[i] - p[i].arrival_time;
        waiting_time[i] = turnaround_time[i] - p[i].burst_time;
    }

    // print the results
    printf("PID\tBurst Time\tArrival Time\tWaiting Time\tTurnaround Time\tCompletion Time\n");
    for(int i=0; i<n; i++){
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", p[i].pid, p[i].burst_time, p[i].arrival_time, waiting_time[i], turnaround_time[i], completion_time[i]);
    }

    // calculate and print average waiting time and average turnaround time
    float avg_waiting_time = 0, avg_turnaround_time = 0;
    for(int i=0; i<n; i++){
        avg_waiting_time += waiting_time[i];
        avg_turnaround_time += turnaround_time[i];
    }
    avg_waiting_time /= n;
    avg_turnaround_time /= n;

    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
}

int main(){
    int n;
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    struct process p[n];

    // get input for process details
    for(int i=0; i<n; i++){
        printf("Enter the details for process %d:\n", i+1);
        p[i].pid = i+1;
        printf("Burst time: ");
        scanf("%d", &p[i].burst_time);
        printf("Arrival time: ");
        scanf("%d", &p[i].arrival_time);
    }

    // implement Longest Job First scheduling algorithm
    LJF(p, n);

    return 0;
}

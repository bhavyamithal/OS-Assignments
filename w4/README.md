CPU Scheduling Algorithms

This repository contains C programs for three different CPU scheduling algorithms - Longest Job First (LJF), Longest Remaining Time First (LRTF), and Highest Response Ratio Next (HRRN).

Longest Job First (LJF)

Characteristics of Longest Job First (LJF)
It is a non-preemptive scheduling algorithm
It schedules the process with the largest burst time
The tie is broken using FCFS

Advantages of Longest Job First (LJF)
It minimizes the average waiting time for the processes
It gives better performance for processes with longer burst time

Disadvantages of Longest Job First (LJF)
It may cause the problem of starvation for short processes
It may result in a longer turnaround time for short processes

Sample Input
Number of processes: 4
Process ID    Arrival Time    Burst Time
P1              0              8
P2              1              4
P3              2              9
P4              3              5

Sample Output
Process     Waiting Time     Turnaround Time
  P1             0                8
  P2             8               12
  P3            12               21
  P4            21               26

Average Waiting Time = 10.25
Average Turnaround Time = 16.75


Longest Remaining Time First (LRTF)

Characteristics of Longest Remaining Time First (LRTF)
It is a preemptive scheduling algorithm
It schedules the process with the largest remaining burst time
If two processes have the same remaining burst time, then tie is broken using FCFS
It can be of both preemptive and non-preemptive type

Advantages of Longest Remaining Time First (LRTF)
No other process can execute until the longest job or process executes completely
All the jobs or processes finish at approximately the same time

Disadvantages of Longest Remaining Time First (LRTF)
It may give a very high average waiting time and average turnaround time for a given set of processes
It may lead to the convoy effect
A short process may never get executed and the system keeps on executing the longer processes
It reduces the processing speed and thus reduces the efficiency and utilization of the system

Sample Input
Number of processes: 4
Process ID    Arrival Time    Burst Time
P1              0              8
P2              1              4
P3              2              9
P4              3              5


Sample Output
Process     Waiting Time     Turnaround Time
  P1             7               15
  P2             0                4
  P3            10               19
  P4            15               20

Average Waiting Time = 8.00
Average Turnaround Time = 14.50


Highest Response Ratio Next (HRRN) 
Overview
HRRN is a non-preemptive CPU scheduling algorithm that selects the process with the highest response ratio as the next process to run. Response ratio is calculated as the sum of waiting time and burst time divided by the burst time. This algorithm is considered optimal as it tries to minimize the average waiting time and turnaround time of the processes.

Characteristics
Non-preemptive CPU Scheduling Algorithm
Highest response ratio is the selection criteria
Response ratio is calculated as (waiting time + burst time) / burst time
Modification of Shortest Job First to reduce the problem of starvation

Advantages
Generally gives better performance than the Shortest Job First scheduling algorithm
Reduction in waiting time for longer jobs and encourages shorter jobs

Disadvantages
Not possible to implement in real-world scenarios as it's not possible to know the burst time of every job in advance
Possibility of overloading on the CPU

Algorithm
Sort the processes in ascending order of arrival time.
Initialize time and completed processes count to 0.
Initialize a list to hold the waiting times of the processes.
Repeat until all processes are completed:
Calculate the response ratio of each process using the formula (waiting time + burst time) / burst time.
Select the process with the highest response ratio.
Update its waiting time as time - arrival time - burst time.
Increment the completed processes count.
Add its waiting time to the list of waiting times.
Increment the time by the process's burst time.

Input Format
The input is taken from the user and it should contain the arrival time and burst time of each process in the following format:
<process_id> <arrival_time> <burst_time>

Output Format
The output of the algorithm includes the waiting time, turnaround time, and average waiting time and turnaround time of the processes.

Sample Input
1 0 7
2 2 4
3 4 1
4 5 4

Sample Output
Process   Waiting Time   Turnaround Time
P1        0              7
P2        5              9
P3        1              2
P4        4              8

Average Waiting Time:    2.50
Average Turnaround Time: 6.50
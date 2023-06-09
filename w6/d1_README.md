In the main function, we first prompt the user to enter 
the number of processes and resources. Then, we ask for 
the available resources, maximum demand of each process, 
and resources allocated to each process. 
After that, we calculate the need matrix using the 
formula need[i][j] = max[i][j] - allocation[i][j]. 
Finally, we call the bankerAlgorithm function and print 
the safe sequence if it exists, or an error message otherwise.
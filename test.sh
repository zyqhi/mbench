msg_sz=16384
sample_time=5

mpicc -g mbench.c -o mbench
mpiexec -n 8 -f machinefile ./mbench $msg_sz $sample_time

proc_num=16
msg_sz=`expr 1024 \* 60`
sample_time=1

mpicc  mbench.c -o mbench
mpiexec -n $proc_num -f machinefile ./mbench $msg_sz $sample_time

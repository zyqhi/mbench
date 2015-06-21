#!/bin/bash

benchmark=./mbench
machinefile=./machinefile
proc_num=16
msg_sz_array=(2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 64512)
sample_time=2

if [[ $1 = "mpich3" ]]; then
    echo "mpich3"
    export PATH=/usr/lib64/qt-3.3/bin:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/opt/ibutils/bin:/root/zyq/mpich3/mpich3-install/bin:/root/bin

     for msg_sz in ${msg_sz_array[@]}; do
        echo "--------"  $msg_sz "--------" 
        mpicc mbench.c -o mbench
        mpiexec -n $proc_num -f $machinefile $benchmark $msg_sz $sample_time
        sleep 5
    done
elif [[ $1 = "mpich3-fsom" ]]; then
    echo "mpich3-fsom"
    export PATH=/usr/lib64/qt-3.3/bin:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/opt/ibutils/bin:/root/zyq/mpich3-fsom/mpich3-fsom-install/bin:/root/bin

     for msg_sz in ${msg_sz_array[@]}; do
        echo "--------" $msg_sz "--------" 
        mpicc mbench.c -o mbench
        mpiexec -n $proc_num -f $machinefile $benchmark $msg_sz $sample_time
        sleep 5
    done
fi

# mpicc  mbench.c -o mbench
# mpiexec -n $proc_num -f machinefile ./mbench $msg_sz $sample_time

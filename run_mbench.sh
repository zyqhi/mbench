proc_num=16
# msg_sz=`expr 1024 \* 64`
msg_sz=$2
sample_time=2

if [[ $1 = "mpich3" ]]; then
    export PATH=/usr/lib64/qt-3.3/bin:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/opt/ibutils/bin:/root/zyq/mpich3/mpich3-install/bin:/root/bin
elif [[ $1 = "mpich3-fsom" ]]; then
    export PATH=/usr/lib64/qt-3.3/bin:/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/opt/ibutils/bin:/root/zyq/mpich3-fsom/mpich3-fsom-install/bin:/root/bin
fi

mpicc  mbench.c -o mbench
mpiexec -n $proc_num -f machinefile ./mbench $msg_sz $sample_time

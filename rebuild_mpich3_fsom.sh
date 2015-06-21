#!/bin/bash
src_dir="/root/zyq/mpich3-fsom/src/fsom-bcast"
bench_dir="/root/zyq/test/mbench"

cd $src_dir
./build.sh
cd $bench_dir

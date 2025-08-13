#!/bin/bash
#usage: ./batch_complie.sh [lmbench case name]  
#Ex. ./batch_complie.sh bw_rd
data_size_list="8*KB 16*KB 32*KB 64*KB 128*KB 256*KB 512*KB 1*MB 2*MB 3*MB 4*MB 6*MB 8*MB 16*MB 32*MB 64*MB"
page_size="1G"
stride="4096"

echo "execute $1"

for data_size in $data_size_list;do
  make TEST=lmbench_$1  PAGE=$page_size SIZE=$data_size STRIDE=$stride WARMLOOP=4 BENCHLOOP=10
done

#large data size, benchloop=2
make TEST=lmbench_$1  PAGE=$page_size SIZE=256*MB STRIDE=$stride WARMLOOP=4 BENCHLOOP=2
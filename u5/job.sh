#!/bin/sh
#PBS -q wr8
#PBS -l nodes=1:ppn=48
#PBS -l walltime=00:30:00
#PBS -l vmem=16GB

# change to job submit directory
cd $PBS_O_WORKDIR

module load intel-icc

./sched_test.exe

exit

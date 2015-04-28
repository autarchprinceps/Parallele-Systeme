#!/bin/sh
#PBS -q wr5
#PBS -l nodes=1:ppn=56
#PBS -l walltime=00:30:00
#PBS -l vmem=16GB

# change to job submit directory
cd $PBS_O_WORKDIR

module load intel-icc

./u2a3.exe

exit

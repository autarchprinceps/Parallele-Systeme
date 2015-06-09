#!/bin/sh
#PBS -q wr5
#PBS -l nodes=1:ppn=56
#PBS -l walltime=01:00:00
#PBS -l vmem=60GB

# change to job submit directory
cd $PBS_O_WORKDIR

module load intel-icc

make run

exit

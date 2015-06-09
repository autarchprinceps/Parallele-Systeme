#!/bin/sh
#PBS -q wr5
#PBS -l nodes=1:ppn=56
#PBS -l walltime=02:00:00
#PBS -l vmem=120GB

# change to job submit directory
cd $PBS_O_WORKDIR

module load intel-icc

make run

exit

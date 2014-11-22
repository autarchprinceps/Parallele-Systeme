#!/bin/sh
#PBS -q mpi
#PBS -l nodes=1:ppn=16
#PBS -l walltime=00:05:00

# change to job submit directory
cd $PBS_O_WORKDIR

module load acml-sp

./matmul_c.exe

exit

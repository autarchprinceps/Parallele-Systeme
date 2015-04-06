#!/bin/sh
#PBS -q mpi
#PBS -l nodes=2:ppn=16
#PBS -l walltime=00:30:00
#PBS -l vmem=16GB
#PBS -N pp

# change to job submit directory
cd $PBS_O_WORKDIR

# load openmpi module
module load intel-icc
module load openmpi/intel

# determine number of available processors
NNODES=`cat $PBS_NODEFILE | wc -l`

mpirun -np $NNODES -machinefile $PBS_NODEFILE ./ping_pong.exe

exit

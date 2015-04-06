#!/bin/sh
#PBS -q mpi
#PBS -l nodes=2:ppn=16
#PBS -l walltime=00:30:00
#PBS -l vmem=1GB
#PBS -N pp

# change to job submit directory
cd $PBS_O_WORKDIR

# load openmpi module
module load openmpi/gnu

# determine number of available processors
NNODES=`cat $PBS_NODEFILE | wc -l`

mpirun -np $NNODES -machinefile $PBS_NODEFILE ./pp.exe

exit
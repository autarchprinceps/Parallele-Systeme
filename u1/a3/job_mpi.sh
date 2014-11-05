#!/bin/sh
#PBS -q mpi
#PBS -l nodes=2:ppn=16
#PBS -l walltime=00:10:00
#PBS -l vmem=1GB

# change to job submit directory
cd $PBS_O_WORKDIR

module load openmpi/gnu

# determine number of available processors
NNODES=`cat $PBS_NODEFILE | wc -l`

# start here your MPI program (example here: mpi.exe)
mpirun -np $NNODES -machinefile $PBS_NODEFILE ./ring.exe

exit

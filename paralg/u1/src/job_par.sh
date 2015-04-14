#!/bin/sh
#PBS -q mpi
#PBS -l nodes=10:ppn=16
#PBS -l walltime=06:00:00
#PBS -l vmem=16GB
#PBS -N floyd_par

# change to job submit directory
cd $PBS_O_WORKDIR

# load openmpi module
module load openmpi/gnu


# matrix dimension
export N=6000


# generate different number of processors
for NNODES in  1 2 3 4 5 6 8 10 ; do 

   echo "running with " $NNODES " nodes"

# we cheat the job system / MPI
# machinefile is a given file in the local directory
# we run the program 3 times
    mpirun -np $NNODES -machinefile machinefile floyd_mpi.exe $N
    mpirun -np $NNODES -machinefile machinefile floyd_mpi.exe $N
    mpirun -np $NNODES -machinefile machinefile floyd_mpi.exe $N

done

exit

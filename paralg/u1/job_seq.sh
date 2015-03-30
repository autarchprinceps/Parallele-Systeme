#!/bin/sh
#PBS -q mpi
#PBS -l nodes=1:ppn=16
#PBS -l walltime=06:00:00
#PBS -l vmem=16GB
#PBS -N floyd_seq

# change to job submit directory
cd $PBS_O_WORKDIR

# matrix dimension
export N=6000


# run the sequential program 3 times.
echo "running sequentially (3 times)"
./floyd_seq.exe $N
./floyd_seq.exe $N
./floyd_seq.exe $N

exit

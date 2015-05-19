#!/bin/sh
#PBS -q mpi
#PBS -l nodes=8:ppn=16
#PBS -l walltime=01:00:00
#PBS -l vmem=16GB

# change to job submit directory
cd $PBS_O_WORKDIR

module load openmpi/gnu

# determine number of available processors

for i in `seq 0 7`
do
	echo $(awk "BEGIN{print 2 ** $i}")
	mpirun -np $(awk "BEGIN{print 2 ** $i}") -machinefile $PBS_NODEFILE ./sort.exe
done

exit

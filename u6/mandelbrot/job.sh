#!/bin/sh
#PBS -q mpi
#PBS -l nodes=2:ppn=16
#PBS -l walltime=01:00:00
#PBS -l vmem=16GB

# change to job submit directory
cd $PBS_O_WORKDIR

module load intel-icc
module load openmpi/intel
module load metis/5.1.0-icc-64


# determine number of MPI processes we have from the machinefile
#NNODES=`cat $PBS_NODEFILE | wc -l`

nprocs="2 4 8 16 32"
#nprocs="8"

# loop over all combinations of processor numbers
for nd in $nprocs; do
    NNODES=$nd

    # start here your program, for example an MPI program
    echo "running on " $NNODES " processors"
    mpirun -np $NNODES -machinefile $PBS_NODEFILE mandelbrot.exe
done

exit

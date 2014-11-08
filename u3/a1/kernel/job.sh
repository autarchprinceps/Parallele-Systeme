#!/bin/sh
##############################################################
#PBS -q default
#PBS -l nodes=1:ppn=2
#PBS -l walltime=23:59:00
#PBS -l vmem=8gb
##############################################################
#
# generate a data memory reference trace for a program,
# run the dinero cache simulator with this memory reference trace
# trace data is send through a socket
#
##############################################################
# log everything
set -x

module load intel-icc

# log arguments submitted to script
echo "Running with PORT=$PORT,PROGRAM=$PROGRAM,PROGRAM_ARGS=$PROGRAM_ARGS,DIR=$DIR,FILE=$FILE,DINERO_ARGS=$DINERO_ARGS" >&2

# change to job submit directory
cd $PBS_O_WORKDIR
# create output directory for this job (if it doesn't exist) and change to it
mkdir -p output/$DIR
cd output/$DIR

#!!!!!!! CHANGE port !!!!!!!!
# free socket port to use for communication between server and client
export PIN_ON_SOCKET=$PORT
#e.g. 14423

#!!!!!!! CHANGE program name !!!!!!!!
# program to instrument
#export PROGRAM="./bubblesort.exe"
#!!!!!!! CHANGE program arguments !!!!!!!!
#export PROGRAM_ARGS="30000"

# increase stack size for some programs
ulimit -s 10000000

##############################################################

# path extensions for trace generator
export PATH=/usr/local/pin:/usr/local/pin_appl:$PATH
export LD_LIBRARY_PATH=/usr/local/pin/source/tools/MyPinTool/obj-intel64:$LD_LIBRARY_PATH

##############################################################
# run server and client

# note: valgrind has also a (less accurate) memory tracer: "valgrind --tool=lackey --trace-mem=yes" with an appropriate filter
# start trace generator with application to instrument (including program arguments)
pin -t memtrace -- $PBS_O_WORKDIR/$PROGRAM $PROGRAM_ARGS &

# otherwise the server isn't ready to accept client requests on port
sleep 10

#!!!!!!! CHANGE dinero parameters !!!!!!!!
# memtrace client with cache simulation (change dinero parameters to your needs)
time memtrace_client.exe localhost $PIN_ON_SOCKET | \
	 dineroIV $DINERO_ARGS > $FILE

##############################################################

exit

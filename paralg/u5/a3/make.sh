#!/bin/bash
gcc -std=c99 -I/usr/local/libFHBRS/ -L/usr/local/libFHBRS/ -Wall -openmp -fopenmp main.c -lFHBRS -lm -o main.exe

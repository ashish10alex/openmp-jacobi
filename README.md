# openmp-jacobi
A parallel programming implementation in C using OpenMP

## Introduction
Jacobi iterative method has been used to compute temperature distribution for a rectangular 2D problem. The boundary condition is set and temperature distribution has been calculated for a range of problem sizes with tolerance kept at 0.0001.
The main aim of this project is concentrated at computing the speedup achieved while comparing the serial code provided with the contrived parallel version of the code.
Speed up is defined as ratio of the serial runtime obtained under the best optimization and algorithmic conditions to the time taken by the parallel version of the code to solve the same problem with certain number of processors.    


## Serial code execution

__gcc jacobi_serial.c -o serial.out__

__./serial.out  <matrix size> <matrix size>  <tolerance>__

[serail implementation]: (https://github.com/ashish10alex/openmp-jacobi/tree/master/images/Serial_graph.png)

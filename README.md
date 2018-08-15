# openmp-jacobi
A parallel programming implementation in C using OpenMP

## Introduction
Jacobi iterative method has been used to compute temperature distribution for a rectangular 2D problem. The boundary condition is set and temperature distribution has been calculated for a range of problem sizes with tolerance kept at 0.0001.
The main aim of this project is concentrated at computing the speedup achieved while comparing the serial code provided with the contrived parallel version of the code.
Speed up is defined as ratio of the serial runtime obtained under the best optimization and algorithmic conditions to the time taken by the parallel version of the code to solve the same problem with certain number of processors.    


## Serial code execution

`gcc jacobi_serial.c -o serial.out`

`./serial.out  'matrix size' 'matrix size'  'tolerance'` <br>

`./serial.out 200 200 0.0001`

![serial Implementation](https://github.com/ashish10alex/openmp-jacobi/blob/master/images/Serial_graph.PNG)


### Parallel code validation 

The above code prints out matrices obtained from both serial and parallel implementations with the execution times for each. Validation has been performed at this stage for various problem sizes and 1,2,3,4 threads. The same script substracts the resultant parallel output from serial output and gives the values for the same as shown in figure below.

`gcc -fopenmp jacobi_parallel_validation.c -o test.out`

`./test.out <matrix size> <matrix size> <tolerance> < threads > `<br>

`./test.out 10 10 0.0001 2`

![Parellel code validation](https://github.com/ashish10alex/openmp-jacobi/blob/master/images/parallel_validation.PNG)


## Progressive Speedup
Progressive speedup can be visualized by running the following

`gcc -fopenmp Pspeed.c -o speed.out`
`./speed.out <matrix size> <matrix size> <tolerance>` 

![Progressive speedup](https://github.com/ashish10alex/openmp-jacobi/blob/master/images/Pspeed.PNG)

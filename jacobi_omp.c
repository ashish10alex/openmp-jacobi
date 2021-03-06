#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{

	int m;
	int n;
	double tol; // = 0.0001;

	double tstart, tstop;

	int i, j, iter;

	int nthreads; // Initilise  number of threads

	double stop[20];
	int count = 0; //Counter for tstop array
	int y = 4;	   //THreads comparision counter.
	int u;
	m = atoi(argv[1]);
	n = atoi(argv[2]);
	tol = atof(argv[3]);
	for (u = 0; u < 5; u++)
	{ // Iterations for o/p
		for (nthreads = 2; nthreads <= 10; nthreads += 2)
		{ // Loop for thread numbers
			if (nthreads == 10)
			{
				nthreads = 16;
			}
			omp_set_num_threads(nthreads);

			double t[m + 2][n + 2], tnew[m + 1][n + 1], diff, difmax;

			tstart = omp_get_wtime(); //Start of timer for parallel

			// initialise temperature array
#pragma omp parallel default(shared) private(i, j)

			{ // Start of  parellel region 1

#pragma omp for schedule(auto)
				for (i = 0; i <= m + 1; i++)
				{
					for (j = 0; j <= n + 1; j++)
					{
						t[i][j] = 30.0;
					}
				}
				//#pragma omp barrier
// fix boundary conditions
#pragma omp for schedule(auto)
				for (i = 1; i <= m; i++)
				{
					t[i][0] = 10.0;
					t[i][n + 1] = 140.0;
				}

#pragma omp for schedule(auto)
				for (j = 1; j <= n; j++)
				{
					t[0][j] = 20.0;
					t[m + 1][j] = 100.0;
				}

			} // end parallel region 1

			// main loop
#pragma omp parallel default(shared) private(i, j, diff) // start of parallel region 2
			{
				iter = 0;
				difmax = 1000000.0;
				while (difmax > tol)
				{ // Beginning of loop
					iter++;

					// update temperature for next iteration
#pragma omp for schedule(auto)
					for (i = 1; i <= m; i++)
					{
						for (j = 1; j <= n; j++)
						{
							tnew[i][j] = (t[i - 1][j] + t[i + 1][j] + t[i][j - 1] + t[i][j + 1]) / 4.0;
						}
					}

					difmax = 0.0;

					// #pragma omp barrier

#pragma omp for schedule(auto)
					for (i = 1; i <= m; i++)
					{
						for (j = 1; j <= n; j++)
						{
							diff = fabs(tnew[i][j] - t[i][j]); // Absolute value calculation.

							if (diff > difmax)
							{
#pragma omp critical
								difmax = diff;
							}
							// copy new to old temperatures

							t[i][j] = tnew[i][j];
						}
					}

				} //End of  while loop

			} // End of parellel region 2

			tstop = (omp_get_wtime() - tstart); //End of timer for parallel

			stop[count] = tstop * 1000000; // Time required for each thread has been stored in array elements.
			count++;

			printf("Time: %4.3lf for %d Threads\n", tstop * 1000000, nthreads);

		} // End of thread loops
		printf("\n");

	} // Iterations for o/p's End
}

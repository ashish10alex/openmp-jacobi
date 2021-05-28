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

	m = atoi(argv[1]);
	n = atoi(argv[2]);
	tol = atof(argv[3]);
	nthreads = atoi(argv[4]); // Put number of threads

	double t[m + 2][n + 2], tnew[m + 1][n + 1], diff, difmax; // parallel Array
	double s[m + 2][n + 2], snew[m + 1][n + 1];				  // Serial Array
	double sub[m + 2][m + 2];								  // (Serial-Parallel)o/p will be shown here

	printf("%d %d %lf\n", m, n, tol);

	tstart = omp_get_wtime(); //Start of timer for parallel

	omp_set_num_threads(nthreads);
	// initialise temperature array
#pragma omp parallel default(shared) private(i, j)

	{ // Start of  parellel region

#pragma omp for schedule(static) collapse(2)
		for (i = 0; i <= m + 1; i++)
		{
			for (j = 0; j <= n + 1; j++)
			{
				t[i][j] = 30.0;
			}
		}

#pragma omp for nowait schedule(static) collapse(2)
		for (i = 1; i <= m; i++)
			for (j = 1; j <= n; j++)
			{
				t[i][0] = 10.0;
				t[i][n + 1] = 140.0;
				t[0][j] = 20.0;
				t[m + 1][j] = 100.0;
			}
	}

#pragma omp parallel default(shared) private(i, j, diff)
	{
		// main loop
		iter = 0;
		difmax = 1000000.0;
		while (difmax > tol)
		{ // Beginning of loop

			iter++;
			// update temperature for next iteration
#pragma omp for schedule(static) collapse(2)
			for (i = 1; i <= m; i++)
			{
				for (j = 1; j <= n; j++)
				{
					tnew[i][j] = (t[i - 1][j] + t[i + 1][j] + t[i][j - 1] + t[i][j + 1]) / 4.0;
				}
			}

			// work out maximum difference between old and new temperatures

			difmax = 0.0;

#pragma omp for schedule(static) collapse(2)
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

			//End of loop
		}
	} // End of parellel region.

	tstop = (omp_get_wtime() - tstart); //End of timer for parallel

	// print results
	printf("\n");
	printf("\n############## PARALLEL SECTION ############");

	printf("\niter = %d  difmax = %9.11lf", iter, difmax);
	for (i = 0; i <= m + 1; i++)
	{
		printf("\n");
		for (j = 0; j <= n + 1; j++)
		{
			printf("%3.5lf ", t[i][j]);
		}
	}
	printf("\n");

	printf("\ntime taken is %4.3lf uSeconds", tstop * 1000000);

	// Begining of SERIAL section.

	printf("\n############## SERIAL SECTION ############\n");

	double tstart1 = omp_get_wtime(); // Begining for timer for SERIAL

	printf("\n%d %d %lf\n", m, n, tol);

	// initialise temperature array
	for (i = 0; i <= m + 1; i++)
	{
		for (j = 0; j <= n + 1; j++)
		{
			s[i][j] = 30.0;
		}
	}

	// fix boundary conditions
	for (i = 1; i <= m; i++)
	{
		s[i][0] = 10.0;
		s[i][n + 1] = 140.0;
	}
	for (j = 1; j <= n; j++)
	{
		s[0][j] = 20.0;
		s[m + 1][j] = 100.0;
	}

	// main loop
	iter = 0;
	difmax = 1000000.0;
	while (difmax > tol)
	{
		iter++;

		// update temperature for next iteration
		for (i = 1; i <= m; i++)
		{
			for (j = 1; j <= n; j++)
			{
				snew[i][j] = (s[i - 1][j] + s[i + 1][j] + s[i][j - 1] + s[i][j + 1]) / 4.0;
			}
		}

		// work out maximum difference between old and new temperatures
		difmax = 0.0;
		for (i = 1; i <= m; i++)
		{
			for (j = 1; j <= n; j++)
			{
				diff = fabs(snew[i][j] - s[i][j]);
				if (diff > difmax)
				{
					difmax = diff;
				}
				// copy new to old temperatures
				s[i][j] = snew[i][j];
			}
		}
	}

	double tstop1 = omp_get_wtime(); // End of timer for SERIAL

	printf("\ntime taken is %4.3lf uSeconds\n", (tstop1 - tstart1) * 1000000);

	// print results
	printf("\niter = %d  difmax = %9.11lf", iter, difmax);
	for (i = 0; i <= m + 1; i++)
	{
		printf("\n");
		for (j = 0; j <= n + 1; j++)
		{
			printf("%3.5lf ", s[i][j]);
		}
	}
	printf("\n");

	/// SUBSTACTION O/P
	printf("\n############## SERIAL-PARELLEL ############\n");

	for (i = 0; i <= m + 2; i++)
	{
		printf("\n");
		for (j = 0; j <= n + 2; j++)
		{
			sub[i][j] = s[i][j] - t[i][j]; // Subracting serial o/p from parellel.
			printf("%3.5lf ", sub[i][j]);  // Subtracted o/p
		}
	}
	printf("\n");

	printf("\n\nSpeed up is %0.02f\n", (tstop1 - tstart1) / tstop);
}

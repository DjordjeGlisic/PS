#include<stdio.h>
#include<mpi.h>
#define N 1000
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	double h = 1 /(double) N;
	double sum = 0.0f;
	int i = 0;
	while (true)
	{
		double subX = (0.5 + rank + i * size);
		if (subX < N)
		{
			double x = h * (0.5+rank + i*size);
			double fx = (double)4 / (1 + x * x);
			fx *= h;
			sum += fx;
			i ++;

		}
		else
		{
			break;
		}
	}
	double rez;
	MPI_Reduce(&sum, &rez, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("Pi=%lf\n", rez);
	}
	MPI_Finalize();

}
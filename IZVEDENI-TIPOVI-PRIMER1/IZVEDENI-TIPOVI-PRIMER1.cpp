#include<stdio.h>
#include<mpi.h>
#include <corecrt_malloc.h>
#define n 4
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
	{
		double* matrica = (double*)malloc(n * n * sizeof(double));
		double broj = 1.0f;
		for(int i=0;i<n;i++)
			for (int j = 0;j < n;j++)
			{
				matrica[i*n+j] = broj;
				broj++;
			}
		MPI_Datatype sredina;
		/*pod a
		MPI_Type_vector(n, 2, n, MPI_DOUBLE, &sredina);*/
		/*pod bMPI_Type_vector(n - 1, 1, n + 1, MPI_DOUBLE, &sredina);*/
		MPI_Type_vector(n, 1, n - 1, MPI_DOUBLE, &sredina);
		MPI_Type_commit(&sredina);
		MPI_Send(&matrica[0*n+n-1], 1, sredina, 1, 1, MPI_COMM_WORLD);
	}
	else
	{
		double* buff = (double*)malloc(n * sizeof(double));
		//pod aMPI_Recv(buff, 2 * n,MPI_DOUBLE, 0, 1, MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
		//pod bMPI_Recv(buff, n-1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Recv(buff, n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		printf("Proces 1 je dobio vektor podataka [");
		for (int i = 0;i < n;i++)
			printf("%lf ", buff[i]);
		printf("]\n");
	}
	MPI_Finalize();

	return 0;
}
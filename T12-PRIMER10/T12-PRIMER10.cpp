#include<stdio.h>
#include<mpi.h>
#include <corecrt_malloc.h>
int main(int argc, char* argv[])
{
	int rank, size;
	int* data = (int*)malloc(5 * sizeof(int));
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* niz = nullptr;
	if (rank == 0)
	{
		 niz = (int*)malloc(5 * size * sizeof(int));
		for (int i = 0;i < 5 * size;i++)
			niz[i] = i;
	}
	MPI_Scatter(niz, 5, MPI_INT, data, 5, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Podaci koje je primio proces %d:[", rank);
	for(int i=0;i<5;i++)
		printf("%d,", data[i]);
	printf("]\n");



	MPI_Finalize();
	return 0;
}
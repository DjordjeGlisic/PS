#include<stdio.h>
#include<mpi.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int sendBuf = rank + 1;
	int prod;
	MPI_Reduce(&sendBuf, &prod, 1, MPI_INT, MPI_PROD, 7, MPI_COMM_WORLD);
	if (rank == 7)
		printf("Proizvod prvih %d prirodnih brojeva je %d ", size, prod);

	MPI_Finalize();
	return 0;
}
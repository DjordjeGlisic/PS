#include<stdio.h>
#include<mpi.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	printf("PROCESS ID:%d OF TOTAL: %d PROCESS PRINTS: HELLO WORLD", rank, size);
	MPI_Finalize();
	return 0;
}

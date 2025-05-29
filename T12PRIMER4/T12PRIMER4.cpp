#include<stdio.h>
#include<mpi.h>
int main(int argc, char* argv[])
{
	int rank, size, sum;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
	{
		sum = 0+1;
		if (size > 1)
			MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);

	}
	else
	{
		MPI_Status status;
		MPI_Recv(&sum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
		sum += rank + 1;
		if(rank!=size-1)
			MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		else
		{
			printf("Izracunata suma prvih %d prirodnih brojeva je %d i ovo stampa proces sa rankom=%d\n", size, sum, rank);
		}
	}

	MPI_Finalize();
	return 0;
}
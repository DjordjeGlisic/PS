#include<stdio.h>
#include<mpi.h>
#include <corecrt_malloc.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* local = (int*)malloc(5 * sizeof(int));
	for (int i = 0;i < 5;i++)
		local[i] = rank;
	int* recvBuff = (int*)malloc(5*size*sizeof(int));
	MPI_Gather(local, 5, MPI_INT, recvBuff, 5, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("Proces %d je od svih procesa primio po 5 elemenata: [", rank);
		for (int i = 0;i < 5 * size;i++)
			printf("%d,", recvBuff[i]);
		printf("]\n");
	}

	MPI_Finalize();
	return 0;
}
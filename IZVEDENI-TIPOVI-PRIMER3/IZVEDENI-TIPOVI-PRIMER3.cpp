#include<stdio.h>
#include<mpi.h>
#include <corecrt_malloc.h>
#define N 5
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* matrica = (int*)malloc(N * N * sizeof(int));
	for (int i = 0;i < N;i++)
		for (int j = 0;j < N;j++)
			matrica[i * N + j] = rank;
	MPI_Datatype sub;
	int dimn[] = {N,N};
	int simn[] = { 2,2 };
	int start[] = { 1,1 };
	MPI_Type_create_subarray(2, dimn, simn, start, MPI_ORDER_C, MPI_INT, &sub);
	MPI_Type_commit(&sub);
	if (rank == 1)
	{
		MPI_Send(matrica, 1, sub, 0, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Recv(matrica, 1, sub, 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		for (int i = 0;i < N;i++)
		{
			printf("[");
			for (int j = 0;j < N;j++)
			{
				printf("%d ", matrica[i * N + j]);

			}
			printf("]\n");
		}
	}
	MPI_Finalize();

}
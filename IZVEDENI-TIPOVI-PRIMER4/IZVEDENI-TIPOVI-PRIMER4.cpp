#include<stdio.h>
#include<mpi.h>
#define N 3
#define M 4
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
	{
		int matrica[] = { 1,2,3,4,5,6,7,8,9,10,11,12 };
		MPI_Datatype vektor;
		MPI_Type_vector(N, 1, M, MPI_INT, &vektor);
		MPI_Type_commit(&vektor);
		MPI_Send(matrica, 1, vektor, 1, 1, MPI_COMM_WORLD);
	}
	else
	{
		int matrica[100];
		for (int i = 0;i < N;i++)
			for (int j = 0;j < M;j++)
				matrica[i * M + j] = 0;
		MPI_Recv(matrica, N, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		printf("Proces1 stampa modifikovanu matricu\n");
		for (int i = 0;i < N;i++)
		{

			printf("[");
			for (int j = 0;j < M;j++)
				printf("%d ", matrica[i * M + j]);
			printf("]\n");

		}
	}
	MPI_Finalize();
	return 0;
}
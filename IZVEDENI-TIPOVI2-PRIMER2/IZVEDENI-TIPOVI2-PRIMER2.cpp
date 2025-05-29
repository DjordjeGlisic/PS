#include<stdio.h>
#include<mpi.h>
#include <malloc.h>
#define M 2
#define N 3
#define K 4
int main(int argc, char* argv[])
{
	int rank, size;
	int* A = nullptr;
	int* B=nullptr;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
	{
		A = (int*)malloc(M * N * sizeof(int));
		B = (int*)malloc(N * K * sizeof(int));
		int br = 1;
		for (int i = 0;i < M;i++)
			for (int j = 0;j < N;j++)
			{
				A[i*N+j] = br;
				br++;
			}
		br = 1;
		for (int i = 0;i < N;i++)
			for (int j = 0;j < K;j++)
			{
				B[i * K + j] = br;
				br++;
			}


	}
	MPI_Datatype vector,vectorResized;
	MPI_Type_vector(M, 1, N, MPI_INT, &vector);
	MPI_Type_commit(&vector);
	MPI_Type_create_resized(vector, 0, 1 * sizeof(int), &vectorResized);
	MPI_Type_commit(&vectorResized);
	int* kolona = (int*)malloc(M * sizeof(int));
	int* vrsta = (int*)malloc(K * sizeof(int));

	MPI_Scatter(A, 1, vectorResized, kolona, M, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Proces%d=[",rank);
	for (int i = 0;i < M;i++)
		printf("%d ", kolona[i]);
	printf("]\n");
	MPI_Scatter(B, K, MPI_INT, vrsta, K, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Proces%d=[",rank);
	for (int i = 0;i < K;i++)
		printf("%d ", vrsta[i]);
	printf("]\n");
	int* parciajlniNiz = (int*)malloc(M * K * sizeof(int));
	int count = 0;
	for(int i=0;i<M;i++)
		for (int j = 0;j < K;j++)
		{
			parciajlniNiz[count] = kolona[i] * vrsta[j];
			count++;
		}
	int* sum = (int*)malloc(M * K * sizeof(int));
	MPI_Reduce(parciajlniNiz, sum, count, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		for (int i = 0;i < M;i++)
		{
			printf("[");
			for (int j = 0;j < K;j++)
			{
				printf("%d ", sum[i * K + j]);
			}
			printf("]\n");
		}
	}

	MPI_Finalize();

	return 0;
}
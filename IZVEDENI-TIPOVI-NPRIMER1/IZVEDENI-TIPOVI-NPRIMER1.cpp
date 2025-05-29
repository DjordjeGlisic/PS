#include<stdio.h>
#include<mpi.h>
#include <malloc.h>
#define N 3
#define M 4
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int np = M / size;
	MPI_Datatype kolone,resKolone;
	MPI_Type_vector(N, np, M, MPI_INT, &kolone);
	MPI_Type_commit(&kolone);
	MPI_Type_create_resized(kolone, 0, np * sizeof(int), &resKolone);
	MPI_Type_commit(&resKolone);
	int* matrica = (int*)malloc(N * M * sizeof(int));

	if (rank == 0)
	{
		int br = 1;
		for (int i = 0;i < N;i++)
		{
			printf("[");
			for (int j = 0;j < M;j++)
			{
				matrica[i * M + j] = br;
				br++;
				printf("%d ", matrica[i * M + j]);
			}
			printf("]\n");
		}


	}
	int* column = (int*)malloc(np*N * sizeof(int));
	MPI_Scatter(matrica, 1, resKolone, column, np*N, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Proces%d=[", rank);
	for (int i = 0;i < np * N;i++)
		printf("%d ", column[i]);
	printf("]\n");
	int* nizKolona = (int*)malloc(M * N * sizeof(int));
	//////////////////////
	MPI_Gather(column, np*N, MPI_INT, nizKolona, np*N , MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(nizKolona, N * M, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Proces%d stampa niz kolona koji je dobio [", rank);
	for (int i = 0;i < N * M;i++)
		printf("%d ", nizKolona[i]);
	printf("]\n");
	MPI_Finalize();

	return 0;
}
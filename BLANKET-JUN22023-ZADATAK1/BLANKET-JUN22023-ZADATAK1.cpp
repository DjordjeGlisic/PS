#include<stdio.h>
#include<mpi.h>
#include<malloc.h>
#define K 9
#define N 4
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* A = nullptr;
	int* B = (int*)malloc(N * sizeof(int));
	if (rank == 0)
	{
		int mat[K][N] = {
			{0,1,0,0},
			{1,0,1,1},
			{0,1,1,0},
			{1,0,0,1},
			{1,1,1,1},
			{0,0,0,0},
			{1,0,1,0},
			{0,1,0,1},
			{1,1,0,0}
		};
		A = (int*)malloc(K * N * sizeof(int));
		for (int i = 0;i < K;i++)
		{
			for (int j = 0;j < N;j++)
			{
				A[i * N + j] = mat[i][j];
			}

		}
		int br = 1;
		for (int i = 0;i < N;i++)
		{
			B[i] = br++;
		}

	
	}	
	MPI_Datatype nov,pravi;
	int kolko = K / size;
	MPI_Type_vector(kolko, N, size * N, MPI_INT, &nov);
	MPI_Type_commit(&nov);
	MPI_Type_create_resized(nov, 0, N * sizeof(int), &pravi);
	MPI_Type_commit(&pravi);
	int* parcA = (int*)malloc(kolko*N* sizeof(int));
	MPI_Scatter(A, 1, pravi, parcA, kolko * N, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(B, N, MPI_INT, 0, MPI_COMM_WORLD);
	/*printf("Proces%d stampa svoj deo matrice A\n", rank);
	for (int i = 0;i < kolko;i++)
	{
		printf("[");
		for (int j = 0;j < N;j++)
		{
			printf("%d ", parcA[i * N + j]);
		}
		printf("]\n");
	}*/
	/*printf("Proces%d B=[",rank);
	for (int i = 0;i < N;i++)
		printf("%d ", B[i]);
	printf("]\n");*/
	int* parcVektor = (int*)malloc(kolko * sizeof(int));
	double min = 10000000000;
	for (int i = 0;i < kolko;i++)
	{
		parcVektor[i] = 0;
		for (int j = 0;j < N;j++)
		{
			parcVektor[i] += parcA[i * N + j] * B[j];
		}
		if (min > parcVektor[i])
			min = (double)parcVektor[i];

	}
	int* rez = (int*)malloc(K * sizeof(int));
	struct ValRank {
		double val;
		int rank;
	};
	struct ValRank loc, glob;
	loc.rank = rank;
	loc.val =min;
	MPI_Reduce(&loc, &glob, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&glob, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
	MPI_Datatype tip;
	MPI_Type_vector(kolko, 1, kolko, MPI_INT, &tip);
	MPI_Type_commit(&tip);
	MPI_Datatype izv;
	MPI_Type_create_resized(tip,0,sizeof(int),&izv);
	MPI_Type_commit(&izv);

	MPI_Gather(parcVektor, kolko, MPI_INT, rez, 1, izv, glob.rank, MPI_COMM_WORLD);
	if (rank == glob.rank)
	{
		printf("Proces%d stampa rezultat mnozenja matrice i vektora: rez=[", rank);
		for (int i = 0;i < K;i++)
		{
			printf("%d ", rez[i]);
		}
		printf("]\n");
	}

	MPI_Finalize();
	return 0;
}
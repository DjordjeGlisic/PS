#include<mpi.h>
#include<stdio.h>
#include<malloc.h>
#define M 9
#define K 3
void Transponuj(int* A,int  vrste,int  kolone,int*B)
{
	printf("MAtrica B=\n");
	for (int i = 0;i < vrste;i++)
	{
		printf("[");
		for (int j = 0;j < kolone;j++)
		{
			B[j * vrste + i] = A[i * kolone + j];
			printf(" %d ", B[j * vrste + i]);
		}
		printf("]\n");
	}

}
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* A = nullptr;
	int* B = nullptr;
	int* transA = nullptr;
	if (rank == 0)
	{
		A = (int*)malloc(K * M * sizeof(int));
		int pomA[K][M] = {
			{1,0,0,1,1,1,0,0,0},
			{0,1,0,1,0,1,1,1,0},
			{1,1,0,0,1,1,1,0,1}
		};
		B = (int*)malloc(M * sizeof(int));
		int br = 1;
		for (int i = 0;i < K;i++)
		{
			for (int j = 0;j < M;j++)
			{
				A[i * M + j] = pomA[i][j];
				if (i == 0)
				{
					B[j] = br++;
				}
			}
		}
		 transA = (int*)malloc(M * K * sizeof(int));
		Transponuj(A, K, M,transA);
	}
	int kolko = M / size;
	int* parcA = (int*)malloc(kolko * K * sizeof(int));
	MPI_Datatype p;
	MPI_Type_vector(kolko, K, kolko*K, MPI_INT, &p);
	MPI_Type_commit(&p);
	MPI_Datatype tip;
	MPI_Type_create_resized(p,0,K*sizeof(int),&tip);
	MPI_Type_commit(&tip);
	MPI_Scatter(transA, 1, tip, parcA, K * kolko, MPI_INT, 0, MPI_COMM_WORLD);
	int* parcB = (int*)malloc(kolko * sizeof(int));
	MPI_Datatype p1;
	MPI_Type_vector(kolko, 1, kolko, MPI_INT, &p1);
	MPI_Type_commit(&p1);
	MPI_Datatype tip1;
	MPI_Type_create_resized(p1, 0, sizeof(int), &tip1);
	MPI_Type_commit(&tip1);
	MPI_Scatter(B, 1, tip1, parcB, kolko, MPI_INT, 0, MPI_COMM_WORLD);
	int* parcC = (int*)malloc(kolko * sizeof(int));
	double min = 10000000.0f;
	for (int i = 0;i < K;i++)
	{
		parcC[i] = 0;
		for (int j = 0;j < kolko;j++)
		{
			parcC[i] += parcA[j * K + i]*parcB[j];
		}
		if (parcC[i]< min)
			min = parcC[i];

	}
	struct ValRank {
		double val;
		int rank;
	};
	struct ValRank loc, glob;
	loc.rank = rank;
	loc.val = min;
	MPI_Reduce(&loc, &glob, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&glob, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
	int* C = nullptr;
	if (rank == glob.rank)
		C = (int*)malloc(K * sizeof(int));
	MPI_Reduce(parcC, C, K, MPI_INT, MPI_SUM, glob.rank, MPI_COMM_WORLD);
	if (rank == glob.rank)
	{
		printf("Rezultat mnozenja matrice A i matrice B  stampa proces%d\n", rank);
		printf("[");
		for (int i = 0;i < K;i++)
			printf(" %d ", C[i]);
		printf("]\n");

	}

	
	MPI_Finalize();
	return 0;
}
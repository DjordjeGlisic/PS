#include<mpi.h>
#include<stdio.h>
#include<malloc.h>
#define K 3 
#define M 9
void Transponuj(int*ulaz, int v, int k, int*izlaz)
{
	for (int i = 0;i < v;i++)
	{
		for (int j = 0;j < k;j++)
		{
			izlaz[j * v + i] = ulaz[i * k + j];
		}
	}
}
int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* A = nullptr;
	int* B = (int*)malloc(M * sizeof(int));
	int* pom = nullptr;
	MPI_Datatype vec,izv;
	int kolko = M / size;
	MPI_Type_vector(kolko, K, size*K, MPI_INT, &vec);
	MPI_Type_commit(&vec);
	MPI_Type_create_resized(vec, 0, K * sizeof(int), &izv);
	MPI_Type_commit(&izv);
	if (rank == 0)
	{
		int matA[K][M] = {
			{1,0,0,1,1,1,0,0,0},
			{0,1,0,1,0,1,1,1,0},
			{1,1,0,0,1,1,1,0,1}
		};
		A = (int*)malloc(K * M * sizeof(int));
		int br = 1;
		for (int i = 0;i < K;i++)
		{
			for (int j = 0;j < M;j++)
			{
				if (i == 0)
					B[j] = br++;
				A[i * M + j] = matA[i][j];
			}
		}
		pom = (int*)malloc( M *K* sizeof(int));
		Transponuj(A, K, M, pom);
		
	}
	int* parcA = (int*)malloc(kolko * K * sizeof(int));
	int* parcB = (int*)malloc(kolko * sizeof(int));
	MPI_Scatter(pom, 1, izv, parcA, K * kolko*sizeof(int), MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Datatype vec1, izv1;
	
	MPI_Type_vector(kolko, 1, size, MPI_INT, &vec1);
	MPI_Type_commit(&vec1);
	MPI_Type_create_resized(vec1, 0, sizeof(int), &izv1);
	MPI_Type_commit(&izv1);
	MPI_Scatter(B, 1, izv1, parcB, kolko, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Proces%d A\n",rank);
	for (int i = 0;i < kolko;i++)
	{
		printf("[");
		for(int j=0;j<K;j++)
			printf(" %d ", parcA[i*K+j]);
		printf("]\n");
	}
	printf("Proces%d B=[", rank);
	for (int i = 0;i < kolko;i++)
		printf(" %d ", parcB[i]);
	printf("]\n");
	struct ValRank {
		double val;
		int rank;
	};
	struct ValRank glob, loc;
	loc.rank = rank;
	int* parcC = (int*)malloc(K * sizeof(int));
	loc.val = 0.0f;
	for (int i = 0;i < K;i++)
	{
		parcC[i] = 0;
		for (int j = 0;j < kolko;j++)
		{
			if (i == 0)
			{
				if (parcB[j] > loc.val)
					loc.val = parcB[j];
			}
			parcC[i] += parcA[j * K + i] * parcB[j];
		}

	}
	MPI_Reduce(&loc, &glob, 1, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&glob, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
	int* C = nullptr;
	if (rank == glob.rank)
		C = (int*)malloc(K * sizeof(int));
	MPI_Reduce(parcC, C, K, MPI_INT, MPI_SUM, glob.rank, MPI_COMM_WORLD);
	if (rank == glob.rank)
	{
		printf("Rezultat mnzoenja matrice i vektora se nalazi u Procesu%d i iznosi C=[", rank);
		for (int i = 0;i < K;i++)
			printf(" %d ", C[i]);
		printf("]\n");
	}

	MPI_Finalize();
	return 0;
}
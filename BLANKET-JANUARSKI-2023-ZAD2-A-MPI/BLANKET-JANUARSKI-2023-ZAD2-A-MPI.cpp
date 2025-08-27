#include<stdio.h>
#include<mpi.h>
#include<malloc.h>
#include<math.h>
#define N 8
#define K 2
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* A = nullptr;
	int* B = nullptr;
	int* C = nullptr;
	if (rank == 0)
	{
		int MatA[N][N] = {
			{1,0,0,1,0,0,1,1},
			{0,0,1,1,1,1,0,1},
			{1,0,1,0,1,0,1,1},
			{0,1,0,0,1,0,0,0},
			{1,1,1,0,0,0,1,1},
			{1,0,0,1,0,0,1,0},
			{0,0,1,0,0,1,0,0},
			{1,1,1,0,0,0,1,1}

		};
		int vekB[N][N] = {
			{1,1,2,2,1,3,7,1 },
			{2,2,1,1,3,1,9,2},
			{3,3,2,2,1,3,6,3},
			{4,4,7,7,3,1,0,1},
			{1,1,0,0,1,3,1,0},
			{2,2,2,2,3,1,1,0},
			{3,3,1,1,1,3,2,2},
			{4,4,1,1,3,1,2,2}
		
		};
		A = (int*)malloc(N * N * sizeof(int));
		B = (int*)malloc(N * N * sizeof(int));
		for (int i = 0;i < N;i++)
		{
			for (int j = 0;j < N;j++)
			{
				A[i * N + j] = MatA[i][j];
				B[i * N + j] = vekB[i][j];

			}
		}

	}
	MPI_Datatype vrste;
	MPI_Type_vector(K, N, 0, MPI_INT, &vrste);
	MPI_Type_commit(&vrste);
	MPI_Datatype kolone;
	MPI_Type_vector(N, K, N, MPI_INT, &kolone);
	MPI_Type_commit(&kolone);
	int* parcA = (int*)malloc(K * N * sizeof(int));
	int* parcB = (int*)malloc(N*K * sizeof(int));
	int t = size;
	int kolko = N / K;
	if (rank == 0)
	{
		for (int i = 0;i < K;i++)
		{
			for (int j = 0;j < N;j++)
			{
				parcA[i * N + j] = A[i * N + j];
				//printf("Proces%d a[%d][%d]=%d\n",rank,i,j, parcA[i * N + j]);
			
			}
		}
		for (int i = 0;i < N;i++)
		{
			for (int j = 0;j < K;j++)
			{

				parcB[i * N + j] = B[i * N + j];
				printf("Proces%d b[%d][%d]=%d\n", rank, i, j, parcB[i * N + j]);

			}
		}

		for (int i = 1;i < size;i++)
		{
			int idB = i / kolko;
			int idA = i % kolko;
			MPI_Send(&A[idA * K * N], 1, vrste, i, i, MPI_COMM_WORLD);
			MPI_Send(&B[idB * K], 1, kolone, i, t*i, MPI_COMM_WORLD);
		}

	}
	else
	{
		MPI_Recv(parcA, K * N, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Recv(parcB, N*K, MPI_INT, 0, t*rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	}
	int* parcC = (int*)malloc(K * K * sizeof(int));
	struct ValRank {
		double val;
		int rank;
	}loc,glob;
	loc.rank = rank;
	loc.val = -1;
	for (int i = 0;i < K;i++)
	{
		for (int j = 0;j < K;j++)
		{
			parcC[i*K+j] = 0;
			for (int z = 0;z < N;z++)
			{
				if (rank == 0)
				{
					printf("B[%d][%d]=%d\n", z, j, parcB[z * K + j]);

				}
				parcC[i*K+j] += parcA[i * N + z] * parcB[z*K+j];
			}
			if (parcC[i*K+j] > loc.val)
				loc.val = (double)parcC[i*K+j];
	
		}
	}
	if (rank == 0)
	{
		printf("MAtrica Cprac\n");
		for (int i = 0;i < K;i++)
		{
			printf("[");
			for (int j = 0;j < K;j++)
				printf(" %d ", parcC[i * K + j]);
			printf("]\n");
		}
	}
	MPI_Reduce(&loc, &glob, 1, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	MPI_Comm COL;
	int colorK;
	colorK = rank % kolko;
	MPI_Comm_split(MPI_COMM_WORLD, colorK,0, &COL);
	int newRank;
	MPI_Comm_rank(COL, &newRank);
	int* Ccol = nullptr; 
	if (newRank == 0)
	{
		Ccol=(int*)malloc(kolko* K* K * sizeof(int));
		
	}
	MPI_Gather(parcC, K* K, MPI_INT, Ccol, K* K, MPI_INT, 0, COL);
	if(rank==0)
		C=(int*)malloc(N* N * sizeof(int));
	MPI_Comm VRS;
	int colorV;
	colorV = rank / kolko;
	MPI_Comm_split(MPI_COMM_WORLD, colorV, 0, &VRS);
	if (colorV == 0)
	{
		MPI_Gather(Ccol, kolko* K* K, MPI_INT, C, kolko* K* K, MPI_INT, 0, VRS);
	}
	//if (rank == 0)
	//{
	//	printf("Proces%d je imao najveci element matrice A i to je %lf a rezultujca matrica\n", glob.rank, glob.val);
	//	for (int i = 0;i < N;i++)
	//	{
	//		printf("[");
	//		for (int j = 0;j < N;j++)
	//			printf(" %d ", C[i * N + j]);
	//		printf("]\n");
	//	}
	//}
	
	MPI_Finalize();
	return 0;
}
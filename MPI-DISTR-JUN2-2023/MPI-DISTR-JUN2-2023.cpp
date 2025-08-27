#include<stdio.h>
#include<mpi.h>
#include<malloc.h>
#define N 4
#define K 9
#define Q 3
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* A = nullptr;
	int* B = nullptr;
	if (rank == 0)
	{
		A = (int*)malloc(N * K * sizeof(int));
		B = (int*)malloc(K * N * sizeof(int));
		int pomA[N][K] = {
			{0,1,1,1,1,1,1,0,0},
			{1,1,0,0,1,0,0,1,0},
			{1,1,1,0,0,0,1,0,1},
			{1,0,1,0,1,0,1,0,1}

		};
		int pomB[K][N] = {
			{1,2,3,4},
			{5,6,7,8},
			{9,0,7,1},
			{3,2,2,1},
			{0,5,4,7},
			{3,8,2,9},
			{1,0,7,2},
			{0,8,0,2},
			{3,5,7,4}
		};
		for (int i = 0;i < N;i++)
			for (int j = 0;j < K;j++)
				A[i * K + j] = pomA[i][j];
		for (int i = 0;i < K;i++)
			for (int j = 0;j < N;j++)
				B[i * N + j] = pomB[i][j];


	}
	int* kolA = (int*)malloc( N *Q *sizeof(int));
	int* vrsB = (int*)malloc(Q * N * sizeof(int));

	if (rank == 0)
	{
		for (int i = 0;i < N;i++)
		{
			for (int j = 0;j < Q;j++)
			{
				kolA[i * Q + j] = A[i * K + j];
			}
		}
		MPI_Datatype tip;
		MPI_Type_vector(N, Q, K, MPI_INT, &tip);
		MPI_Type_commit(&tip);
		for (int i = 1;i < size;i++)
		{
			int pomeraj = i * Q;
			MPI_Send(&A[pomeraj], 1, tip, i, i, MPI_COMM_WORLD);
			
		}
	}
	else
	{
		MPI_Recv(kolA, N * Q, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	}
	MPI_Scatter(B, Q * N, MPI_INT, vrsB, Q * N, MPI_INT, 0, MPI_COMM_WORLD);
	/*if (rank == 2)
	{
		printf("Proces%d podmatricaA\n",rank);
		for (int i = 0;i < N;i++)
		{
			printf("[");
			for (int j = 0;j < Q;j++)
				printf(" %d ", kolA[i * Q + j]);
			printf("]\n");
		}
		printf("Proces%d podmatricaB\n",rank);
		for (int i = 0;i < Q;i++)
		{
			printf("[");
			for (int j = 0;j < N;j++)
				printf(" %d ", vrsB[i * N + j]);
			printf("]\n");
		}

	}*/
	int* parcC = (int*)malloc(N * N * sizeof(int));
	for (int i = 0;i < N;i++)
	{
		for (int j = 0;j < N;j++)
		{
			parcC[i * N + j] = 0;
			for (int z = 0;z < Q;z++)
				parcC[i*N+j] += kolA[i * Q + z] * vrsB[z * N + j];

		}

	}
	struct ValRank {
		double val;
		int rank;
	};
	struct ValRank loc, glob;
	loc.rank = rank;
	loc.val = 0.0f;
	for (int i = 0;i < Q;i++)
	{
		for (int j = 0;j < N;j++)
		{
			if ((int)loc.val < vrsB[i * N + j])
				loc.val = (double)vrsB[i * N + j];
		}
	}
	MPI_Reduce(&loc, &glob, 1, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&glob, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
	int* C = nullptr;
	if (rank == glob.rank)
	{
		C=(int*)malloc(N* N * sizeof(int));


	}
	MPI_Reduce(parcC, C, N* N, MPI_INT, MPI_SUM, glob.rank, MPI_COMM_WORLD);
	if (rank == glob.rank)
	{
		printf("Proces%d Matrica C iznosi\n", rank);
		for (int i = 0;i < N;i++)
		{
			printf("[");
			for (int j = 0;j < N;j++)
				printf(" %d ", C[i * N + j]);
			printf("]\n");
		}
	}
	MPI_Finalize();
}
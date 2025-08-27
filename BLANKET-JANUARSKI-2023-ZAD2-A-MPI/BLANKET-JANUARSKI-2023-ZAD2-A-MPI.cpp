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
	MPI_Type_vector(1, K, 0, MPI_INT, &kolone);
	MPI_Type_commit(&kolone);
	int* parcA = (int*)malloc(K * N * sizeof(int));
	int* parcB = (int*)malloc(K * sizeof(int));
	int t = size;
	int kolko = N / K;
	if (rank == 0)
	{
		for (int i = 0;i < K;i++)
		{
			for (int j = 0;j < N;j++)
				parcA[i * N + j] = A[i * N + j];
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
		MPI_Recv(parcB, K, MPI_INT, 0, t*rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

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
		parcC[i] = 0;
		for (int j = 0;j < K;j++)
		{
			for (int z = 0;z < N;z++)
			{
				parcC[i] += parcA[j * N + z] * parcB[j * K + z];
			}
		}
		if (parcC[i] > loc.val)
			loc.val = parcC[i];
	}
	MPI_Reduce(&loc, &glob, 1, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	int q = N / K;
	int*C=(int*)malloc(K*K)
	MPI_Finalize();
	return 0;
}
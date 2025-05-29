#include<stdio.h>
#include<mpi.h>
#include <corecrt_malloc.h>
#define N 4
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Group world, nova;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_group(MPI_COMM_WORLD, &world);
	int ranks[] = { 0 };
	MPI_Group_excl(world, 1, ranks, &nova);
	MPI_Comm novaCom;
	MPI_Comm_create(MPI_COMM_WORLD, nova, &novaCom);
	if (rank == 0)
	{
		int* a = (int*)malloc(N * N * sizeof(int));
		int* b = (int*)malloc(N * N * sizeof(int));
		int br = 1;
		for (int i = 0;i < N;i++)
		{
			for (int j = 0;j < N;j++)
			{
				a[i * N + j] = br;
				br++;
			}
		}
		for (int i = 0;i < N;i++)
		{
			for (int j = 0;j < N;j++)
			{
				b[i * N + j] = br;
				br++;
			}
		}
		MPI_Datatype vector,vectorResized;
		MPI_Type_vector(N, 1, N, MPI_INT, &vector);
		MPI_Type_commit(&vector);
		MPI_Type_create_resized(vector, 0, 1 * sizeof(int), &vectorResized);
		MPI_Type_commit(&vectorResized);
		br = 1;
		for (int i = 0;i < N;i++)
		{
			MPI_Send(&a[0], 1, vectorResized, br, br, MPI_COMM_WORLD);
			MPI_Send(&b[i * N], N, MPI_INT, br, br, MPI_COMM_WORLD);
			br++;
		}
	}
	else
	{
		int* kol = (int*)malloc(N * sizeof(int));
		int* vrsta = (int*)malloc(N * sizeof(int));
		MPI_Recv(kol, N, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Recv(vrsta, N, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

		
		
		int* parc = (int*)malloc(N * N * sizeof(int));
		int count = 0;
		for (int i = 0;i < N;i++)
		{
			for (int j = 0;j < N;j++)
			{
				parc[count++] = vrsta[i] * kol[j];
			}
		}
		int* sum = (int*)malloc(N * N + sizeof(int));
		MPI_Reduce(parc, sum, count, MPI_INT, MPI_SUM, 0, novaCom);
		int newRank;
		MPI_Comm_rank(novaCom, &newRank);
		if (newRank == 0)
		{
			printf("Proces%d koji je u novoj grupi postao Proces%d stampa rezultujucu matricu\n ", rank, newRank);
			for (int i = 0;i < N;i++)
			{
				printf("[");
				for (int j = 0;j < N;j++)
					printf("%d ", sum[i * N + j]);
				printf("]\n");
			}
		}
	}
	MPI_Finalize();
	return 0;
}
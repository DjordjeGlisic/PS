#include<stdio.h>
#include<mpi.h>
#include <corecrt_malloc.h>
#define N 5
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* matrica = (int*)malloc(N * N * sizeof(int));
		int br = 1;
		for (int i = 0;i < N;i++)
			for (int j = 0;j < N;j++)
			{
				if (rank == 0)
				{
					matrica[i * N + j] = br;
					br++;

				}
				else
					matrica[i * N + j] = 0;
			}
		int count = N;
		int* blen = (int*)malloc(N * sizeof(int));
		int* disp = (int*)malloc(N * sizeof(int));
		for (int i = 0;i < N;i++)
		{
			if (rank == 0)
			{
				disp[i] = i * N;
				blen[i] = i + 1;
			}
			else
			{
				disp[i] = i * (N + 1);
				blen[i] = N - i;
			}

		}
		MPI_Datatype index;
		
		MPI_Type_indexed(count, blen, disp, MPI_INT, &index);
		MPI_Type_commit(&index);
		if (rank == 0)
		{
			MPI_Send(matrica, 1, index, 1, 1, MPI_COMM_WORLD);
		}
		else
		{
			MPI_Recv(matrica, 1, index, 0, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
			for (int i = 0;i < N;i++)
			{
				printf("[");
				for (int j = 0;j < N;j++)
					printf("%d ", matrica[i * N + j]);
				printf("]\n");
			}
		}
			
		
	
	MPI_Finalize();

	return 0;
}
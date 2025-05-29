#include<stdio.h>
#include<mpi.h>
#include <corecrt_malloc.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
	{
		int p = size - 1;
		int n = p * (p + 1);
		int* niz = (int*)malloc(n* sizeof(int));
		for (int i = 0;i < n;i++)
			niz[i] = i;
		int brojac = 0;
		for (int i = 1;i < size;i++)
		{
			int* sendBuf = (int*)malloc(2 * i * sizeof(int));
			for (int j = 0;j < 2 * i;j++)
			{
				sendBuf[j] = niz[brojac];
				brojac++;
			}
			MPI_Send(sendBuf, 2 * i, MPI_INT, i, i, MPI_COMM_WORLD);
		}
	}
	else
	{
		int* recvBuff = (int*)malloc(2 * rank * sizeof(int));
		MPI_Recv(recvBuff, 2 * rank, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		int sum = 0;
		for (int i = 0;i < 2 * rank;i++)
		{
			sum += recvBuff[i];

		}
		printf("Proces %d je pronasao sumu %d\n", rank, sum);
	}

	MPI_Finalize();
	return 0;
}
#include<stdio.h>
#include<mpi.h>
#include<math.h>
#define R 3
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int sum = rank;
	for (int i = 0;i < R;i++)
	{
		int sendrecv = rank ^ (int)pow(2, i);
		int buff=sum;
		MPI_Sendrecv_replace(&buff, 1, MPI_INT, sendrecv, sendrecv, sendrecv, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		sum += buff;
	}
	MPI_Scan(&rank, &sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	if (rank == 0)
		printf("Suma %d prirodinih brojeva iznosi %d\n",size-1, sum );
	if (rank == size-1)
		printf("Suma %d prirodinih brojeva iznosi %d\n",size-1, sum );

	MPI_Finalize();
	return 0;
}
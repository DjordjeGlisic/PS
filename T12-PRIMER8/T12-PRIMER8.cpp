#include<mpi.h>
#include<stdio.h>
#include<math.h>
int main(int argc, char* argv[])
{
	int rank, size, data;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	data = rank;
	int res;
	MPI_Scan(&data, &res, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	printf("Proces %d stampa sumu svih rankova od 0-%d i ta suma iznosi %d\n", rank, rank, res);
	MPI_Finalize();
	return 0;
}
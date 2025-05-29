#include<stdio.h>
#include<mpi.h>
#include<math.h>
#define M 4
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int ndims = 2;
	int dims[] = { M,M};
	int periods[] = { 1,0 };
	MPI_Comm resetka;
	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 1, &resetka);
	int newRank;
	MPI_Comm_rank(resetka, &newRank);
	int gornji, donji;
	MPI_Cart_shift(resetka, 0, -1, &donji, &gornji);
	int inout = rank;
	MPI_Sendrecv_replace(&inout, 1, MPI_INT, donji, donji, gornji, newRank, resetka, MPI_STATUSES_IGNORE);
	MPI_Barrier(resetka);
	printf("Proces%d je primio podatak od Proces%d i to je %d i poslao ga procsu Proces%d\n", newRank, gornji, inout, donji);
	MPI_Finalize();
	return 0;
}
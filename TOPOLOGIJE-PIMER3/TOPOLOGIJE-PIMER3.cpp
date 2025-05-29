#include<stdio.h>
#include<mpi.h>
#define Ndims 1
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int dims[Ndims];
	int periods[Ndims];
	dims[0] = size;
	periods[0] = 1;
	MPI_Comm lin;
	int newRank, levi, desni;
	MPI_Cart_create(MPI_COMM_WORLD, Ndims, dims, periods, 1, &lin);
	MPI_Comm_rank(lin, &newRank);
	MPI_Cart_shift(lin, 0, -1, &desni, &levi);
	int buffSlanje=rank;
	int buffPrijem;
	
	
	MPI_Sendrecv(&buffSlanje, 1, MPI_INT, desni, desni, &buffPrijem, 1, MPI_INT, levi, newRank, lin, MPI_STATUSES_IGNORE);
	
	MPI_Barrier(lin);
	printf("Proces%d je primio podatak %d od procesa %d i prosledio ga je procesu %d\n", newRank, buffPrijem, levi, desni);


	MPI_Finalize();
	return 0;
}
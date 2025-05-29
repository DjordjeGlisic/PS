#include<stdio.h>
#include<mpi.h>
#define N 3
#define M 4
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int ndims = 2;
	int dims[] = { N,M };
	int wraps[] = { 0,1 };
	int record = 1;
	MPI_Comm resetka;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, wraps, record, &resetka);
	int koord[] = { 0,0 };
	int newRank;
	int leviSused, desniSused, gornjiSused, donjiSused;
	MPI_Comm_rank(resetka, &newRank);
	MPI_Cart_coords(resetka, newRank, 2, koord);
	MPI_Cart_shift(resetka, 0, -1, &donjiSused, &gornjiSused);
	MPI_Cart_shift(resetka, 1, 1, &leviSused, &desniSused);
	printf("Proces%d u resetci Proces%d=(%d,%d) sa susedima levo=%d,desno=%d,gore=%d,dole=%d\n",rank,newRank,koord[0],koord[1],leviSused,desniSused,gornjiSused,donjiSused);
	MPI_Finalize();
	return 0;
}
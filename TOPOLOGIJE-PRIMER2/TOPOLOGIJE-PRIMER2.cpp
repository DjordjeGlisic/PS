#include<stdio.h>
#include<mpi.h>
#define N 1
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm periodicnaLin, neperiodicnaLin;
	int dims[N];
	dims[0] = size;
	int periodsP[N];
	periodsP[0] = {1};
	int periodsNp[N];
	periodsNp[0] = {0};
	MPI_Cart_create(MPI_COMM_WORLD, N, dims, periodsP, 1, &periodicnaLin);
	MPI_Cart_create(MPI_COMM_WORLD, N, dims, periodsNp, 1, &neperiodicnaLin);
	int newRankP,newRankNp, levi, desni;
	if (rank == 0 || rank == size - 1 || rank == 3)
	{
		MPI_Comm_rank(periodicnaLin, &newRankP);
		MPI_Comm_rank(neperiodicnaLin, &newRankNp);
		int koords[] = {0,0};
		MPI_Cart_coords(periodicnaLin, newRankP, N, koords);
		MPI_Cart_shift(periodicnaLin, 0, -1, &desni, &levi);
		printf("Proces%d ={%d,%d} u periodicnoj topologiji: levi=%d desni=%d\n",rank,koords[0],koords[1],levi,desni);
		MPI_Cart_coords(neperiodicnaLin, newRankNp, N, koords);
		MPI_Cart_shift(neperiodicnaLin, 0, -1, &desni, &levi);
		printf("Proces%d ={%d,%d} u ne periodicnoj topologiji levi=%d desni=%d\n", rank, koords[0], koords[1], levi, desni);


		
	}

	MPI_Finalize();
	return 0;
}
#include<mpi.h>
#include<stdio.h>
#define N 3
#define M 4
#define K 22
int main(int argc, char* argv[])
{
	int rank,size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm resetka;
	int dims[] = { N,M };
	int records[] = { 0,0 };
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, records, MPI_ORDER_C, &resetka);
	int koords[] = { -1,-1 };
	int novi;
	MPI_Comm_rank(resetka, &novi);
	MPI_Cart_coords(resetka, novi, 2,koords);
	int k=K + novi;
	if (koords[1] % 2 == 0)
	{
		printf("Proces%d sa {%d,%d} stampa K=%d pre razmene\n", novi, koords[0], koords[1], k);
	}
	if (koords[1] != 0&&koords[1]!=1)
	{
		int prijem[2] = { koords[0],koords[1] - 2 };
		int odKog;
		MPI_Cart_rank(resetka, prijem, &odKog);
		MPI_Recv(&k, 1, MPI_INT, odKog, novi, resetka, MPI_STATUSES_IGNORE);
	}
	if(koords[1]!=M-1&&koords[1]!=M-2)
	{

		int salji[2] = { koords[0],koords[1] + 2 };
		int kaKim;
		MPI_Cart_rank(resetka, salji, &kaKim);
		MPI_Send(&k, 1, MPI_INT, kaKim, kaKim, resetka);

	}
	if (koords[1] % 2 == 0)
	{
		printf("Proces%d sa {%d,%d} stampa K=%d nakon razmene\n", novi, koords[0], koords[1], k);
	}


	MPI_Finalize();

	return 0;
}
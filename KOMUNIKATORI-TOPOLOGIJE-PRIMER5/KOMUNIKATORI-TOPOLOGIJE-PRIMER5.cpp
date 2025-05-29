#include<stdio.h>
#include<mpi.h>
#include<malloc.h>
#define M 4
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int n = size / M;
	int ndims[] = { n,M };
	int dims = 2;
	int periods[] = { 1,0 };
	MPI_Comm komunikator;
	MPI_Cart_create(MPI_COMM_WORLD, dims, ndims, periods, 1, &komunikator);
	int buff = rank;
	printf("Proces%d pre primo-predaje podataka ima podatak:%d\n", rank, buff);
	int koords[] = { -1,-1 };
	MPI_Cart_coords(komunikator, rank, 2, koords);
	if (koords[1] != 0 && koords[1] != M)
	{
		int primaOd;
		int saljePrema;
		int nov;

		if (koords[1] % 2 == 1)
		{
			MPI_Cart_shift(komunikator, 0, 1, &primaOd, &saljePrema);
			MPI_Comm_rank(komunikator, &nov);
			MPI_Sendrecv_replace(&buff, 1, MPI_INT, saljePrema, saljePrema, primaOd, nov, komunikator, MPI_STATUSES_IGNORE);
		}
		else
		{
			MPI_Cart_shift(komunikator, 0, -1, &primaOd, &saljePrema);
			MPI_Comm_rank(komunikator, &nov);
			MPI_Sendrecv_replace(&buff, 1, MPI_INT, saljePrema, saljePrema, primaOd, nov, komunikator, MPI_STATUSES_IGNORE);


		}
		//printf("Proces%d salje na Proces%d a prima od Proces%d\n", rank, saljePrema, primaOd);
	}
	MPI_Barrier(komunikator);
	printf("////////////////////////////////////////////////////////////////////\n");
	printf("Proces%d nakon primo-predaje podataka ima podatak:%d\n", rank, buff);
	MPI_Finalize();

}
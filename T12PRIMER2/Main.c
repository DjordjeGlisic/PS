#include<stdio.h>
#include<mpi.h>
#include<iostream>
using namespace std;
int main(int argc, char* argv[])
{
	int rank, size, slanje, prijem;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
	{
		printf("Proces %d unesite podatak za slanje ", rank);
		slanje = 3;
		MPI_Send(&slanje, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Recv(&prijem, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);

	}
	else
	{
		printf("Proces %d unesite podatak za slanje ", rank);
		slanje = 5;

		MPI_Recv(&prijem, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Send(&slanje, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);


	}
	printf("Proces %d je primio podatak %d , a poslao podatak %d", rank, prijem, slanje);


	MPI_Finalize();
	return 0;
}
#include<stdio.h>
#include<mpi.h>
int main(int argc, char* argv[])
{
	int rank, size,data;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
		data = 23;
	if (rank == 0)
		printf("Proces 0 ce slati podatak %d svima u grupi\n", data);
	MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if(rank!=0)
		printf("Proces %d je primio od procesa 0 podatak %d\n", rank, data);

	MPI_Finalize();
	return 0;
}
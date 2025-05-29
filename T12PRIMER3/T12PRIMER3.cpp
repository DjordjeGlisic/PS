#include<stdio.h>
#include<mpi.h>
int main(int argc, char* argv[])
{
	int rank, size, buff;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int i = 0;
	
	do {
		if (rank == 0)
		{
			buff = 10-i;
			if (rank +1 < size)
				MPI_Send(&buff, 1, MPI_INT, rank+1, 30, MPI_COMM_WORLD);
			i++;
		}
		else
		{
			MPI_Status status;
			MPI_Recv(&buff, 1, MPI_INT, rank - 1, 30,MPI_COMM_WORLD ,&status);
			printf("Proces %d je primio podatak od procesa %d a vednost podatka je %d\n", rank, rank - 1, buff);
			if(rank+1<size)
				MPI_Send(&buff, 1, MPI_INT, rank+1, 30, MPI_COMM_WORLD);



		}
		
	}while (buff > -1);
	MPI_Finalize();
	return 0;
}
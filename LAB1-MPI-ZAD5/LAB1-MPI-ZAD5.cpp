#include<mpi.h>
#include<stdio.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int niz[50];
	int sum=0;
	for (int i = 0;i < 50;i++)
	{
		niz[i] = (i * size) + rank;
		sum += niz[i];
	}
	if (rank != 0)
		MPI_Send(&sum, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
	else
	{
		for (int i = 1;i < size;i++)
		{
			int data;
			MPI_Recv(&data, 1, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
			if (data % i == 0)
				printf("Suma procesa %d iznosi %d i ona  je deljiva sa IDem tog procesa\n", i, data);
		}
	}


	MPI_Finalize();
	return 0;

}
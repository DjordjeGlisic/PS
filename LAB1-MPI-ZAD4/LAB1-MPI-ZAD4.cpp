#include<stdio.h>
#include<mpi.h>
#include<math.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int buf[100];
	int sum = 0;
	for (int i = 0;i < 99;i++)
	{
		buf[i] = rank + i;
		sum += buf[i];
	}
	int data = 0;
	if (rank == size - 1)
	{
		if (sum % 2 == 0)
		{
			
			data = data xor (int)pow((double)2,(double)rank);

		}
		MPI_Send(&data, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Recv(&data, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		if (sum % 2 == 0)
		{
			data = data xor (int)pow(2, rank);
		}
		if(rank!=0)
			MPI_Send(&data, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
		else
		{
			for (int i = 0;i < size;i++)
			{
				int mask = (int)pow(2, i);
				int res = mask & data;
				if (res == mask)
					printf("Proces %d poseduje parnu sumu svojih elemenata niza\n", i);

			}
		}
	}

	MPI_Finalize();
	return 0;
}
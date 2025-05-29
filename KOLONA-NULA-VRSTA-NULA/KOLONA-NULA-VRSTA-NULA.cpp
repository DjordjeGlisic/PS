#include<mpi.h>
#define n 5
#define m 5
#include<stdio.h>
#include <malloc.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* matrica=nullptr;
	if (rank == 0)
	{
		matrica = (int*)malloc(n *m* sizeof(int));
		int brojac = 1;
		for (int i = 0;i < n;i++)
		{
			for (int j = 0;j < m;j++)
			{
				matrica[i*m+j] = brojac;
				brojac++;
			}

		}
	
	}
	else
	{
		matrica = (int*)malloc(n * m * sizeof(int));

		int brojac = 20;
		 
		 printf("Proces 1 stampa izvornu matricu\n");
		for (int i = 0;i < n;i++)
		{
			printf("[");
			for (int j = 0;j < m;j++)
			{
				matrica[i*m+j] = 0;
				
				printf("%d ", matrica[i*m+j]);
			}
			printf("]\n");

		}

	}
	MPI_Datatype newType;
	MPI_Type_vector(n, 1, m, MPI_INT, &newType);
	MPI_Type_commit(&newType);
	if (rank == 0)
		MPI_Send(matrica, 1, newType, 1, 0, MPI_COMM_WORLD);
	else
		MPI_Recv(matrica, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	if (rank == 1)
	{
		printf("Proces 1 stampa modifikovanu matricu\n");
		for (int i = 0;i < n;i++)
		{
			printf("[");
			for (int j = 0;j < m;j++)
				printf("%d ", matrica[i*m+j]);
			printf("]\n");
		}
	}

	MPI_Finalize();
	return 0;
}
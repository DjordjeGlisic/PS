#include<stdio.h>
#include<mpi.h>
#include <malloc.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//int** mat = (int**)malloc(size * sizeof(int*));
	//for (int i = 0;i < size;i++)
	//	mat[i] = (int*)malloc(size * sizeof(int));
	
	int* vektor = (int*)malloc(size * sizeof(int));
	int* mat = (int*)malloc(2 * size * sizeof(int));

	if (rank == 0)
	{
		int br = 1;
		for (int i = 0;i < size;i++)
		{
			vektor[i] = i;
			for (int j = 0;j < size;j++)
			{
				//mat[i][j] = br;
				mat[size*i+j] = br;
				br++;
			}
		}
	}
	int* vrsta = (int*)malloc(size * sizeof(int));
	//int* vor = (int*)malloc(size * sizeof(int));
	/*if (rank == 0)
	{
		for (int i = 1;i < size;i++)
			MPI_Send(vektor, size, MPI_INT, i, i, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Recv(vor, size, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	}*/
	MPI_Scatter(mat, size, MPI_INT, vrsta, size, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(vektor, size, MPI_INT, 0, MPI_COMM_WORLD);
	int sum = 0;
	for (int i = 0;i < size;i++)
	{
		//if(rank==0)
			printf("Proces %d vrsta[%d]=%d vektor[%d]=%d\n", rank, i, vrsta[i], i, vektor[i]);
		//else
		//	printf("Proces %d vrsta[%d]=%d vektor[%d]=%d\n", rank, i, vrsta[i], i, vor[i]);

			
		//if (rank == 0)
			sum += vrsta[i] * vektor[i];
		//else
			//sum += vrsta[i] * vor[i];
	}
	int* res = (int*)malloc(size * sizeof(int));
	MPI_Gather(&sum, 1, MPI_INT, res, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("Rezultat mnozenja vektora i matrice iznosi [");
		for (int i = 0;i < size;i++)
			printf("%d ", res[i]);
		printf("]\n");

	}
	MPI_Finalize();
	return 0;
}
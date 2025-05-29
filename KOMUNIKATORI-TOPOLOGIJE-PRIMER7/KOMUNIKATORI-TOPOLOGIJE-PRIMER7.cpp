#include<stdio.h>
#include<mpi.h>
#include<malloc.h>
#include<math.h>
#define N 8
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int q = sqrt(size);
	int k = N / q;
	int* matrica = nullptr;
	int* vektor = (int*)malloc(N * sizeof(int));
	int* subMatrix = (int*)malloc(k * k * sizeof(int));
	if (rank == 0)
	{
		matrica = (int*)malloc(N * N * sizeof(int));
		
		int br = 0;
		for (int i = 0;i < N;i++)
		{
			for (int j = 0;j < N;j++)
			{
				matrica[i * N + j] = br++;
			}
		}
		br = 0;
		for (int i = 0;i < N;i++)
		{
			vektor[i] = br++;
		}
		MPI_Datatype sub;


		for (int i = 0;i < k;i++)
		{
			for (int j = 0;j < k;j++)

			{
				subMatrix[i * k + j] = matrica[i * N + j];
			}
		}
		MPI_Type_vector(k, k, N, MPI_INT, &sub);
		MPI_Type_commit(&sub);
		for (int i = 1;i < size;i++)
		{
			int kolona = (i * k) % N;
			int vrsta = i / (2 * k);
			vrsta *= k;
			MPI_Send(&matrica[vrsta * N + kolona], 1, sub, i, i, MPI_COMM_WORLD);

		}

	}
	else
	{
		MPI_Recv(subMatrix, k * k, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		/*printf("Proces%d MATRICA=", rank);
		for (int i = 0;i < k;i++)
		{
			printf("[");
			for (int j = 0;j < k;j++)
				printf("%d ", subMatrix[i * k + j]);
			printf("]\n");
		}*/
	}
	MPI_Bcast(vektor, N, MPI_INT, 0, MPI_COMM_WORLD);
	int* subVector = (int*)malloc(k * sizeof(int));
	MPI_Comm commRow, commCol;
	int colorCol = rank % q;
	int colorRow = rank / q;
	MPI_Comm_split(MPI_COMM_WORLD, colorCol, rank, &commCol);
	MPI_Comm_split(MPI_COMM_WORLD, colorRow, rank, &commRow);
	int rankRow, rankCol;
	MPI_Comm_rank(commRow, &rankRow);
	MPI_Comm_rank(commCol, &rankCol);
	MPI_Scatter(vektor, k, MPI_INT, subVector, k, MPI_INT, 0, commRow);
	
	/*printf("Proces%d VEKTOR=[", rank);
	for (int i = 0;i < k;i++)
		printf("%d ", subVector[i]);
	printf("]\n");*/
	int* Cparc = (int*)malloc(k*sizeof(int));
	for (int i = 0;i < k;i++)
	{
		Cparc[i] = 0;
		for (int j = 0;j < k;j++)
		{
			Cparc[i] += subMatrix[i * k + j] * subVector[j];
		}

	}
	int* C = (int*)malloc(k*size* sizeof(int));
	MPI_Gather(Cparc, k, MPI_INT, C, k, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("Proces%d stampa rezultat mnozenja matrice i vektora koji iznosi:[", rank);
		for (int i = 0;i < k*size;i++)
		{
				printf("%d ",C[i]);

			
		}
		printf("]\n");
	}
	MPI_Finalize();
	return 0;
}

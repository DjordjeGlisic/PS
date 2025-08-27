#include<stdio.h>
#include<mpi.h>
#include<malloc.h>
#define N 6

int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm komunikator;
	int fleg;
	if (rank==0||rank == 1 || rank == 3 || rank == 5)
	{
		fleg = 0;
	}
	else
	{
		fleg = 1;

	}
	MPI_Comm_split(MPI_COMM_WORLD, fleg, 0, &komunikator);
	int* matrica = nullptr;
	int* parcijalni = nullptr;
	if (rank == 0)
	{
		matrica = (int*)malloc(N * N * sizeof(int));
		int br = 1;
		for (int i = 0;i < N;i++)
		{
			for (int j = 0;j < N;j++)
			{
				matrica[i * N + j] = br++;

			}
		}
	}
	else
	{
		parcijalni = (int*)malloc(15 * sizeof(int));
	}
	MPI_Datatype tip;
	int blockLens[] = { 2,1,2,1,2, 1,2,1,1,1,1 };
	int kolko = 11;

	int disLens[] = { 1,4,8,11,15,18,22,25,29,30,32 };
	MPI_Type_indexed(kolko, blockLens, disLens, MPI_INT, &tip);
	MPI_Type_commit(&tip);
	int newSize;
	int newRank;
	
	if (rank == 0)
	{
		MPI_Comm_size(komunikator, &newSize);
		/*for (int i = 1;i < newSize;i++)
		{
			MPI_Send(matrica, 1, tip, i, i, komunikator);
		}*/
		MPI_Bcast(matrica, 1, tip, 0, komunikator);
	}
	else if(fleg==0)
	{
		/*MPI_Comm_rank(komunikator, &newRank);
		MPI_Recv(parcijalni, 15, MPI_INT, 0, newRank, komunikator, MPI_STATUSES_IGNORE);
		*/
		MPI_Bcast(parcijalni, 15, MPI_INT, 0, komunikator);
		printf("Proces%d je dobio vektor=[",rank);
		for (int i = 0;i < 15;i++)
			printf("%d,", parcijalni[i]);
		printf("]\n");
	
	}
	MPI_Finalize();
}
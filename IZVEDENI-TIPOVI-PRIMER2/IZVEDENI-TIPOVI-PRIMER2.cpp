#include<stdio.h>
#include<mpi.h>
#include <malloc.h>
#define N 16
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Datatype index;
	int blen[] = {1,3,4};
	int disp[] = { 2,5,12 };
	int count = 3;
	MPI_Type_indexed(count, blen, disp, MPI_FLOAT, &index);
	MPI_Type_commit(&index);
	if (rank == 0)
	{
		float niz[] = {1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f,10.0f,11.0f,12.0f,13.0f,14.0f,15.0f,16.0f };
		for (int i = 0;i < 2;i++)
			MPI_Send(niz, 1, index, i + 1, i + 1, MPI_COMM_WORLD);
	}
	else if (rank == 1)
	{
		float prijemFloat[100];
		MPI_Recv(prijemFloat, 8, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		printf("Proces 1 stampa:[");
		for (int i = 0;i < 8;i++)
			printf("%lf ", prijemFloat[i]);
		printf("]\n");
	}
	else
	{
		float prijemIzvedeni[100];
		MPI_Recv(prijemIzvedeni, 16, index, 0, 2, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		printf("Proces 2 stampa:[");
		for (int i = 0;i < 16;i++)
			printf("%lf ", prijemIzvedeni[i]);
		printf("]\n");
	}
	MPI_Finalize();
	return 0;
}
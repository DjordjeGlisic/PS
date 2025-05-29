#include<stdio.h>
#include<mpi.h>
#include <malloc.h>
#include<math.h>
#define N 10
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* skup = nullptr;
	if (rank == 0)
	{
		 skup = (int*)malloc(N * sizeof(int));
		for (int i = 0;i < N;i++)
			skup[i] = i+3;

	}
	int n = N / size;
	int* podskup = (int*)malloc(n * sizeof(int));
	MPI_Scatter(skup, n, MPI_INT, podskup, n, MPI_INT, 0, MPI_COMM_WORLD);
	int sum = 0;
	for (int i = 0;i < n;i++)
		sum += podskup[i];
	int ukSum;
	MPI_Reduce(&sum, &ukSum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Bcast(&ukSum, 1, MPI_INT, 0, MPI_COMM_WORLD);
	float avg = (float)ukSum / (float)N;
	float Sum = 0;
	float UkSum;
	for (int i = 0;i < n;i++)
	{
		Sum += pow((podskup[i] - avg), 2);
	}

	MPI_Reduce(&Sum, &UkSum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		float s = UkSum / (N - 1);
		s = sqrt(s);
		printf("Proces%d stampa standardnu devijaciju s=%lf\n",rank, s);

	}
	MPI_Finalize();
	return 0;
}
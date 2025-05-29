#include<mpi.h>
#include<stdio.h>
#include <malloc.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int n = 10;
	float* a = (float*)malloc(n * sizeof(float));
	float* b = (float*)malloc(n * sizeof(float));
	
	if (rank == 0)
	{
		for (int i = 0;i < n;i++)
			a[i] = (float)i;
		int j = 0;
		for (int i = n;i < (2 * n);i++)
		{
			b[j] = (float)i;
			j++;
		}

	}
	float segmentA[100];
	float segmentB[100];
	int s = n / size;
	MPI_Scatter(a, s, MPI_FLOAT, segmentA, s, MPI_FLOAT, 0, MPI_COMM_WORLD);
	MPI_Scatter(b, s, MPI_FLOAT, segmentB, s, MPI_FLOAT, 0, MPI_COMM_WORLD);
	float sum = 0.0f;
	for (int i = 0;i < s;i++)
	{
		printf("Proces %d mnozi a[%d]=%f i b[%d]=%f\n", rank,i, segmentA[i], i, segmentB[i]);
		sum += segmentA[i] * segmentB[i];
	}
	float res;
	MPI_Reduce(&sum, &res, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
		printf("Suma pokomponentnog mnozenja dva vektora je %f\n", res);

	MPI_Finalize();
	return 0;
}
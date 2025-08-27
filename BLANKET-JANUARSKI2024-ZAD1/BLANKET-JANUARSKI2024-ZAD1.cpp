#include<stdio.h>
#include<mpi.h>
#include<malloc.h>
#define N 10
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	float* nizA = nullptr;
	int paramC;
	if (rank == 0)
	{
		nizA = (float*)malloc(N * sizeof(float));
		float br = 1;
		for (int i = 0;i < N;i++)
			nizA[i] = br++;
		paramC = 30;
	}
	float* parcA = (float*)malloc((N / size) * sizeof(float));
	int kol = N / size;
	MPI_Scatter(nizA, kol, MPI_FLOAT, parcA, kol, MPI_FLOAT, 0, MPI_COMM_WORLD);
	float avg;
	float sum = 0;
	for (int i = 0;i < kol;i++)
	{

		sum += parcA[i];
	}
	MPI_Reduce(&sum, &avg, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("Suma svih elemenata niza je %lf\n", avg);
		avg = (float)avg / (float)N;
	}
	MPI_Bcast(&avg, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
	float s = 0;
	float max = -1;
	for (int i = 0;i < kol;i++)
	{
		if (parcA[i] > max)
			max = parcA[i];
		s += parcA[i] + avg;
	}
	struct ValId {
		float val;
		int id;
	};
	struct ValId slanje, prijem;
	slanje.id = rank;
	slanje.val = max;
	MPI_Reduce(&slanje, &prijem, 1, MPI_FLOAT_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&prijem, 1, MPI_FLOAT_INT, 0, MPI_COMM_WORLD);
	if (rank == 0)
		MPI_Send(&paramC, 1, MPI_INT, prijem.id, prijem.id, MPI_COMM_WORLD);
	else if (rank == prijem.id)
		MPI_Recv(&paramC, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	float brojilac;
	MPI_Reduce(&s, &brojilac, 1, MPI_FLOAT, MPI_SUM, prijem.id,MPI_COMM_WORLD);
	if (rank == prijem.id)
	{
		printf("Rezultat M ce stampati onaj proces kojiima max vrednost niza a max vrednost je %lf a to je proces%d\n", prijem.val, rank);
		float M = brojilac / (float)paramC;
		printf("M=%lf", M);

	}
	

	MPI_Finalize();
}

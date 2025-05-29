#include<mpi.h>
#include<stdio.h>
#define a 3
#define b 31
#define x 5
struct ValRank {
	double val;
	int rang;
};
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int niz[10];
	struct ValRank minLocBroj, minGlobBroj, minLocUkupno, minGlobUkupno;
	minLocBroj.rang = rank;
	minLocUkupno.rang = rank;
	minLocBroj.val = (double)INT64_MAX;
	minLocUkupno.val = 0.0f;
	double broj = 3 + 2 * rank;
	for (int i = 0;i < size;i++)
	{
		if (broj > 31)
			break;
		niz[i] = broj;
		if (niz[i] % 2 != 0 && niz[i] % x == 0)
		{
			if (niz[i] < minLocBroj.val)
				minLocBroj.val = (double)niz[i];
			minLocUkupno.val++;
		}

		broj += 2*size;


	}
	MPI_Reduce(&minLocUkupno, &minGlobUkupno, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&minGlobUkupno, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
	MPI_Reduce(&minLocBroj, &minGlobBroj, 1, MPI_DOUBLE_INT, MPI_MINLOC, minGlobUkupno.rang, MPI_COMM_WORLD);
	if (rank == minGlobUkupno.rang)
	{
		printf("Proces %d ima najmanji broj neparnih deljivih sa x i ima ih %lf, proces %d ima najmanji neparan deljiv sa x i to je broj %lf\n", rank, minGlobUkupno.val, minGlobBroj.rang, minGlobBroj.val);

	}


	MPI_Finalize();

	return 0;
}
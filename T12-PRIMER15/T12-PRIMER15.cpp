#include<stdio.h>
#include<mpi.h>
struct ValProc
{
	double val;
	int rank;
};
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	struct ValProc niz[30];
	for (int i = 0;i < 30;i++)
	{
		niz[i].rank = rank;
		niz[i].val = rank + i;
	}
	struct ValProc res[30];
	MPI_Reduce(niz, res, 30, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("Proces %d je primio podatke od ostalih procesa i pokomponentno nasao maksimum na svakom primljenom elementu\n",rank);
		printf("[");
		for (int i = 0;i < 30;i++)
			printf("{ID=%d,VAL=%lf},", res[i].rank, res[i].val);
	}


	MPI_Finalize();
	return 0;
}
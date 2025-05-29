#include<mpi.h>
#include<stdio.h>
struct ProcValue {
	double value;
	int rank;
};
int main(int argc, char* argv[])
{
	int rank, size;
	double niz[] = { 7, 1, 9, 25, 41, 32, 33, 77, 21, 0, 11, 18, 20, 25, 31 };
	struct ProcValue par,res;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);	
	par.value = (double)niz[rank];
	par.rank = rank;
	printf("%d %lf\n",par.rank, par.value);
	MPI_Reduce(&par, &res, 1, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("Proces %d sadrzi najvecu vrednost svoje lokalne promenljive a to je vrednsot %lf\n", res.rank, res.value);
	}
	MPI_Finalize();
	return 0;
}

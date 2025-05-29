#include<stdio.h>
#include<mpi.h>
#define N 5
#define M 4
int main(int argc, char* argv[])
{
	int rank, size, rankRow, rankCol;
	MPI_Init(&argc, &argv);
	MPI_Comm vrsta, kolona;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int colVrsta = rank / M;
	int colKolona = rank % M;
	MPI_Comm_split(MPI_COMM_WORLD, colVrsta, 0, &vrsta);
	MPI_Comm_split(MPI_COMM_WORLD, colKolona, 0, &kolona);
	MPI_Comm_rank(vrsta, &rankRow);
	MPI_Comm_rank(kolona, &rankCol);
	int n, m;
	MPI_Comm_size(vrsta, &n);
	MPI_Comm_size(kolona, &m);

	printf("Proces%d sada predstavlja Proces%d u komunikatoru vrste sa %d procesa i Proces%d u komunikatoru kolone sa %d procesa\n",rank,rankRow,n,rankCol,m);

	return 0;
}

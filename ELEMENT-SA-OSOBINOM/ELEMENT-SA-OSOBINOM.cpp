#include<stdio.h>
#include<mpi.h>

#include <corecrt_malloc.h>
#define a 3
#define b 31
#define x 5
struct brojId {
	double broj;
	int rang;
};

	int main(int argc, char* argv[])
{
	int rank, size;
	int* podaci;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	podaci = (int*)malloc(size * sizeof(int));
	podaci[0] = a + rank * 2;
	int broj = 0;
	int min = 100000;
	if (podaci[0] % x == 0)
	{
		broj++;
		min = podaci[0];
	}
	for (int i = 1;i < size;i++)
	{
		int res = podaci[i - 1] + 2 * size;
		if (res > b)
			break;
		podaci[i] = res;
		if (podaci[i] % x == 0)
		{
			broj++;
			if (podaci[i] < min)
				min = podaci[i];
		}

	}
	struct brojId procNum,minimalni,pod,minPod;
	procNum.rang = rank;
	procNum.broj = (double)broj;
	pod.rang = rank;
	pod.broj = (double)min;
	MPI_Reduce(&procNum, &minimalni, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&minimalni, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
	int suma;
	MPI_Reduce(&broj, &suma, 1, MPI_INT, MPI_SUM, minimalni.rang, MPI_COMM_WORLD);
	int najmanji;
	MPI_Reduce(&pod, &minPod, 1, MPI_DOUBLE_INT, MPI_MINLOC, minimalni.rang, MPI_COMM_WORLD);
	if(rank==minimalni.rang)
		printf("najmanji broj nepraran deljiv sa x je %d i on se nalazi u procesu %d, broj brojeva sa zadatom osobinom je %d, id procesa koji prikazuje rezultate %d", (int)minPod.broj,minPod.rang, suma, minimalni.rang);



		
	MPI_Finalize();
	return 0;
}

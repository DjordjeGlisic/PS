#include<mpi.h>
#include<stdio.h>
#include<malloc.h>
#define M 3
#define N 4
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int novi;
	MPI_Comm RESETKA;
	int ndims[] = { M,N };
	int records[] = { 0,0 };
	MPI_Cart_create(MPI_COMM_WORLD, 2, ndims, records, 0, &RESETKA);
	MPI_Comm_rank(RESETKA, &novi);
	int* niz=(int*)malloc(3 * size * sizeof(int));
	int* parc = (int*)malloc(3 * sizeof(int));
	int koords[] = { -1,-1 };
	MPI_Cart_coords(RESETKA, novi,2, koords);
	parc[0] = koords[0];
	parc[1] = koords[1];
	parc[2] = koords[0] + koords[1];
	
	MPI_Datatype tip;
	MPI_Type_vector(3, 1, size, MPI_INT, &tip);
	MPI_Type_commit(&tip);
	MPI_Datatype izv;
	MPI_Type_create_resized(tip, 0, sizeof(int), &izv);
	MPI_Type_commit(&izv);
	MPI_Gather(parc, 3, MPI_INT, niz, 1, izv, 0, MPI_COMM_WORLD);
	MPI_Bcast(niz, 3 * size, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("NIZ=[");
		for (int i = 0;i < 3 * size;i++)
			printf(" %d ", niz[i]);
		printf("]\n");
	}
	if (koords[1] % 2 == 0)
	{
		printf("Proces%d {%d,%d} stampa svoj niz parametara pre razmene niz=[",novi,koords[0],koords[1]);
		for (int i = 0;i < 3;i++)
			printf(" %d ", niz[rank+i*size]);
		printf("]\n");
	}
	if (koords[1] == 0 || koords[1] == 1)
	{
		int salji;
		int nk[] = { koords[0],koords[1] + 2 };
		MPI_Cart_rank(RESETKA, nk, &salji);
		MPI_Send(&niz[rank], 1, tip, salji, salji, RESETKA);
	}
	else
	{
		int prijem;
		int nkp[] = { koords[0],koords[1] - 2 };
		MPI_Cart_rank(RESETKA, nkp, &prijem);
		MPI_Recv(parc, 3, MPI_INT, prijem, novi, RESETKA, MPI_STATUSES_IGNORE);
		if (koords[1] % 2 == 0)
		{
			printf("Proces%d {%d,%d} stampa svoj niz parametara nakon prijema niz=[",novi, koords[0], koords[1]);
			for (int i = 0;i < 3;i++)
				printf(" %d ", parc[i]);
			printf("]\n");
		}
		niz[rank] = parc[0]+koords[0];
		niz[rank + size] = parc[1]+koords[1]-1;
		niz[rank + 2 * size] = parc[2] + koords[0] + koords[1]-1;
		if (koords[1] % 2 == 0)
		{
			printf("Proces%d {%d,%d} stampa svoj niz parametara pre slanja niz=[",novi, koords[0], koords[1]);
			for (int i = 0;i < 3;i++)
				printf(" %d ", niz[rank+i*size]);
			printf("]\n");
		}
		if (koords[1] != N - 1 && koords[1] != N - 2)
		{
			int prema;
			int nks[] = { koords[0],koords[1] + 2 };
			MPI_Cart_rank(RESETKA, nks, &prema);
			MPI_Send(&niz[rank], 1, tip, prema, prema, RESETKA);
		}


	}


	MPI_Finalize();
	return 0;
}
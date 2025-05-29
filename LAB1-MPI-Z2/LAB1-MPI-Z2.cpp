#include<stdio.h>
#include<mpi.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	float niz[100];
	int kol;
	if (rank == 0)
	{
		int n;
		l:
		//printf("Uneiste broj elemenata niza\n");
		//scanf("%d", &n);
		n = 3 * size;
		/*if (n % size != 0)
		{
			printf("N nije deljiv sa brojem procesa");
			goto l;
		}*/
		for (int i = 0;i < n;i++)
		{
			printf("niz[%d]=%d\n ", i,i);
			/*scanf("%d", &niz[i]);
			printf("\n");*/
			niz[i] = i;
		}

		kol = n / size;
	}
	MPI_Bcast(&kol, 1, MPI_INT, 0, MPI_COMM_WORLD);
	float buff[100];
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Scatter(niz, kol, MPI_FLOAT, buff,kol, MPI_FLOAT, 0, MPI_COMM_WORLD);
	float min = 10000000.0f;
	for (int i = 0;i < kol;i++)
	{
		printf("Proces %d niz[%d]=%f\n", rank, i, buff[i]);
		if (buff[i] < min)
			min = buff[i];

	}
	printf("Proces %d je iz svog niza pronasao najmanji element:%f\n",rank, min);
	int minBuff[100];
	MPI_Gather(&min, 1, MPI_FLOAT, minBuff,1, MPI_FLOAT, 0, MPI_COMM_WORLD);
	if (rank == 0)
	{
		min = 100000.0f;
		for (int i = 0;i < size;i++)
			if (minBuff[i] < min)
				min = minBuff[i];
		printf("Najmanji od svih elemenata je %f\n", min);
	}


	MPI_Finalize();
	return 0;
}
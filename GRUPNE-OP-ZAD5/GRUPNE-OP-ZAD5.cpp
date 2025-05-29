#include<stdio.h>
#include<mpi.h>
#include <corecrt_malloc.h>
#define n 3
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* matrica = nullptr;
	int* kolonaMatrice = nullptr;
	int* vektor=nullptr;
	int el;
	if (rank == 0)
	{
		matrica = (int*)malloc(n * size * sizeof(int));
		vektor = (int*)malloc(size * sizeof(int));
		//vektor = { 0,5,10,15 };
		int broj = 1;
		for (int i = 0;i < n;i++)
		{
			
			for (int j = 0;j < size;j++)
			{
				if (i == 0)
				{
					vektor[j] = j * 5;

				}
				matrica[i*size+j] = broj;
				broj++;
			}
		}
	}
	 kolonaMatrice= (int*)malloc(n * sizeof(int));
	 if (rank == 0)
	 {
		 for (int j = 0;j < n;j++)
		 {
			 kolonaMatrice[j] = matrica[j * size + 0];
		 }
		 for (int i = 1;i < size;i++)
		 {
			 int buff[100];
			 for (int j = 0;j < n;j++)
			 {
				 buff[j] = matrica[j * size + i];
			 }
			 MPI_Send(buff, n, MPI_INT, i, i, MPI_COMM_WORLD);

		 }
	 }
	 else
		 MPI_Recv(kolonaMatrice, n, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	 MPI_Scatter(vektor, 1, MPI_INT, &el, 1, MPI_INT, 0, MPI_COMM_WORLD);
	 struct MinVal { double val;int rank; }locMin,globMin;
	 locMin.rank = rank;
	 locMin.val = (double)INT64_MAX;
	 printf("P%d el=%d kol:[", rank,el);
	 for (int i = 0;i < n;i++)
	 {
		 printf("%d-", kolonaMatrice[i]);
		 if ((double)kolonaMatrice[i] < locMin.val)
			 locMin.val = (double)kolonaMatrice[i];

		 kolonaMatrice[i] = kolonaMatrice[i] * el;
		 printf("%d ", kolonaMatrice[i]);

	 }
	 printf("]\n");
	 MPI_Reduce(&locMin, &globMin, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	 MPI_Bcast(&globMin, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
	 int* rez=(int*)malloc(size*sizeof(int));
	 MPI_Barrier(MPI_COMM_WORLD);
	 MPI_Reduce(kolonaMatrice, rez, n, MPI_INT, MPI_SUM, globMin.rank, MPI_COMM_WORLD);
	 if (rank == globMin.rank)
	 {
		printf("Proces sa min elementom je %d i on stampa rezultujuci vektor:[", rank);
		for (int i = 0;i < n;i++)
			printf("%d ", rez[i]);
		printf("]\n");

	 }
	MPI_Finalize();

	return 0;
}
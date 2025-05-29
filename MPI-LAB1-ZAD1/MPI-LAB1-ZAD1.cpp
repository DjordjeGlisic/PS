#include<stdio.h>
#include<mpi.h>
#include <malloc.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
	{
		int* niz = (int*)malloc(2 * size * sizeof(int));
		int ids;
		int brojac = 1;
		for (int i = 0;i < 2 * size;i++)
		{
			niz[i] = i + 2;
			if (i % 2 != 0)
			{
				if (brojac < size) {
					MPI_Send(&niz[i - 1], 1, MPI_INT, brojac, 1, MPI_COMM_WORLD);
					MPI_Send(&niz[i], 1, MPI_INT, brojac, 2, MPI_COMM_WORLD);
					MPI_Recv(&ids, 1, MPI_INT, brojac, 3, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
					printf("Proces %d je je prihvatio ID=%d\n", rank, ids);
					brojac++;
				}
			}
			
		}
	}
	else
	{
		int a, b;
		MPI_Recv(&a, 1, MPI_INT, 0, 2, MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
		printf("Proces %d prima prvu poruku od master procesa sa sadrzajem %d\n", rank, a);
		MPI_Recv(&b, 1, MPI_INT, 0, 1, MPI_COMM_WORLD,MPI_STATUSES_IGNORE);
		printf("Proces %d prima drugu poruku od master procesa sa sadrzajem %d\n", rank, b);
		MPI_Send(&rank, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);


	}
	MPI_Finalize();

	return 0;
}
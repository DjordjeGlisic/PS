#include<stdio.h>
#include<mpi.h>
#include<math.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int sum=rank+1;
	int moduo;
	bool poslao = false;
	MPI_Status status;
	for (int i = 0;i < log2(size);i++)
	{
		moduo =pow(2,i+1);
		if (rank % moduo == 0)
		{
			int prijem;
			MPI_Recv(&prijem, 1, MPI_INT, rank+pow(2, i),0, MPI_COMM_WORLD, &status);
			sum += prijem;
			printf("Proces %d je prihvatio podatak %d od procesa %d i kreirao parcijalnu sumu %d\n", rank, prijem, rank + pow(2, i), sum);
		}
		else
		{
			if (poslao == false)
			{
				poslao = true;
				MPI_Send(&sum, 1, MPI_INT, rank - pow(2, i), 0, MPI_COMM_WORLD);
			}

		}



	}
	if (rank == 0)
	{
		printf("Konacna suma je %d i stampa je proces %d\n", sum,rank);
	}
	

	MPI_Finalize();
	return 0;
}
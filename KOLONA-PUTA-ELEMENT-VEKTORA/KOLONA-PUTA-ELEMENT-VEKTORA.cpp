#include<mpi.h>
#include<stdio.h>
#include <corecrt_malloc.h>
#define n 3
#define m 3
struct ProcVal
{
	double val;
	int rank;
};
int main(int argc, char* argv[])
{
	int rank, size;
	int* mat;
	int* vektor=nullptr;
	int* kolona=nullptr;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
	{
		mat = (int*)malloc(n *m* sizeof(int));
		vektor = (int*)malloc(n * sizeof(int));
		int br = 1;
		int* prosledi = (int*)malloc(m * sizeof(int));

		for (int i = 0;i < n;i++)
		{
			vektor[i] = i;
			if (i == 0)
			{
				kolona = (int*)malloc(m * sizeof(int));
			}
			for (int j = 0;j < m;j++)
			{
				mat[i * m + j] = br;
				br++;
			
			}
			
			
		}
		
		for (int j = 0;j < m;j++)
		{
			for (int i = 0;i < n;i++)
			{
				if (j == 0)
				{
					kolona[i] = mat[i * m + j];
				}
				else
					prosledi[i] = mat[i * m + j];
			}
			if (j != 0)
				MPI_Send(prosledi, n, MPI_INT, j, j, MPI_COMM_WORLD);

		}
		

		/*for (int i = 0;i < n;i++)
		{
			printf("[");
			for (int j = 0;j < m;j++)
				printf("%d ", mat[i * m + j]);
			printf("]\n");
		
		}*/
	}
	else
	{
		kolona = (int*)malloc(n * sizeof(int));
		MPI_Recv(kolona, n, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		

	}
	/*printf("Proces %d je dobio sledecu kolonu [", rank);
	for (int i = 0;i < n;i++)
		printf("%d", kolona[i]);
	printf("]");
	*/
	int element;
	MPI_Scatter(vektor, 1, MPI_INT, &element, 1, MPI_INT, 0, MPI_COMM_WORLD);
	printf("Proces %d je dobio element vektora %d\n", rank, element);
	struct ProcVal lokalniMinimum,globalniMinimum;
	lokalniMinimum.rank = rank;
	lokalniMinimum.val = 10000000.0f;
	//printf("Proces %d kreira ove elemente:[",rank);
	for (int i = 0;i < n;i++)
	{
		if (kolona[i] < lokalniMinimum.val)
			lokalniMinimum.val = (double)kolona[i];
		kolona[i] = kolona[i] * element;
		//printf("%d ", kolona[i]);
	}
	//printf("]\n");
	MPI_Reduce(&lokalniMinimum, &globalniMinimum, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&globalniMinimum, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
	//int* akumulatorKolona = nullptr;
	//akumulatorKolona=(int*)malloc(n * m * sizeof(int));
	//MPI_Gather(kolona, n, MPI_INT, akumulatorKolona, n, MPI_INT, globalniMinimum.rank, MPI_COMM_WORLD);
	int* rez = (int*)malloc(n * sizeof(int));
	//for (int i = 0;i < n;i++)
	//{
		
		MPI_Reduce(kolona, rez, n, MPI_INT, MPI_SUM, globalniMinimum.rank, MPI_COMM_WORLD);

	//}
	if (rank == globalniMinimum.rank)
	{
		/*int* rezVektor = (int*)malloc(n * sizeof(int));
		int ind = 0;
		for (int j = 0;j < n;j++)
		{
			int sum = 0;
			for (int i = 0;i < m;i++)
			{
				
				sum += akumulatorKolona[i * n + j];

			}
			rezVektor[ind] = sum;
			ind++;
		}*/
		
		printf("Proces koji posudeuje najmanji element matrice stampa pomnozenu matricu sa vektorom i to je proces %d\n", rank);
		printf("[");
		for (int i = 0;i < n;i++)
		{
			
				printf("%d ",rez[i]);
			
		}
			printf("]\n");

	}


	MPI_Finalize();
	return 0;
}
#include<stdio.h>
#include<mpi.h>
#include<malloc.h>
#define K 3
#define M 9
#define N 3
int main(int argc, char* argv[])
{
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* matA = nullptr;
	int* matB = (int*)malloc(M * N * sizeof(int));
	if (rank == 0)
	{
		int br = 0;
		int niz1[] = { 0,1,1 ,0,1,0 ,1,0,0 ,1,0,0 ,1,0,0, 1,0,1, 0,1,0, 1,0,1, 0,1,1 };
		matA = (int*)malloc(K * M * sizeof(int));
		for (int i = 0;i < K;i++)
		{
			for (int j = 0;j < M;j++)
				matA[i * M + j] = niz1[br++];
		}
		br = 0;
		int niz2[] = { 1,0,1 ,0,1,0,1,1,1,0,1,1,1,1,0,0,0,1,1,0,1,0,0,0,0,1,1 };
		for (int i = 0;i < M;i++)
		{
			for (int j = 0;j < N;j++)
				matB[i * N + j] = niz2[br++];
		}
	}
	int brojKolona = M / size;
	int* koloneA = (int*)malloc(K * brojKolona * sizeof(int));
	//POINT TO POINT //////////////////////////////////
	if (rank == 0)
	{
		MPI_Datatype tip;
		MPI_Type_vector(K, 1, M, MPI_INT, &tip);
		MPI_Type_commit(&tip);
		int index = 0;
		for (int j = 0;j < M;j+=size)
		{
			for (int i = 0;i < K;i++)
			{
				koloneA[index++] = matA[i * M + j];
			}
		}
		int* pom = (int*)malloc(K * brojKolona * sizeof(int));
		for (int i = 1;i < size;i++)
		{
			index = 0;
			for (int j = i;j < M;j += size)
			{
				for (int z = 0;z < K;z++)
				{
					pom[index++] = matA[z * M + j];
				}
			}

			MPI_Send(pom, K * brojKolona, MPI_INT, i, i, MPI_COMM_WORLD);
		}

	}
	else
	{
		MPI_Recv(koloneA, K * brojKolona, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	}
	////////////////////////////////////////////////////////////////////////////

	printf("Proces%d dobija vektor=[",rank);
	for (int i = 0;i < brojKolona * K;i++)
		printf("%d,", koloneA[i]);
	printf("]\n");
	MPI_Bcast(matB, M * N, MPI_INT, 0, MPI_COMM_WORLD);
	struct ValId {
		int val;
		int id;
	};
	struct ValId str,gl;
	str.val = 1000000;
	str.id = rank;
	
	for (int i = 0;i < brojKolona * K;i++)
	{
		if (koloneA[i] < str.val)
			str.val = koloneA[i];
	}
	MPI_Reduce(&str, &gl, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&gl, 1, MPI_2INT, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	int* parcRes = (int*)malloc(K * N * sizeof(int));
	int index = 0;
	//printf("Parcijalni rezulatat Procesa%d=[",rank);
	int* trans = (int*)malloc(K * brojKolona * sizeof(int));
	for (int i = 0;i < K;i++)
	{
		for (int j = 0;j < brojKolona;j++)
		{
			trans[index++] = koloneA[j * K + i];
		}
	}
	index = 0;
	for (int i = 0;i < K;i++)
	{
		for (int z = 0;z < N;z++)
		{
			int ind = 0;
			int j = rank;
			parcRes[index] = 0;
		
			while (ind < brojKolona)
			{
				parcRes[index] += trans[i * brojKolona + ind] * matB[j * N + z];

				j += size;
				ind++;
			}
			index++;

		}
	}
	//printf("]\n");
	int* matC = nullptr;
	if (rank == gl.id)
		matC = (int*)malloc(K * N * sizeof(int));
	MPI_Reduce(parcRes, matC, K* N, MPI_INT, MPI_SUM, gl.id, MPI_COMM_WORLD);
	if (rank == gl.id)
	{
		printf("Rezultat mnozenja matrice A i matrice B:\n");
		for (int i = 0;i < K;i++)
		{
			printf("[");
			for (int j = 0;j < N;j++)
			{
				printf("%d ", matC[i * N + j]);
			}
			printf("]\n");
		}
	}
	MPI_Finalize();

}
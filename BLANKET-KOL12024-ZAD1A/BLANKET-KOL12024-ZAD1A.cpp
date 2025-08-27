#include<stdio.h>
#include<mpi.h>
#include<malloc.h>
#define N 3
#define K 4
#define M 8
int main(int argc, char* argv[])
{
	int* matA;
	int* matB;
	int* matC;
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	matA = (int*)malloc(N * K * sizeof(int));
	matB = nullptr;
	int brojKolona = M / size;
	int* kolonaB = (int*)malloc(brojKolona * K * sizeof(int));
	if (rank == 0)
	{
		int br[] = { 1,2,3,4,5,6,7,8,9,10,11,12 };
		for (int i = 0;i < N;i++)
		{
			for (int j = 0;j < K;j++)
				matA[i * K + j] = br[i * K + j];
		}
		int br2[] = { 0,0,1,1,0,0,1,0, 1,1,0,0,1,0,1,1, 0,1,0,1,0,0,1,0, 1,0,1,0,1,0,1,0 };
		matB = (int*)malloc(K * M * sizeof(int));
		for (int i = 0;i < K;i++)
		{
			for (int j = 0;j < M;j++)
				matB[i * M + j] = br2[i * M + j];
		}
		
	}
	
	MPI_Bcast(matA, N * K, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Datatype tip;
	MPI_Type_vector(K, brojKolona, M, MPI_INT, &tip);
	MPI_Type_commit(&tip);
	MPI_Datatype novi;
	MPI_Type_create_resized(tip, 0, brojKolona * sizeof(int), &novi);
	MPI_Type_commit(&novi);
	//POD B
	if (rank == 0)
	{
		
		int pomInd = 0;
		printf("Vektor B dobija deo matrice B[");
		for (int i = 0;i < K;i++)
		{
			for (int j = 0;j < brojKolona;j++)
			{

				kolonaB[pomInd] = matB[i * M + j];
				printf("%d,", kolonaB[pomInd]);
				pomInd++;
			}

		}

		for (int i = 1;i < size;i++)
		{

			MPI_Send(&matB[i*brojKolona], 1, tip, i, i, MPI_COMM_WORLD);

		}
	}
	else
	{
		MPI_Recv(kolonaB, K * brojKolona, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	}
	//pod A
	 // MPI_Scatter(matB, 1, novi, kolonaB, brojKolona * K, MPI_INT, 0, MPI_COMM_WORLD);

	/*printf("Proces%d vektor=[",rank);
	for (int i = 0;i < brojKolona * K;i++)
		printf("%d,", kolonaB[i]);
	printf("]\n");*/
	int max = -1;
	for (int i = 0;i < brojKolona * K;i++)
		if (kolonaB[i] > max)
			max = kolonaB[i];
	struct IdVal {
		int val;
		int id;
	};
	struct IdVal str, global;
	str.id = rank;
	str.val = max;
	MPI_Reduce(&str, &global, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	int gl;
	if (rank == 0)
		gl = global.id;
	MPI_Bcast(&gl, 1, MPI_INT, 0, MPI_COMM_WORLD);
	/*if (rank == gl)
		printf("Najveci element matrice B je %d i stampa ga proces %d\n", str.val, rank);*/
	int* kolC = (int*)malloc(N * brojKolona * sizeof(int));
	int skok = 0;
	//printf("Proces%d menja Vektor B sad je B=[",rank);
	skok = 0;
	int* linear = (int*)malloc(brojKolona * K * sizeof(int));
	int index = 0;
	for (int i = 0;i < brojKolona;i++)
	{
		int j = skok;
		int count = 0;

		while (count < K)
		{
			linear[index] = kolonaB[j];
			//printf("%d,", linear[index]);
			index++;
			j += brojKolona;
			count++;
		}
		skok++;

	}
	//printf("]\n");
	//printf("Proces%d parcijalni proizvod matrice=[", rank);
	index = 0;
	for (int i = 0;i < brojKolona;i++)
	{
		for (int j = 0;j < N;j++)
		{
			kolC[index] = 0;
			for (int z = 0;z < K;z++)
			{
				kolC[index] += matA[j * K + z] * linear[i * K + z];
			}
			//printf("%d,", kolC[index]);
			index++;
		}
	}
	//printf("]\n");

	matC = nullptr;
	if (rank == gl)
	{
		matC = (int*)malloc(N * M * sizeof(int));
	}

	MPI_Gather(kolC, N*brojKolona, MPI_INT, matC, N*brojKolona,MPI_INT,gl,MPI_COMM_WORLD );
	if (rank == gl)
	{
		/*printf("Proces%d stampa matricu C\n",rank);
		for (int i = 0;i < N;i++)
		{
			printf("[");
			for (int j = 0;j < M;j++)
			{
				printf("%d ", matC[i * M + j]);
			}
			printf("]\n");
		}*/
		int* konacna = (int*)malloc(N * M * sizeof(int));
		int ind = 0;
		for (int i = 0;i < N;i++)
		{
			printf("[");
			for (int j = 0;j < N * M;j += N)
			{
				konacna[ind] = matC[i+j];
				printf("%d ", konacna[ind]);
				ind++;
			}
			printf("]\n");
		}


	}
	MPI_Finalize();


}
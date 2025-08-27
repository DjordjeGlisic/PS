#include<stdio.h>
#include<mpi.h>
#include<math.h>
#include<malloc.h>
#include<string.h>
#define N 8
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* A = nullptr;
	int* B = nullptr;
	if (rank == 0)
	{
		int MatA[N][N] = {
			{0,1,0, 1,1,0, 1,0},
			{1,1,0, 0,1,0, 1,0},
			{1,0,0, 1,1,0, 0,0},
			{0,1,0, 0,0,1, 1,1},
			{0,0,1, 1,1,0, 0,1},
			{1,0,0, 0,1,0, 0,0},
			{0,0,0, 0,1,1, 1,1},
			{1,1,0,1,0,0,0,0}


		};
		int br = 1;
		A = (int*)malloc(N * N * sizeof(int));
		B = (int*)malloc(N * sizeof(int));
		for (int i = 0;i < N;i++)
		{
			B[i] = br++;
			for (int j = 0;j < N;j++)
			{
				A[i * N + j] = MatA[i][j];
			}
		}
	}
	MPI_Datatype tip;
	int q = sqrt(size);
	int kolko = N / q;
	MPI_Type_vector(kolko*kolko, 1, q, MPI_INT, &tip);
	MPI_Type_commit(&tip);
	int* parcA = (int*)malloc(kolko * kolko * sizeof(int));
	if (rank == 0)
	{
		for (int i = 0;i < kolko;i++)
		{
			for (int j = 0;j < kolko;j++)
			{
				parcA[i * kolko + j] = A[i * N + j];
			}
		}
		for (int i = 1;i < size;i++)
		{
			int vrsta = i / q;
			int kolona = i % q;
			int adresa = vrsta * kolko * N + kolona;
			MPI_Send(&A[adresa], 1, tip, i, i, MPI_COMM_WORLD);
		}
	}
	else
		MPI_Recv(parcA, kolko * kolko, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Comm vrsta, kolona;
	int vrs, kol;
	vrs = rank / q;
	kol = rank % q;
	MPI_Comm_split(MPI_COMM_WORLD, vrs, 0, &vrsta);
	int* parcB = (int*)malloc(kolko * sizeof(int));
	if (vrs == 0)
	{
		MPI_Scatter(B, kolko, MPI_INT, parcB, kolko, MPI_INT, 0, vrsta);
	}
	MPI_Comm_split(MPI_COMM_WORLD, kol, 0, &kolona);
	MPI_Bcast(parcB, kolko, MPI_INT, 0, kolona);
	/*printf("Pod vektor B procesa%d je [",rank);
	for (int i = 0;i < kolko;i++)
		printf(" %d ", parcB[i]);
	printf("]\n");*/
	struct ValRank {
		double val;
		int rank;
	} global,local;
	local.rank = rank;
	local.val = 100000000000000000.0f;
	//printf("Proces%d je primio pod matricu A\n",rank);
	for (int i = 0;i < kolko;i++)
	{
		//printf("[");
		for (int j = 0;j < kolko;j++)
		{
			if (local.val < parcA[i * kolko + j])
				local.val = parcA[i * kolko + j];
			//printf(" %d ", parcA[i * kolko + j]);
		}
		//printf("]\n");
	}
	int* parcC = (int*)malloc(kolko * sizeof(int));
	for (int j = 0;j < kolko;j++)
	{
		parcC[j] = 0;
		for (int z = 0;z < kolko;z++)
		{
			parcC[j] += parcA[j * kolko + z] * parcB[z];
		}
	}
	int* Ckol = (int*)malloc(kolko * q * sizeof(int));
	MPI_Gather(parcC, kolko, MPI_INT, Ckol, kolko, MPI_INT, 0, kolona);
	int* C = (int*)malloc(kolko * q * sizeof(int));

	if (vrs == 0)
	{
		MPI_Reduce(Ckol, C, kolko * q, MPI_INT, MPI_SUM, 0, vrsta);
	}
	MPI_Reduce(&local, &global, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&global, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(C, kolko* q, MPI_INT, 0, MPI_COMM_WORLD);
	if (rank == global.rank)
	{
		printf("Proces%d ima mininalni element matrice A stampa rez=[",rank);
		for (int i = 0;i < q*kolko;i++)
		{
			
				printf(" %d ", C[i]);
			
		}
		printf("]\n");
	}
	MPI_Comm dijagonala;
	int fleg = abs(kol - vrs);
	MPI_Comm_split(MPI_COMM_WORLD, fleg, 0, &dijagonala);
	int buff;
	if (rank == 0)
	{
		buff = 500;
		MPI_Bcast(&buff,1, MPI_INT, 0, dijagonala);
	
	}
	else if(fleg==0)
	{
		MPI_Bcast(&buff, 1, MPI_INT, 0, dijagonala);
		int newRank;
		MPI_Comm_rank(dijagonala, &newRank);
		printf("Proces%d u dijagonali proces%d je primio poruku %d\n", rank, newRank, buff);


	}

	MPI_Finalize();
}
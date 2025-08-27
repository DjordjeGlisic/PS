#include<mpi.h>
#include<stdio.h>
#include<malloc.h>
#include<math.h>
#define P 16
#define N 8
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int* A = nullptr;
	int* B = (int*)malloc(N * sizeof(int));
	if (rank == 0)
	{
		A = (int*)malloc(N * N * sizeof(int));
		
		int pomA[N][N] = {
			{0,1,1,0  ,1,1,0,0},
			{0,0,0,0  ,1,1,1,1},
			{1,0,1,0  ,0,1,0,1},
			{0,1,1,0  ,0,0,1,1},
			{1,1,0,0  ,1,1,0,0},
			{0,0,1,1  ,0,0,1,1},
			{1,0,0,1  ,0,1,1,0},
			{1,1,1,1  ,1,1,1,1}
		};
		int br = 1;
		for (int i = 0;i < N;i++)
		{
			for (int j = 0;j < N;j++)
			{
				if (i == 0)
				{
					B[j] = br++;
				}
				A[i * N + j] = pomA[i][j];
			}

		}
	}
	int q = sqrt(P);
	int kolko = N / q;
	int count = kolko * kolko;
	int* blens=(int*)malloc(kolko*kolko*sizeof(int));
	for (int i = 0;i < count;i++)
		blens[i] = 1;
	int* disps = (int*)malloc(kolko * kolko * sizeof(int));
	int iter = 0;
	int pomeraj = 0;
	for (int vrsta = 0;vrsta < kolko;vrsta++)
	{
		pomeraj = (vrsta*q)*N;
		for (int kolona = 0;kolona < kolko;kolona++)
		{
			disps[iter] = pomeraj;
			pomeraj += q;
			iter++;
		}
	}
	/*printf("Vektor rastojanja od pocetka Proces%d=[", rank);
	for (int i = 0;i < kolko * kolko;i++)
	{
		printf(" %d ", disps[i]);
	}*/
	/*printf("]\n");*/
	MPI_Datatype tip;
	MPI_Type_indexed(kolko*kolko, blens, disps, MPI_INT, &tip);
	MPI_Type_commit(&tip);

	int* parcA = (int*)malloc(kolko * kolko * sizeof(int));
	if (rank == 0)
	{
		for (int i = 0;i < kolko;i++)
		{
			for (int j = 0;j < kolko;j++)
			{
				parcA[i * kolko + j] = A[i *q*N + j * q];
			}
		}
		for (int i = 1;i < size;i++)
		{
			int start = (int)(i / q);
			start *= N;
			start += (int)i % q;
			/*for (int j = 0;j < kolko * kolko;j++)
			{
				printf("Proces%d prima %d\n", i, A[start+disps[j]]);
			}*/
			MPI_Send(&A[start], 1, tip, i, i, MPI_COMM_WORLD);
		}

	}
	else
	{
		MPI_Recv(parcA, kolko * kolko, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	}
	/*printf("Proces%d je dobio podmatricu A\n", rank);
	for (int i = 0;i < kolko;i++)
	{
		printf("[");
		for (int j = 0;j < kolko;j++)
		{
			printf(" %d ", parcA[i * kolko + j]);
		}
		printf("]\n");
	}*/
	MPI_Comm KOL;
	int col = rank % q;
	MPI_Comm_split(MPI_COMM_WORLD, col, 0, &KOL);
	int colRank;
	MPI_Comm_rank(KOL, &colRank);
	MPI_Comm ROW;
	int color = rank / q;
	MPI_Comm_split(MPI_COMM_WORLD, color, 0, &ROW);
	int rowRank;
	MPI_Comm_rank(ROW, &rowRank);


	MPI_Datatype t;
	MPI_Type_vector(kolko, 1, q, MPI_INT, &t);
	MPI_Type_commit(&t);
	MPI_Datatype izveden;
	MPI_Type_create_resized(t, 0, sizeof(int), &izveden);
	MPI_Type_commit(&izveden);

	int* parcB = (int*)malloc(kolko * sizeof(int));
	if (color == 0)
	{
		MPI_Scatter(B, 1, izveden,parcB,kolko,MPI_INT,0,ROW);

	}
	
	MPI_Bcast(parcB,kolko,MPI_INT,0,KOL);

	
	
	printf("Proces%d parcB=[",rank);
	for (int i = 0;i < kolko;i++)
		printf(" %d ", parcB[i]);
	printf("]\n");
	int* parcC = (int*)malloc(kolko * sizeof(int));
	for (int i = 0;i < kolko;i++)
	{
		parcC[i] = 0;
		for (int j = 0;j < kolko;j++)
		{
			parcC[i] += parcA[i * kolko + j] * parcB[j];
		}



	}
	
	int* kolC = (int*)malloc(kolko * q * sizeof(int));
	MPI_Gather(parcC, kolko, MPI_INT, kolC, kolko, MPI_INT, 0, KOL);
	int* C = nullptr;
	if (rank == 0)
		C = (int*)malloc(kolko * q * sizeof(int));
	

	if (colRank == 0)
	{
		MPI_Reduce(kolC, C, kolko * q, MPI_INT, MPI_SUM, 0, ROW);
	}
	/*if (rank == 0)
	{
		printf("Proces%d stampa proizovd matrice i vektora C=[", rank);
		for (int i = 0;i < kolko * q;i++)
			printf(" %d ", C[i]);
		printf("]\n");
	}*/


	MPI_Finalize();
	return 0;
}
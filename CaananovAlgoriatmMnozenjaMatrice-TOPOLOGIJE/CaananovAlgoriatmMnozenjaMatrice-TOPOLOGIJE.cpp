#include<stdio.h>
#include<mpi.h>
#include<math.h>
#include<malloc.h>
#define N 16
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int q = sqrt(N);
	int k = N / q;
	MPI_Comm kom;
	int dims = 2;
	int ndims[] = { q,q };
	int periods[] = { 1,1 };
	MPI_Cart_create(MPI_COMM_WORLD, dims, ndims, periods, 1, &kom);
	int newRank;
	MPI_Comm_rank(kom, &newRank);
	int* matA = nullptr;
	int* matB = nullptr;
	if (newRank == 0)
	{
		matA=(int*)malloc(k * k * sizeof(int));
		matB = (int*)malloc(k * k * sizeof(int));
		int brojac = 0;
		for (int i = 0;i < k;i++)
		{
			for (int j = 0;j < k;j++)
			{
				matA[i * k + j] = brojac;
				matB[i * k + j] = brojac;
				brojac++;

			}
		}
	}
	int aij;
	int bij;
	MPI_Scatter(matA, 1, MPI_INT, &aij, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(matB, 1, MPI_INT, &bij, 1, MPI_INT, 0, MPI_COMM_WORLD);
	int koords[] = { -1,-1 };
	MPI_Cart_coords(kom, newRank, 2, koords);
	//printf("Proces%d je dobio a%d%d=%d b%d%d=%d\n", newRank, koords[0], koords[1], aij, koords[0], koords[1], bij);
	int cij = 0;
	/*int saljiAprema[2];
	saljiAprema[1] = koords[1] - koords[0];
	saljiAprema[0] = koords[0];
	int saljiBprema[2];
	saljiBprema[0] = koords[0] - koords[1];
	saljiBprema[1] = koords[1];
	if (saljiBprema[0] < 0)
	{
		saljiBprema[0] = k + saljiBprema[0];
	}
	if (saljiAprema[1] < 0)
	{
		saljiAprema[1] = k + saljiAprema[1];
	}
	int rankZaSlanjeA,rankZaSlanjeB;
	MPI_Cart_rank(kom, saljiAprema, &rankZaSlanjeA);
	MPI_Cart_rank(kom, saljiBprema, &rankZaSlanjeB);
	int primiAod[2];
	int primiBod[2];
	primiBod[1] = koords[1];
	primiBod[0] = koords[0] + koords[1];
	if (primiBod[0] >= k)
		primiBod[0] -= k;
	primiAod[2];
	primiAod[0] = koords[0];
	primiAod[1] = koords[1] + koords[0];
	if (primiAod[1] >= k)
		primiAod[1] -= k;
	int rankZaPrijemA, rankZaPrijemB;
	MPI_Cart_rank(kom, primiAod, &rankZaPrijemA);
	MPI_Cart_rank(kom, primiBod, &rankZaPrijemB);*/
	int i = koords[0];
	int j = koords[1];

	int saljiAprema[2] = { i, (j - i + k) % k };
	int primiAod[2] = { i, (j + i) % k };

	int saljiBprema[2] = { (i - j + k) % k, j };
	int primiBod[2] = { (i + j) % k, j };

	int rankZaSlanjeA, rankZaSlanjeB;
	MPI_Cart_rank(kom, saljiAprema, &rankZaSlanjeA);
	MPI_Cart_rank(kom, saljiBprema, &rankZaSlanjeB);

	int rankZaPrijemA, rankZaPrijemB;
	MPI_Cart_rank(kom, primiAod, &rankZaPrijemA);
	MPI_Cart_rank(kom, primiBod, &rankZaPrijemB);

	for (int i = 0;i < k;i++)
	{
		cij += aij * bij;
		MPI_Sendrecv_replace(&aij, 1, MPI_INT, rankZaSlanjeA, 0,rankZaPrijemA, 0, kom, MPI_STATUSES_IGNORE);
		MPI_Sendrecv_replace(&bij, 1, MPI_INT, rankZaSlanjeB,1, rankZaPrijemB, 1, kom, MPI_STATUSES_IGNORE);


	}
	printf("Proces %d c%d%d=%d\n", newRank, koords[0], koords[1], cij);
	MPI_Finalize();
	return 0;
}
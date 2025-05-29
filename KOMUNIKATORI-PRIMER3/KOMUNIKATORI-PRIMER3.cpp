#include<stdio.h>
#include<mpi.h>
#include <corecrt_malloc.h>
int main(int argc, char* argv[])
{
	int rank, size,newRankParan,newRankNeparan;
	MPI_Comm kom1, kom2,kom;
	//MPI_Group parna, neparna,world;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//MPI_Comm_group(MPI_COMM_WORLD, &world);
	
	/*int p, np;
	if (size % 2 == 0)
		p = np = size / 2;
	else
	{
		p = (size / 2) + 1;
		np = size - p;
	}
	int* parniRankovi = (int*)malloc(p * sizeof(int));
	int* neparniRankovi = (int*)malloc(np * sizeof(int));
	int j = 0;
	int z = 0;
	for (int i = 0;i < size;i++)
	{
		if (i % 2 == 0)
		{
			parniRankovi[j] = i;
			j++;
		}
		else
		{
			neparniRankovi[z] = i;
			z++;
		}
	}
	MPI_Group_incl(world, p, parniRankovi, &parna);
	MPI_Group_incl(world, np, neparniRankovi, &neparna);*/
	int col = rank % 2==0?0:1;
	MPI_Comm_split(MPI_COMM_WORLD, col, 1, &kom);
	
	//MPI_Comm_create(MPI_COMM_WORLD, parna, &kom1);
	//MPI_Comm_create(MPI_COMM_WORLD, neparna, &kom2);
	MPI_Comm_rank(kom, &newRankParan);
	MPI_Comm_size(kom, &newRankNeparan);
	printf("Proces%d u COMM_WORLD je postao Proces%d u grupi sa ukupno %d  procesa\n", rank, newRankParan,newRankNeparan);
	MPI_Finalize();
	return 0;
}

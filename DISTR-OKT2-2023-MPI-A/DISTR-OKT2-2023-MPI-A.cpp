#include<stdio.h>
#include<mpi.h>
#define C 20
#define N 5
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int sum = 0;
	int locSum = 0;
	struct ValRank {
		double val;
		int rank;
	};
	struct ValRank glob, loc;
	loc.rank = rank;
	loc.val = 0.0f;
	int count = 0;
	for (int i = C;i <C+N;i++)
	{
		for (int j = N - 1;j>=0;j--)
		{
			MPI_Bcast(&count, 1, MPI_INT, 0, MPI_COMM_WORLD);
			int izraz = i + j;
			int flag = 0;
			if (count % size == rank)
			{
				locSum += izraz;
				for (int z = 2;z < izraz;z++)
				{
					if (izraz % z == 0)
					{
						flag = 1;
						break;

					}
				}
				if(flag==1)
					loc.val++;
			}
			if (rank == 0)
				count++;
		}
	}
	MPI_Reduce(&loc, &glob, 1, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&glob, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
	MPI_Reduce(&locSum, &sum, 1, MPI_INT, MPI_SUM, glob.rank, MPI_COMM_WORLD);
	if (rank == glob.rank)
		printf("Suma %d brojeva iznosi %d\n i to stampa proces %d\n", N * N, sum, rank);

	MPI_Finalize();
}
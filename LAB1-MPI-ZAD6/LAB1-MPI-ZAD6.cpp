#include<stdio.h>
#include<mpi.h>
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
	{
		int data[1000];
		for (int i = 0;i < 1000;i++)
		{
			data[i] = 33;
		}
		for (int i = 1;i < size;i++)
		{
			double start = MPI_Wtime();
			MPI_Send(data, 1000, MPI_INT, i,i, MPI_COMM_WORLD);
			printf("Proces %d blokirajucim putem salje poruku procesu %d \n", rank,i);
			MPI_Recv(data, 1000, MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
			double end=MPI_Wtime();
			printf("Vreme koje protekne od kada proces 0 posalje procesu %d podatak i prihvati ga nazad blokirajcue  od njega je %lf", i, end - start);
	

		}
		for (int i = 1;i < size;i++)
		{
			
			MPI_Request reqs,reqr;
			double start = MPI_Wtime();

			MPI_Isend(data, 1000, MPI_INT, i, i, MPI_COMM_WORLD,&reqs);
			printf("Proces %d ne blokirajucim putem salje poruku procesu 0\n", rank);
			MPI_Irecv(data, 1000, MPI_INT, i, i, MPI_COMM_WORLD,&reqr);
			MPI_Wait(&reqr, MPI_STATUSES_IGNORE);
			double end=MPI_Wtime();
			printf("Vreme koje protekne od kada proces 0 posalje procesu %d podatak i prihvati ga nazad ne blokirajuce od njega je %lf", i, end - start);

		}
	}
	else
	{
		int buf;
		MPI_Request req;
		MPI_Recv(&buf, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Send(&buf, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
		MPI_Irecv(&buf, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &req);
		MPI_Wait(&req, MPI_STATUSES_IGNORE);
		MPI_Isend(&buf, 1, MPI_INT, 0, rank, MPI_COMM_WORLD,&req);

	}

	MPI_Finalize();
	return 0;
}
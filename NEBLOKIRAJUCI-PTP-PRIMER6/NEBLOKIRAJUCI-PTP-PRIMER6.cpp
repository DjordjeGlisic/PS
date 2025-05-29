#include<stdio.h>
#include<mpi.h>
int main(int argc, char* argv[])
{
	int rank, size, dataRecv, dataSend;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
		dataSend = 300;
	else
		dataSend = 500;
	MPI_Request req;
	MPI_Status stat;
	MPI_Irecv(&dataRecv, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &req);
	if (rank == 0)
		MPI_Send(&dataSend, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	else
		MPI_Send(&dataSend, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	int flg;
	MPI_Test(&req,&flg,&stat);
	if (flg == 1)
		printf("Proces %d je uspesno poslao poruku\n", rank);
	else
		printf("Proces %d jos uvek nije poslao poruku\n", rank);
	MPI_Wait(&req, &stat);
	printf("Funkcija prjema poruke poslate od izvora %d a ciji je id %d je uspesno zavrsio proces %d \n", stat.MPI_SOURCE, stat.MPI_TAG, rank);
	printf("Proces %d podatak za slanje %d, podatak koji je primio %d\n", rank, dataSend, dataRecv);


	MPI_Finalize();
	return 0;
}
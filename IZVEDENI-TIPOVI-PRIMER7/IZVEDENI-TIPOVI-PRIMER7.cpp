#include<stdio.h>
#include<mpi.h>
#define MSMPI_NO_DEPRECATE_20
struct struktura {
	double realni;// [5] ;
	int celi; //[5] ;
};
#define N 5
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	struct struktura s;
	if (rank == 0)
	{
		//for (int i = 0;i < N;i++)
		//{
			s.celi = rank;
			s.realni = rank + 3.795f;
		//}
	}
	int count = 2;
	int blen[] = { N,N };
	MPI_Datatype types[] = { MPI_DOUBLE,MPI_INT };
	MPI_Aint disp[2];
	//MPI_Get_address(&s, &disp[0]);
	MPI_Aint base;
	MPI_Get_address(&s, &base);
	MPI_Get_address(&s.celi, &disp[0]);
	MPI_Get_address(&s.realni, &disp[1]);
	disp[1] = disp[1] - base;
	disp[0] = disp[0]-base;
	
	MPI_Datatype str;
	MPI_Type_create_struct(count, blen, disp, types, &str);
	MPI_Type_commit(&str);
	MPI_Bcast(&s, 1, str, 0, MPI_COMM_WORLD);
	printf("Proces%d je primio strukturu od procesa 0 s={celi=[ ",rank);
	//for (int i = 0;i < N;i++)
		printf("%d ", s.celi);
	printf("]; realni=[ ");
	//for (int i = 0;i < N;i++)
		printf("%lf ", s.realni);
	printf("]};\n");




	MPI_Finalize();

}
#include<mpi.h>
#include<stdio.h>
#include<malloc.h>
#include <string.h>
#define N 9
#define MAX 20
int main(int argc, char* argv[])
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	struct Zaposleni {
		int id;
		char ime[MAX];
		char  prezime[MAX];
		double prosPlata;
	};
	struct Zaposleni radnici[N];
	if (rank == 0)
	{
		char imena[N][MAX] = { "Jovan","Vlada","Zoran","Goran","Dejan","Novak","Mladen","Aki","Dzoni" };
		char prezimena[N][MAX] = { "Mitic","Vasic","Zikic","Glisic","Dejanic","Novakov","Mladenic","Akic","Dzonic" };
		for (int i = 0;i < N;i++)
		{
			struct Zaposleni zap;

			zap.id = i;
			for (int j = 0;j < strlen(imena[i]);j++)
			{
				zap.ime[j] = imena[i][j];

			}
			for (int j = 0;j < strlen(prezimena[i]);j++)
			{
				zap.prezime[j] = prezimena[i][j];

			}
			zap.prosPlata = 1000.0f - 5*i;
			radnici[i] = zap;
		}
	}
	MPI_Datatype str;
	int count = 4;
	int bLens[] = { 1,N,N,1 };
	int types[] = { MPI_INT,MPI_CHAR,MPI_CHAR,MPI_DOUBLE };
	MPI_Aint base;
	MPI_Aint disps[4];
	struct Zaposleni z;
	MPI_Get_address(&z, &base);
	MPI_Get_address(&z.id, &disps[0]);
	MPI_Get_address(&z.ime, &disps[1]);
	MPI_Get_address(&z.prezime, &disps[2]);
	MPI_Get_address(&z.prosPlata, &disps[3]);
	for (int i = 0;i < 4;i++)
		disps[i] -= base;
	MPI_Type_create_struct(count, bLens, disps, types, &str);
	MPI_Type_commit(&str);
	int kolko = N / size;
	struct Zaposleni parc[N];
	MPI_Scatter(radnici, kolko, str, parc, kolko, str, 0, MPI_COMM_WORLD);
	struct ValRank {
		double val;
		int rank;
	};
	struct ValRank loc, glob;
	loc.rank = rank;
	loc.val = 9999999999999999999.0f;
	struct Zaposleni najmanji;
	for (int i = 0;i < kolko;i++)
	{
		if (parc[i].prosPlata < loc.val)
		{
			loc.val = parc[i].prosPlata;
			najmanji.id = parc[i].id;
			for(int j=0;j<strlen(parc[i].ime);j++)
				najmanji.ime[j] = parc[i].ime[j];
			for (int j = 0;j < strlen(parc[i].prezime);j++)
				najmanji.prezime[j] = parc[i].prezime[j];
			najmanji.prosPlata = loc.val;


		}

	}
	MPI_Reduce(&loc, &glob, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
	MPI_Bcast(&glob, 1, MPI_DOUBLE_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&najmanji, 1, str, glob.rank, MPI_COMM_WORLD);
	if (rank == 0)
	{
		printf("Proces koji je pronasao najmajju prosecnu platu radnika je Proces%d a ta plata iznosi %lf dok radnik ima id %d\n", glob.rank, najmanji.prosPlata,najmanji.id);
	
	}





	MPI_Finalize();
}
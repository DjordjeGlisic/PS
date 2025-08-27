#include<stdio.h>
#include<omp.h>
#define M 5
int main(int argc, char* argv[])
{
	int A[M][M] = {
		{1,2,3,4,5},
		{6,7,8,9,10},
		{11,12,13,14,15},
		{16,17,18,19,20},
		{21,22,23,24,25}


	};
	int val = -1;
	double start=omp_get_wtime();
#pragma omp parallel num_threads(M) default(shared) 
	{
//#pragma omp for schedule(dynamic,5) reduction(max:val)
//		for (int i = 0;i < M * M;i++)
//		{
//			int vrsta = i / M;
//			int kolona = i % M;
//			if (A[vrsta][kolona] > val)
//				val = A[vrsta][kolona];
//
//		}
#pragma omp for schedule(static,1) reduction(max:val)
		for (int i = 0;i < M;i++)
		{
			
			for (int j = 0;j < M;j++)
			{
				if (val < A[i][j])
					val = A[i][j];
			}
			
		}
	}
	double end = omp_get_wtime();
	printf("Maksimalni element matrice je %d izracunato je za %lf sekunde", val, end-start);
	start = omp_get_wtime();
	int max = -1;
	for (int i = 0;i < M;i++)
	{
		for (int j = 0;j < M;j++)
		{
			if (A[i][j] > max)
				max = A[i][j];
		}
	}
	end = omp_get_wtime();
	printf("Maksimalni element matrice je %d izracunato je za %lf sekunde", max, end - start);


	return 0;
}
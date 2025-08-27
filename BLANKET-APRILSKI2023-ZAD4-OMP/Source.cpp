#include<omp.h>
#include<stdio.h>
#define N 4
int main(int argc, char* argv[])
{
	int A[N][N] = {
		{1,7,9,2},
		{3,2,1,0},
		{1,1,2,0},
		{1,2,3,4}
	};
	int B[N][N] = {
		{0,1,1,0},
		{1,0,0,1},
		{0,1,0,0},
		{0,0,1,0}
	};
	int C[N][N];
	printf("MAtrica C\n");
#pragma omp parallel num_threads(N) if(N>2) default(shared)
	{
#pragma omp for schedule(static,1) collapse(2)
		for (int i = 0;i < N;i++)
		{
			for (int j = 0;j < N;j++)
			{
				printf("c[%d][%d] generise nit%d=", i, j,omp_get_thread_num());
				C[i][j] = 0;
				for (int z = 0;z < N;z++)
				{
					C[i][j] += A[i][z] * B[z][j];
				}
				printf("%d\n", C[i][j]);
			}
		}
		        
	}

}
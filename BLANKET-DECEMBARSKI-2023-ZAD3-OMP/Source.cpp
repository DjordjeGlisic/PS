#include<stdio.h>
#include<omp.h>
#include<malloc.h>
int main(int argc, char* argv[])
{
	int n=4;
	//scanf("%d", &n);
	int** A = (int**)malloc(n * sizeof(int*));
	int** B = (int**)malloc(n * sizeof(int*));
	int pomA[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	int pomB[] = { 0,1,1,0,1,0,0,1,1,1,0,0,0,1,1,0 };
	int** C;
#pragma omp parallel num_threads(10) default(shared)
	{
#pragma omp single
		{
			C = (int**)malloc(n * sizeof(int*));
		}
#pragma omp for schedule(dynamic,1) 
		for (int i = 0;i < n;i++)
		{
			A[i] = (int*)malloc(n * sizeof(int));
			B[i] = (int*)malloc(n * sizeof(int));
			C[i] = (int*)malloc(n * sizeof(int));
			for (int j = 0;j < n;j++)
			{
				/*scanf("%d", A[i][j]);
				scanf("%d", B[i][j]);*/
				A[i][j] = pomA[i * n + j];
				B[i][j] = pomB[i * n + j];
			}

		}
#pragma omp barrier
	}
#pragma omp parallel num_threads(10)
	{
#pragma omp for schedule(static,1) collapse(2)
		for (int i = 0;i < n;i++)
		{
			for (int j = 0;j < n;j++)
			{
				C[i][j] = 0;

				int sum = 0;

				for (int k = 0;k < n;k++)
				{
					sum += A[i][k] * B[k][j];
				}


				C[i][j] = sum;


			}




		}
	}
		printf("MAtrica C=\n");

		for (int i = 0;i < n;i++)
		{
			printf("[");
			for (int j = 0;j < n;j++)
			{
				printf(" %d ", C[i][j]);
			}
			printf("]\n");
		}

}
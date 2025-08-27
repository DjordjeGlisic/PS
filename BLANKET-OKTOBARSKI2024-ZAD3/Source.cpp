#include<stdio.h>
#include<omp.h>
#define M 5
int main(int argc, char* argv[])
{
	int t = 3;
	int k = 3;
	int c[] = { 1, 2, 3, 4, 5 };
	int e[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31 };
#pragma omp parallel num_threads(5)
	{
		for (int i = 0;i < 2;i++)
		{
#pragma omp for schedule(static)
			for (int j = 0;j < M;j++)
			{
				c[j] += e[3 + (i * M + j) * k];
			}
		}
#pragma omp barrier
#pragma omp for schedule(static)
		for (int j = 0;j < M;j++)
		{
			printf("Nit %d stampa c[%d]=%d\n",omp_get_thread_num(),j,c[j]);
		}
	}
	
}
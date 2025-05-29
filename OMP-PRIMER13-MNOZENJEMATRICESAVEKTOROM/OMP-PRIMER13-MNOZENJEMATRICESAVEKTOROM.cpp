#include<stdio.h>
#include<omp.h>
#define n 3
#define m 4
int main(int argc, char* argv[])
{
	int matrica[n][m] = { {1,2,3,4},{5,6,7,8},{9,10,11,12} };
	int vektor[] = { 7,2,5,2 };
	int rez[m] = { 0,0,0,0 };
#pragma omp parallel default(shared) if(m>=n) num_threads(n)
	{
#pragma omp for schedule(static,1) 
		for (int i = 0;i < n;i++)
		{
			for (int j = 0;j < m;j++)
			{
				rez[i] += matrica[i][j] * vektor[j];
			}
		}
	}
	printf("Rezultat mnozenja vektora i matrice:[");
	for (int i = 0;i < n;i++)
		printf("%d ", rez[i]);
	printf("]\n");
	return 0;
}
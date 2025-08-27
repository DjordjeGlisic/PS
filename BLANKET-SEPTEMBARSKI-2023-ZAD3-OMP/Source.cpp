#include<stdio.h>
#include<omp.h>
#include<malloc.h>
#define N 5
int main(int argc, char* argv[])
{
	int z[N] = { 1,2,3,4,5 };
	int a[N] = { 6,7,8,9,10 };
	int b[N] = { 0,7,2,1,4};
	int c[100];
	for (int i = 0;i < 100;i++)
		if (i % 2 == 0)
			c[i] = 2;
		else
			c[i] = 4;
	int d = 42;
	int pom = 3;
	int prod = 1;
#pragma omp parallel num_threads(10) default(shared)
	{
#pragma omp for schedule(runtime) reduction(*:prod) ordered
		for (int i = N - 2;i >= 0;i--)
		{
			prod *= z[i];
#pragma omp ordered
			{
				printf("Iteracija%d a=%d b=%d,c=%d\n",i, a[i + 1], b[i], c[pom + N - i - 1]);
				a[i] = b[i] * c[pom + N - i - 1] - a[i + 1];
			}
		}
	}
	printf("d=%d\n", d / prod);
	printf("Vektor A=[");
	for (int i = 0;i < N;i++)
		printf(" %d ", a[i]);
	printf("]\n");
	return 0;
}
#include<stdio.h>
#include<omp.h>
#define N 3
int main(int argc, char* argv[])
{
	int m = 2;
	int b[100];
	for (int i = 0;i < 100;i++)
		b[i] = i;
	int a[N];
	for (int i = 0;i < N;i++)
		a[i] = i;
	int p = 5;
#pragma omp parallel num_threads(N) default(shared)
	{
#pragma omp for schedule(guided,3) reduction(+:m) ordered
		for (int i = 0;i < N * N;i++)
		{
			m += p;
			int index = i % N;
#pragma omp ordered
			{
				a[index] += b[i * N * p + m];

			}
		}
	}
	printf("NIZ=[");
	for (int i = 0;i < N;i++)
		printf(" %d ", a[i]);
	printf("]\ m=%d\n", m);
}
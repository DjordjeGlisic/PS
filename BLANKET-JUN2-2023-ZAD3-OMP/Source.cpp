#include<stdio.h>
#include<omp.h>
#define N 5
int main(int argc, char* argv[])
{
	int a[] = { 10,9,8,7,6 };
	int b[] = { 6,7,8,9,10 };
	int c[] = { 1,0,0,1,0,1,0,0 };
	int z[] = { 5,1,2,3,4 };
	int pom = 3;
	int d = 2;
#pragma omp parallel if(N>5) num_threads(5) default(shared)
	{
#pragma omp for schedule(static,1) reduction(*:d)
		for (int i = 1;i < N;i++)
			d *= z[i];
#pragma omp single
		{
			for (int i = 1;i < N;i++)
			{
				a[i] = b[i] * c[pom + N - i - 1] + a[i - 1];
			}
		}
	}
	printf("Nakon paralelnog izvrsenja d=%d\n", d);
}
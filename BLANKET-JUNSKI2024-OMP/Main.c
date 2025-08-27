#include<stdio.h>
#include<omp.h>
int main(int argc, char* argv[])
{
	int d = 42;
	int a[] = { 1,2,3,4,5,6,7,8,9,10 };
	int N = 10;
	int z[] = { 2,2,1,2,3,4,5,6,1,2,3,4,5,1,5,2,3 };
	int c[] = { 1,1,1,1,1,11,1,1,1,1,1,1,1,1,1,1,1,1,1 };
	int pom = d;
	int b[] = {10,9,8,7,6,5,4,3,2,1};
	int a2[100];
#pragma omp parallel num_threads(10)
	{
#pragma omp for schedule(static,3)
		for (int i = 1;i < N - 2;i++)
		{
			a2[i] = a[i];
		}
#pragma omp for schedule(static,3) reduction(*:d)
		for (int i = N - 2;i > 0;i--)
		{
			d *= z[i];
			a[i] = b[i] * c[pom + N - 1] - a2[i - 1];
		}
	}
	
}
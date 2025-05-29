#include<stdio.h>
#include<omp.h>
#define n 10
int main(int argc, char* argv[])
{
	int rez = 1;
#pragma omp parallel num_threads(4) default(shared)
	{
#pragma omp for schedule(static) reduction(*:rez)
		for (int i = 1;i <= n;i++)
		{
			rez *= i;
		}
	}
	printf("Nakon redukcije u paralelizovanoj for petlji faktorjel broja %d iznosi %d\n", n, rez);
}
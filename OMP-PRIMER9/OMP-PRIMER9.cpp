#include<stdio.h>
#include<omp.h>
#define CHUNK_SIZE 3
#define n 16
#define p 4
int main(int argc, char* argv[])
{
#pragma omp parallel  default(shared) num_threads(p) if(n>p)
	{
#pragma omp  for   schedule(static,CHUNK_SIZE)
		//n=16 svako po 3  0->0,1,2,12,13,14  1->3,4,5,15   2->6,7,8     3->9,10,11
		for (int i = 0;i < n;i++)
			printf("Nit % d dobija iteraciju % d\n", omp_get_thread_num(), i);
	}
printf("//////////////////////////////////\n");
#pragma omp parallel  default(shared) num_threads(p) if(n>p)
	{
#pragma omp  for   schedule(static)
		//n=16 i p=4 svako po 4  0->0,1,2,3  1->4,5,6,7   2->8,9,10,11     3->12,13,14,15
		for (int i = 0;i < n;i++)
			printf("Nit % d dobija iteraciju % d\n", omp_get_thread_num(), i);
	}
}
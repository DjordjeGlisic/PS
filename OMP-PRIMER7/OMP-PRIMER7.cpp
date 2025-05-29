#include<stdio.h>
#include<omp.h>
#include<malloc.h>
int main(int argc, char* argv[])
{
	int* a;
	int* b;
	int* c;
	int n = 100;
	a = (int*)malloc(n * sizeof(int));
	b = (int*)malloc(n * sizeof(int));
	c = (int*)malloc(n * sizeof(int));
	int br = 1;
#pragma omp parallel num_threads(2) firstprivate(br) default(shared)
	{
#pragma omp single
		{
#pragma omp task
			{
			for (int i = 0;i < n;i++)
				a[i] = br++;


			}
#pragma omp task
			{

				for (int i = 0;i < n;i++)
					b[i] = br++;
		
			}
		}

	}
	printf("a=[");
	for (int i = 0;i < n;i++)
		printf("%d ", a[i]);
	printf("\nb=[");
	for (int i = 0;i < n;i++)
		printf("%d ", b[i]);
	printf("]\n");

#pragma omp parallel default(shared) num_threads(30)
	{
#pragma omp for schedule(static,10)  
		for (int i = 0;i < n;i++)
		{
			c[i] = a[i] + b[i];
			printf("Nit%d a[%d]+b[%d]=c[%d]=%d\n", omp_get_thread_num(), i, i, i,c[i]);
		
		}

	}

}
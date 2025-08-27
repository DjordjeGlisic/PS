#include<stdio.h>
#include<omp.h>
int main(int argc, char* argv[])
{
	omp_set_num_threads(3);
//#pragma omp parallel
//	{
//#pragma omp task
//		printf("Task1\n");
//#pragma omp task
//		printf("Task2\n");
//
//
//	}
	int a = 2;
	int okt[] = { 1, 2, 3, 4, 5 };
	int okt2[] = { 6,7,8,9,10 };
	int rok[] = { 1,2,3,4,5 };
	int n = 5;
	int rok2[10];
	int nizA[10];
	nizA[n-1] = a;
#pragma omp parallel num_threads(5) default(shared)
	{
#pragma omp for schedule(static,1)
		for (int i = 1;i < n;i++)
			rok2[i] = rok[i - 1];
#pragma omp for schedule(dynamic,1) ordered 
		for (int i = n - 1;i >1;i--)
		{
#pragma omp ordered
			{
				
			nizA[i] = nizA[i] + okt[i] + okt2[i];
			nizA[i - 1] = nizA[i];
			}
			

		}
#pragma omp barrier
#pragma omp master
		{
			a = nizA[0];
		}
#pragma omp for schedule(runtime)
		for (int i = n - 1;i > 1;i--)
			rok[i] = rok2[i] + nizA[i];


	}
	printf("Promenljiva a=%d\n", a);
	printf("ROK=[");
	for (int i = 0;i <n;i++)
		printf(" %d ", rok[i]);
	printf("]\n");

	return 0;
}
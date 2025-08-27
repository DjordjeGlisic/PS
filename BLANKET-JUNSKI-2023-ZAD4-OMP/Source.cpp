#include<omp.h>
#include<stdio.h>
#define N 10
int main(int argc, char* argv[])
{
	int a[100];
	for (int i = 0;i < N;i++)
	{
		printf("Uneiste %d element\n", i);
		scanf("%d", &a[i]);
	}
	int globMax=INT_LEAST32_MIN;
	int redMax= INT_LEAST32_MIN;
#pragma omp parallel num_threads(5) default(shared)
	{
		int maksimum = 0;
#pragma omp for schedule(static,3)
		for (int i = 0;i < N;i++)
		{
			if (a[i] > maksimum)
				maksimum = a[i];
		}
#pragma omp critical (najveci)
		{
			if (globMax < maksimum)
			{
				globMax = maksimum;
			}
		}
#pragma omp for schedule(static,3) reduction(max:redMax)
		for (int i = 0;i < N;i++)
		{
			if (redMax < a[i])
				redMax = a[i];

		}
	}
	printf("Maksimum preko kritikal sekcije =%d\n", globMax);
	printf("Maksimum preko redukcije =%d\n", redMax);

}
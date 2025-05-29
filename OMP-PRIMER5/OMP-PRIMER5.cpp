#include<stdio.h>
#include<omp.h>
int main(int* argc, char* argv[])
{
	int a = 2;
#pragma omp parallel default(shared) num_threads(3) reduction(+:a)
	{

			a += 2;
		
#pragma omp barrier
	//sve niti stampaju 2 samo master nit stampa 2*3+2=8, ali izvan paralelenog regiona
	printf("ovo je nit %d i ona stampa vrednost podatka a=%d\n", omp_get_thread_num(), a);


	}
	printf("ovo je master nit i ona stampa vrednost podatka a=%d\n", a);

	return 0;
}
#include<stdio.h>
#include<malloc.h>
#include<omp.h>
int racunajSumu(int* a, int n)
{
	int sum = 0;
#pragma omp parallel num_threads(3) reduction(+:sum)
	{
		#pragma omp for schedule(guided,2) 
			
				for (int i = 0;i < n;i++)
				{
					sum += a[i];
				}
			

	}
#pragma omp barrier
	return sum;
}
int main(int argc, char* argv[])
{
	int* vektor = (int*)malloc(10 * sizeof(int));
	int br = 1;
	for (int i = 0;i < 10;i++)
		vektor[i] = br++;
	//vektor=[1,2,3,4,5,6,7,8,9,10]
	printf("Suma svih elemenata vektora je %d\n", racunajSumu(vektor, 10));
	free(vektor);
	return 0;
}
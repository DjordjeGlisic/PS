#include<stdio.h>
#include<omp.h>
#define N 10
int fib(int input)
{
	
	int fn1, fn2;
	if (input < 2) return (input);
#pragma omp task shared(fn1) firstprivate (input)
	fn1 = fib(input - 1);
#pragma omp task shared(fn2) firstprivate (input)
	fn2 = fib(input - 2);
#pragma omp taskwait
	return(fn1 + fn2);

}
		
int main(int argc, char* argv[])
{
	int sum2 = 0;
	int sum1 = 1;
	int sum;
	int i = 2;
#pragma omp parallel num_threads(5) firstprivate(sum2,sum1,i) shared(sum)  
	{
#pragma omp single
		{

			printf("Fibonacijev niz prvih %d brojeva je: ", N);
			while (i <= N)
			{
				sum = sum2 + sum1;
#pragma omp task
				printf("Nit%d uvecava F(n-2) koji je iznosio %d\n", omp_get_thread_num(), sum2);
				sum2 = sum1;
#pragma omp taskwait
#pragma omp task
				printf("Nit%d uvecava F(n-1) koji je iznosio %d\n", omp_get_thread_num(), sum1);
				sum1 = sum;

#pragma omp task
				printf("Nit%d=%d ", omp_get_thread_num(), sum);
#pragma omp taskwait
				i++;
			}
			sum = fib(N);
		}


	}
	printf("%d\n", sum);
	return 0;
}
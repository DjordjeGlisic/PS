#include<stdio.h>
#include<omp.h>
int main(int argc, char* argv[])
{
	int a, b, c;
	int privatna;
#pragma omp parallel default(shared) private(privatna) num_threads(10)
	{
#pragma omp single
		{
			printf("Ovo je nit%d koja ce inicijalizovati deljivu memoriuju\n", omp_get_thread_num());
			a = 5;
			b = 55;
			c = 555;

		}
#pragma omp barrier
		int tid = omp_get_thread_num();
		switch (tid)
		{
		case 0:
			privatna = a;
			break;
		case 1:
			privatna = a + b;
			break;
		case 2:
			privatna = a + b + c;
			break;
		default:
			privatna = -1;
			break;
		}
		printf("Nit%d od %d stampa %d\n", tid, omp_get_num_threads(), privatna);

	}
}
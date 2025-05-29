#include<stdio.h>
#include<omp.h>
//PRIMER 2 UKLJUCUJE MERENJE VREMENA IZVRSENJA PARALELNOG REGIONA
int main(int argc, char* argv[])
{
	int tid;
	int threads;
	double start = omp_get_wtime();
#pragma omp parallel private(tid) shared(threads) num_threads(4)
	{

		#pragma omp master
		{
				threads = omp_get_num_threads();
		}
		#pragma omp barrier
		tid = omp_get_thread_num();
		printf("Nit %d od %d niti stampa Hello\n", tid, threads);
		printf("Nit %d stampa World\n", tid);
	}
	double end = omp_get_wtime();
	end -= start;
	printf("Master nit stampa ukupno vreme izvrsenja paralelenog regiona %lf sekunde\n", end);
	return 0;
}
#include<omp.h>
#include<stdio.h>
#define OMP_NTHREADS 5
int t_id;
#pragma threadprivate(t_id)

int main(int argc, char* argv[])
{
	t_id = 0;
	
#pragma parallel copyin(t_id) 
	{

			printf("Ovo je prvi paralaleni region i stampa ga nit %d\n", omp_get_thread_num());
		
#pragma omp barrier
	printf("Nit%d stampa t_id=%d\n", t_id, t_id);
	t_id = omp_get_thread_num();
	printf("Nit%d stampa t_id=%d\n", t_id, t_id);

	}
#pragma parallel
	{
#pragma single
		{
			printf("Ovo je DRUGI paralaleni region i stampa ga nit %d\n", omp_get_thread_num());
		}
#pragma omp barrier
		printf("Nit%d stampa t_id=%d\n", t_id, t_id);
		t_id += 10;
		printf("Nit%d stampa t_id=%d\n", t_id, t_id);

	}
#pragma parallel
	{
#pragma single
		{
			printf("Ovo je TRECI paralaleni region i stampa ga nit %d\n", omp_get_thread_num());
		}
#pragma omp barrier
		printf("Nit%d stampa t_id=%d\n", t_id, t_id);
		t_id += 20;
		printf("Nit%d stampa t_id=%d\n", t_id, t_id);

	}

}
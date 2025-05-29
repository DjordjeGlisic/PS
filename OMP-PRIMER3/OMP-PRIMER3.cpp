#include<stdio.h>
#include<omp.h>
int main(int argc, char* argv[])
{

	int t_idPrivate = 0;
	int t_idFirstPrivate = 50;
	int t_idLastPrivate = 100;
	int deljiva = -2;
	omp_set_num_threads(5);
#pragma omp parallel default(none) shared(deljiva,t_idLastPrivate) private(t_idPrivate) firstprivate(t_idFirstPrivate) 
	{
		printf("Nit %d stampa privatnu:%d firstP:%d lastP:%d dejivu:%d\n", omp_get_thread_num(),t_idPrivate, t_idFirstPrivate, t_idLastPrivate, deljiva);
		t_idPrivate = omp_get_thread_num();
		t_idFirstPrivate = t_idPrivate;
#pragma omp sections lastprivate(t_idLastPrivate)
		{
#pragma omp section 
		{

				t_idLastPrivate = omp_get_thread_num();
		}

		}
		deljiva = omp_get_thread_num();
#pragma omp barrier
		printf("Nit %d stampa privatnu:%d firstP:%d lastP:%d dejivu:%d\n", omp_get_thread_num(), t_idPrivate, t_idFirstPrivate, t_idLastPrivate, deljiva);

	}
	printf("Nit ID=0 stampa privatnu:%d firstP:%d lastP:%d dejivu:%d\n",t_idPrivate, t_idFirstPrivate, t_idLastPrivate, deljiva);

	return 0;
}
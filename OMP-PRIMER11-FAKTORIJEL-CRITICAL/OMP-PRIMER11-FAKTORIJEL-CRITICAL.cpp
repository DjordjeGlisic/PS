#include<stdio.h>
#include<omp.h>
#define n 10
int main(int argc, char* argv[])
{
	int parc = 1;
	int rezC=1;
	int rezA = 1;
	int rezS=1;
	omp_lock_t sem;
	omp_init_lock(&sem);

#pragma omp parallel num_threads(n) default(shared) firstprivate(parc)
	{
#pragma omp for schedule(static)
		for (int i = 1;i <= n;i++)
		{
			parc *= i;
		}
#pragma omp barrier
#pragma omp critical (akumulacija)
		{
			rezC *= parc;
		}
#pragma omp atomic
		rezA *= parc;
		omp_set_lock(&sem);
		rezS *= parc;
		omp_unset_lock(&sem);
#pragma omp barrier

	}
	omp_destroy_lock(&sem);
	printf("Faktorijel broja %d iznosi %d\n", n, rezA);
	printf("Faktorijel broja %d iznosi %d\n", n, rezC);
	printf("Faktorijel broja %d iznosi %d\n", n, rezS);


}
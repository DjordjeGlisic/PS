#include<stdio.h>
#include<omp.h>

int main(int argc, char* argv[])
{
	int A = 20;
	int B = 22;
#pragma omp parallel default(shared) num_threads(100)
	{
//#pragma omp sections
//		{
		//OBE NITI IZVRSAVAJU OBA TASKA BEZ POZNATOG REDOSLEDA IZVRSAVANJA TASKOVA U OKVIRU JEDNE NITI 
		//NE ZNA SE KOJA NIT CE KADA IZVRSITI TASK ALI SVE NITI TE TASKOVE IZVRSAVAJU 
		//IAKO SU TASKOVI SEKVENCIJALNO POSTAVLJENI ONI TAKO NE MORAJU BITI IZVRSENI
		//ZA 10 NITI I 2 SEKCIJE JEDNU SEKCIJU IZVSRAVA JEDNA NIT DRUGU DRUGA NIT A OSTALE NITI SU NE UPOSLJENE
#pragma omp task
		{
			A++;
			printf("Ovaj task izvrsava Nit%d i stampa A=%d\n", omp_get_thread_num(), A);

		}
#pragma omp task
		{
			B++;
			printf("Ovaj task izvrsava Nit%d i stampa B=%d\n", omp_get_thread_num(), B);
		}
		/*}*/
#pragma omp single
		{
			//SADA POSTO SU TASKOVI U SINGLE DIREKTIVI NIT KOJA TELO SINGLE DIREKTIVE IZVRSAVA NAILAZI NA TASK 1 I DODELJUJE GA NEKOJ NITI
			//NAIAZI NA TASK2 I DODELJUJE GA NEKOJ DRUGOJ NITI
#pragma omp task
			{
				A++;
				printf("Ovaj task izvrsava Nit%d i stampa A=%d\n", omp_get_thread_num(), A);

			}
#pragma omp task
			{
				B++;
				printf("Ovaj task izvrsava Nit%d i stampa B=%d\n", omp_get_thread_num(), B);
			}

		}

	}
}
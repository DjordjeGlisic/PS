#include<stdio.h>
#include<omp.h>
#define n 200
#define p 4
#define CHUNK_SIZE 7
int main(int argc, char* argv[])
{
	omp_set_num_threads(p);
#pragma omp parallel if(n>p) default(shared)
	{
		
#pragma omp for schedule(static,CHUNK_SIZE) 
		

			for (int i = 0;i < n;i++)
				printf("Nit%d dobija iteraciju %d po round robin principu\n", omp_get_thread_num(), i);
			//nit0->0,1,8,9
			//nit1->2,3,10,11
			//nit2->4,5,12,13
			//nit3->6,7,14,15


		
#pragma omp barrier
#pragma omp for schedule(dynamic,CHUNK_SIZE) 
		
			for (int i = 0;i < n;i++)
				printf("Nit%d dobija iteraciju %d po random principu\n", omp_get_thread_num(), i);
			//sve niti dobijaju 3 iteracije minimalno po radnom principu s obzirom da 16/3 nije deljivo neke niti ce dobiti vise od 3 itearacije
			// ne znamo koje su to niti
			

		
#pragma omp barrier
#pragma omp for schedule(guided,CHUNK_SIZE) 
		
			for (int i = 0;i < n;i++)
				printf("Nit%d dobija iteraciju %d po random guided principu\n", omp_get_thread_num(), i);
			//neka nit dobija 16/4->4 iteracija druga neka nit (16-4)/4->3 treca neka 8/4->2 nakon cega imamo manje od chunksize tkd 
			//ostalo se rasporedjuje random po chunk_size

	
	}
	return 0;
}

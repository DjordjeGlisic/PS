#include<stdio.h>
#include<omp.h>
#include<math.h>
#define m 0
#define n 1
#define N 1000

int main(int argc, char* argv[])
{
	double pi = 0.0f;
	double h = 1.0f / (double)N;
	double Fx = 0.0f;
	omp_set_num_threads(4);
#pragma parallel default(shared) 
	{
#pragma for schedule(static,3) reduction(+:Fx)
		for (int i = 1;i <= N;i++)
		{
			float x = (i - 0.5) * h ;
			Fx+= 4.0f / (double)(1.0f + (double)pow(x, 2));

		}

	}
	pi = Fx * h;
	printf("Na osnovu integrala x=[0,1] 4dx/(1+x^2), Pi=%lf\n", pi);
	Fx = 0.0f;
#pragma parallel default(shared) 
	{
#pragma for schedule(static,3) 
		for (int i = 1;i <= N;i++)
		{
			float x = (i - 0.5) * h;
			double sum = 4.0f / (double)(1.0f + (double)pow(x, 2));
#pragma omp critical(dodaj)
			{
				Fx += sum;

			}

		}

	}
	pi = Fx * h;
	printf("Na osnovu integrala x=[0,1] 4dx/(1+x^2), Pi=%lf\n", pi);
}
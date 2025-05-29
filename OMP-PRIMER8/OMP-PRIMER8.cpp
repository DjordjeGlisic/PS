#include<stdio.h>
#include<omp.h>
int main(int argc, char* argv[])
{
	int tid, a;
#pragma parallel num_threads(100) defualt(none) private(tid) shared(a)
	{
		int n = 200;
#pragma for lastprivate(a) schedule(dynamic,5)
		for (int i = 0;i < n;i++)
		{
			a = i + 1;
		}

	}
	//a=200
	printf("Nakon paralelilzovanog izvrsenja for petlje promenljiva a u poslednjoj iteraciji biva setovana na %d\n", a);
	return 0;
}
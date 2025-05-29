#include<stdio.h>
#include<omp.h>
#include<math.h>

int init()
{
	return 300;
}
int f(int x)
{
	return x / 6;
}
int g(int x)
{
	return x / 5;
}
int main(int argc, char* argv[])
{
	int* x = (int*)malloc(sizeof(int));
	int* y= (int*)malloc(sizeof(int));
	int*z= (int*)malloc(sizeof(int));
#pragma parallel shared(x)
	{
#pragma omp single
		{
#pragma omp task //depend(out:x)
			{
				*x = init();
			}
#pragma omp taskwait
#pragma omp task //depend(in:x)
			{
				*y = f(*x);
			}
#pragma omp task //depend(in:x)
			{
				*z = g(*x);
			}
		}
#pragma omp barrier
	}
	printf("X=%d Y=%d Z=%d\n", *x, *y, *z);
}
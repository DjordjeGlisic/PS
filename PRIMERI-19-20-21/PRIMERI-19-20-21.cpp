//#include<stdio.h>
//#include<omp.h>
// //////////////////////////PRIMER 19 //////////////////////////

//int main(int argc, char* argv[])
//{
//	//PARALELIZACIJE PETLJE x=b[i]+c[i]
//						//  a[i]=a[i+1]+x
//	int x = 25;
//	int br = 1;
//	int a[10];
//	int b[10];
//	int c[10];
//	for (int i = 0;i < 10;i++)
//	{
//		a[i] = br;
//		b[i] = br;
//		c[i] = br;
//		br++;
//	}
//	omp_set_num_threads(5);
//	int a2[9];
//	for (int i = 0;i < 9;i++)
//		a2[i] = a[i + 1];
//#pragma omp parallel default(shared)
//	{
//#pragma omp for schedule(runtime)
//		
//			/*for (int i = 0;i < 10;i++)
//			{
//				x = b[i] + c[i];
//				a[i] = a[i + 1] + x;
//				printf("X=%d\n", x);
//			}*/
//			for (int i = 0;i < 9;i++)
//				{
//					x = b[i] + c[i];
//					a[i] = a2[i] + x;
//					printf("X=%d\n", x);
//				}
//			x = b[9] + c[9];
//		
//	}
//	printf("a=[ ");
//	for (int i = 0;i < 10;i++)
//	{
//		printf("%d ",a[i]);
//	}
//	printf("]\n");
//}
//////////////////////////PRIMER 20//////////////////////////

//#include<stdio.h>
//#include<omp.h>
//#include<malloc.h>
//#define N 10
//int main(int argc, char* argv[])
//{
//	int a[N];
//	int b[N];
//	int c[N];
//	int br = 1;
//	int x = -10;
//	for (int i = 0;i < N;i++)
//	{
//		a[i] = b[i] = c[i] = br++;
//		
//	}
//	int d1 =-30;
//	int d2;
//	d2 = 20;
//#pragma omp parallel default(shared) num_threads(5) 
//	{
//#pragma omp  for schedule(runtime) lastprivate(d1,x) 
//		for (int i = 0;i < N;i++)
//		{
//			x= (b[i] + c[i]) / 2;
//			a[i] = a[i] + x;
//			d1 = 2 * x;
//
//		}
//
//	}
//	int y;
//	y = d1 + d2 + x;
//	printf("Y=%d X=%d D1=%d\n", y, x,d1);
//
//}
//////////////////////////PRIMER 21 POD A //////////////////////////

//#include<stdio.h>
//#include<omp.h>
//#define N 10
//int main(int argc, char* argv[])
//{
//	int x = 2;
//	int a[N];
//	for (int i = 0;i < N;i++)
//		a[i] = i;
//#pragma omp parallel num_threads(5) default(shared)
//	{
//#pragma omp for schedule(static,2) reduction(+:x)
//		for (int i = 0;i < N;i++)
//			x += a[i];
//	}
//	printf("X=%d\n", x);//x=47
//}
//////////////////////////PRIMER 21 POD B //////////////////////////
//#include<stdio.h>
//#include<omp.h>
//#define N 10
//int main(int argc, char* argv[])
//{
//	int a[N];
//	for (int i = 0;i < N;i++)
//		a[i] = i;
//	int x = 5;
//#pragma omp parallel default(shared)
//	{
//#pragma omp for schedule(static,3)
//		for (int i = 0;i < N;i++)
//			a[i] += x + i;
//#pragma omp for schedule(static,3) reduction(+:x)
//		for (int i = 0;i < N;i++)
//		{
//			x += 1;
//			printf("X=%d\n", x);
//			printf("Nit%d->a[%d]=%d\n", omp_get_thread_num(), i, a[i]);
//		}
//	}
//	printf("X=%d\n", x);
//
//
//}
//////////////////////////PRIMER 21 POD V //////////////////////////
//#include<stdio.h>
//#include<omp.h>
//#include<math.h>
//#define N 10
//int main(int argc, char* argv[])
//{
//	int a[N];
//	int b[N];
//	int c[N];
//	int iSum = 0;
//	int pow2 = 1;
//	int Idx = 1;
//#pragma omp parallel default(shared) num_threads(3)
//	{
//#pragma omp for schedule(static,3)
//		for (int i = 0;i < N;i++)
//		{
//			a[i] = i;
//			b[i] = i + 1;
//			c[i] = i + 2;
//		}
//#pragma omp barrier
//	}
//	int a2[N];
//	for (int i = 0;i < N-1;i++)
//	{
//		a2[i] = a[i + 1];
//	}
//#pragma omp parallel default(shared) num_threads(3)
//	{
//#pragma omp for schedule(runtime) reduction(+:iSum,pow2,Idx)
//		for (int i = 0;i < N;i++)
//		{
//			if (i < N - 1)
//			{
//				a[i] = a[i] + a[i + Idx];
//
//			}
//			b[i] = (i * (i - 1)) / 2;
//			c[i] = pow(2, i);
//			printf("a[%d]=%d,b[%d]=%d,c[%d]=%d\n", i, a[i], i, b[i], i, c[i]);
//			iSum += i;
//			pow2 *= i;
//			Idx++;
//		}
//#pragma omp barrier
//	}
//	printf("iSum=%d,pow2=%d,Idx=%d\n", iSum, pow2, Idx);
//	return 0;
//}
//////////////////////////PARALELIZACIJA b[i]+=a[i-1];a[i]+=c[i] //////////////////////////
//#include<stdio.h>
//#include<omp.h>
//#define N 10
//#define NUM_THREADS 10
//int main(int argc, char* argv[])
//{
//	int a[N];
//	int b[N];
//	int c[N];
//	for (int i = 0;i < N;i++)
//		a[i] = c[i] = b[i] = i;
//#pragma omp parallel default(shared) num_threads(NUM_THREADS)
//	{
//#pragma omp single
//		{
//			b[1] = b[1] + a[0];
//			a[0] = a[0] + c[0];
//		}
//#pragma omp barrier
//#pragma omp for schedule(static,3)
//		for (int i = 1;i < N-1;i++)
//		{
//			b[i + 1] = b[i + 1] + a[i];
//			a[i] = a[i] + c[i];
//		}
//#pragma omp barrier
//#pragma omp single
//		{
//			a[N - 1] = a[N - 1] + c[N - 1];
//		}
//#pragma omp barrier
//	}
//	for (int i = 0;i < N;i++)
//	{
//		printf("a[%d]=%d,b[%d]=%d,c[%d]=%d\n", i, a[i], i, b[i], i, c[i]);
//	}
//}
//////////////////////////PARALELIZACIJA a[i]+=a[i-1];y+=c[i] //////////////////////////
#include<stdio.h>
#include<omp.h>
#define N 10
#define NUM 10
int main(int argc, char* argv[])
{
	int a[N];
	int c[N];
	int y = 1;
#pragma omp parallel default(shared) num_threads(NUM)
	{
#pragma omp parallel schedule(satatic,1)
		for (int i = 0;i < N;i++)
		{
			a[i] = i;
			c[i] = i * 2;

		}
#pragma omp barrier
#pragma omp single
		{
			a[0] = 0;
			for (int i = 1;i < N;i++)
			{
				a[i] += a[i - 1];
			}

		}

#pragma omp barrier
#pragma omp for schedule(runtime) reduction(+:y)
		for (int i = 0;i < N;i++)
		{
			y += c[i];
		}

	}

	return 0;
}
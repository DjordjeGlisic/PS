#include<stdio.h>
#include<cuda.h>
#include<malloc.h>
#define BLOCK 256
__global__ void IzracunajNizB(int*uzorak,int N,float*izlaz)
{
    __shared__ int mem[blocDim.x+2];
    int tid=threadIdx.x+blockIdx.x*blockDim.x;
    while(tid<N+2)
    {
        mem[threadIdx.x]=uzorak[tid];
        if(threadIdx.x<2&&threadIdx.x+blockDim.x<N+2)
        {
            mem[blockDim.x+threadIdx.x]=uzorak[threadIdx.x+blockDim.x*(blockIdx.x+1)];
    
        }
        __syncThreads();
        izlaz[tid]=0.3*mem[threadIdx.x]+0.4*mem[threadIdx.x+1]+0.3*mem[threadIdx.x+2];
        tid+=blockDim.x*gridDim.x;

    }
}
int main(int argc,char*argv[])
{
    int n;
    scanf("%d",&n);
    int*A=(int*)malloc((n+2)*sizeof(int));
    float*B=(float*)malloc(n*sizeof(float));
    for(int i=0;i<n+2;i++)
        scanf("%d",&A[i]);
    int*devA;
    float*devB;
    cudaMalloc((void**)&devA,(n+2)*sizeof(int));
    cudaMalloc((void**)&devB,n*sizeof(float));
    cudaMemcpy(devA,A,(n+2)*sizeof(int),cudaMemcpyHostToDevice);
    dim3 GridDim(((n+2)+BLOCK-1)/BLOCK);
    dim3 BlockDim(BLOCK);
    IzracunajNizB<<<GridDim,BlockDim>>>(devA,n,devB);
    cudaMemcpy(B,devB,n*sizeof(float),cudaMemcpyDeviceToHost);
    printf("Rezultat obrade niza A kernel fjom je Niz B=[");
    for(int i=0;i<n;i++)
        printf(" %d ",B[i]);
    printf("]\n");
    free(A);
    free(B);
    return 0;
}
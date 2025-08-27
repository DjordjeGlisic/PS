#define BLOCK 128
#include<stdio.h>
#include<cuda.h>
__global__ void Racunaj(int*ulaz,int n,int*izlaz)
{
    __shared__ int deljiva[BLOCK+2];
    int tid=blocDim.x*blockIdx.x+threadIdx.x;
    int s=tid;

    while(s<(n-2))
    {
        if(tid<n)
        {
            deljiva[threadIdx.x]=ulaz[s];
        }
        if(threadIdx.x<2||blockDim.x+threadIdx.x<n)
        {
            deljiva[blocDim.x+threadIdx.x]=ulaz[s+threadIdx.x];
        }
        __syncThreads();
            int im=deljiva[threadIdx.x]+deljiva[threadIdx.x+1]+deljiva[threadIdx.x+2];
            if(im==0)
            {
                izlaz[s]=0;

            }
            else
            {
                int br=ulaz[threadIdx.x]*ulaz[threadIdx.x+1]*ulaz[threadIdx.x+2];
                izlaz[s]=br/im;
            }
            s+=gridDim.x*blockDim.x;
    }
}
int main(int atgc,char*argv[])
{
    int n;
    int*A=nullptr;
    scanf("%d",&n);
    A=(int*)malloc(n*sizeof(int));
    for(int i=0;i<n;i++)
    {
        scanf("%d",&A[i]);
    }
    int* B=(int*)malloc((n-2)*sizeof(int));
    int* devA;
    cudaMalloc((void**)&devA,n*sizeof(int));
    cudaMemcpy(devA,A,n*sizeof(int),cudaMemcpyHostToDevice);
    int*devB;
    cudaMalloc((void**)&devB,(n-2)*sizeof(int));
    dim3 GridDim(((BLOCK-1)+N)/BLOCK);
    dim3 BlockDim(BLOCK);
    Racunaj<<<GridDim,BlockDim>>>(devA,n,devB);
    cudaMemcpy(B,devB,(n-2)*sizeof(int),cudaMemcpyDeviceToHost);
    printf("Rezultujuci niz B=[\n");
    for(int i=0;i<n-2;i++)
        printf("%d ",B[i]);
    printf("]\n");
    cudaFree(devA);
    cudaFree(devB);
}
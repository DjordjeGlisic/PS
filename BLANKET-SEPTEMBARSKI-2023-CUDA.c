#include<stdio.h>
#include<malloc.h>
#include<cuda.h>
#define BLOK 256
__global__ void IzracunajVektor(int*uzr1,int*uzr2,int n,int* res)
{
    __shared__ int niz1[blockDim.x+2];
    __shared__ int niz2[blockDim.x+2];
    int tid=threadIdx.x+blockDim.x*blockIdx.x;
    while(tid<n)
    {
        niz1[threadIdx.x]=uzr1[tid];
        niz2[threadIdx.x]=uzr2[tid];
        if(tid<2&&(blockIdx.x+1)*blockDim.x+threadIdx.x<n)
        {
            niz1[blockDim.x+threadIdx.x]=uzr1[blockDim.x*(blockIdx.x+1)+threadIdx.x];
            niz2[blockDim.x+threadIdx.x]=uzr1[blockDim.x*(blockIdx.x+1)+threadIdx.x];
            

        }
        __syncThreads();
        int izraz1=niz1[threadIdx.x]*niz1[threadIdx.x+1]*niz1[threadIdx.x+2]+10*niz1[threadIdx.x];
        int izraz2=niz2[threadIdx.x]+niz2[threadIdx.x+1]+niz2[threadIdx.x+2];
        uzr1[tid]=(float)izraz1/(float)izraz2;
        __syncThreads();
        tid+=gridDim.x*blockDim.x;
        
    }
}
int main(int argc,char*argv[])
{
    int n;
    scanf("%d",&n);
    int*A=(int*)malloc(n*sizeof(int));
    int*B=(int*)malloc(n*sizeof(int));
    float*C=(float*)malloc((n-2)*sizeof(float));
    for(int i=0;i<n;i++)
    {
        scanf("%d",&A[i]);
        scanf("%d",&B[i]);
        
    }
    int*devA;
    int*devB;
    float*devC;
    cudaMalloc((void**)&devA,n*sizeof(int));
    cudaMalloc((void**)&devB,n*sizeof(int));
    cudaMalloc((void**)&devC,(n-2)*sizeof(float));
    cudaMemcpy(devA,A,n*sizeof(int),cudaMemcpyDeviceToHost);
    cudaMemcpy(devB,B,n*sizeof(int),cudaMemcpyDeviceToHost);
    dim3 GridDim((BLOK-1+n)/BLOK);
    dim3 BlockDim(BLOK);
    IzracunajVektor<<<GridDim,BlockDim>>>(devA,devB,n,devC);
    cudaMemcpy(devC,C,(n-2)*sizeof(float),cudaMemcpyHostToDevice);
    printf("Vektor C=[");
    for(int i=0;i<n;i++)
        printf(" %lf ",C[i]);
    printf("]\n");
    free(A);
    free(B);
    free(C);
    cudaFree(devA);
    cudaFree(devB);
    cudaFree(devC);
    return 0;
    
    
}

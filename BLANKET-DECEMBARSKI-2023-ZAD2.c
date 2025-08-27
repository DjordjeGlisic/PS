#include<stdio.h>
#include<cuda.h>
#include<malloc.h>
#define BLOCK 256
__global__ void kreirajVektor(int* matC,int*vektor,int n)
{
    __shared__ int kolona[blockDim.x+1];
    __shared__ int min=10000000;
   int blockId=blockIdx.x;
   int tid=n*threadIdx.x+blockId;
   while(blockId<n)
   {
       while((tid/n)<n)
       {
           kolona[threadIdx.x]=matC[tid];
           __syncThreads();
           atomicMin(&min,kolona[threadIdx.x]);
           __syncThreads();
           tid+=blockDim.x*n;
       }
       if(threadIdx.x==0)
       {

           vektor[blockId]=min;
       }
       blockId+=GridDim.x;

   }
}
__global__ void kreirajMatricuC(int*matA,int*matB,int n,int* matC)
{
    __shared__ int matricaA[blockDim.x][blockDim.y+1];
    __shared__ int matricaB[blockDim.x][blockDim.y+1];
    
    int xInd=blockDim.x*blockIdx.x+threadIdx.x;
    int yInd=blockDim.y*blockIdx.y+threadIdx.y;
    while(xInd<n&&yInd<n)
    {
        matricaA[threadIdx.x][threadIdx.y]=matA[yInd*n+xInd];
        matricaB[threadIdx.x][threadIdx.y]=matB[xInd*n+yInd];
        __syncThreads();
        matC[yInd*n+xInd]=matricaA[threadIdx.x][threadIdx.y]-2*matricaB[threadIdx.x][threadIdx.y]
        __syncThreads();
        xInd+=gridDim.x*BlockDim.x;
        yInd+=gridDim.y*blockDim.y;
    }
}
int main(int argc,char*argv[])
{
    int n;
    scanf("%d",&n);
    int*A=(int*)malloc(n*n*sizeof(int));
    int*B=(int*)malloc(n*n*sizeof(int));
    int*C=(int*)malloc(n*n*sizeof(int));
    int*vektor=(int*)malloc(n*sizeof(int));
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            scanf("%d",&A[i*n+j]);
            scanf("%d",&B[i*n+j]);
            
        }
    }
    int*devA;
    cudaMalloc((void**)&devA,n*n*sizeof(int));
    cudaMemcpy(devA,A,n*n*sizeof(int),cudaMemcpyHostToDevice);
    int*devB;
    cudaMalloc((void**)&devB,n*n*sizeof(int));
    cudaMemcpy(devB,B,n*n*sizeof(int),cudaMemcpyHostToDevice);
    int*devC;
    cudaMalloc((void**)&devC,n*n*sizeof(int));
    dim3 GridDim((n+BLOCK-1)/BLOCK,(n+BLOCK-1)/BLOCK);
    dim3 BlockDim(BLOCK,BLOCK);
    kreirajMatricuC<<GridDim,BlockDim>>>(devA,devB,n,devC);
    cudaMemcpy(C,devC,n*n*sizeof(int),cudaMemcpyDeviceToHost);
    int*devVekt;
    cudaMalloc((void**)&devVekt,n*sizeof(int));
    dim3 GridDim((BLOCK+n-1)/BLOCK);
    dim3 BlockDim(BLOCK);
    kreirajVektor<<GridDim,BlockDim>>>(devC,devVekt,n);
    cudaMemcpy(vektor,devVekt,n*sizeof(int),cudaMemcpyDeviceToHost);
    printf("Vektor minimalnih clanova kolona rezultujuce matrice C iznosi =[");
    for(int i=0;i<n;i++)
        printf(" %d ",vektor[i]);
    printf("]\n");
    
    

    free(A);
    free(B);
    free(C);
    free(vektor);
    cudaFree(devA);
    cudaFree(devB);
    cudaFree(devC);
    cudaFree(devVekt);
    return 0;
    
    
    
    
}
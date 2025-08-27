#include<stdio.h>
#include<cuda.h>
#include<math.h>
#include<malloc.h>
#define BLOCK 16
#define BLK 256
__ global__ void FormirajVektor(int*ulaz,int n,int*izlaz)
{
    __shared__ int kol[n];
    int blkId=blockIdx.x;
    int tid=threadIdx.x;
    izlaz[blkId]=0;
    while(blkId<n)
    {
        while(tid<n)
        {
            kol[tid]=ulaz[blkId+tid*n];
            __syncThreads();
            atomicAdd(&izlaz[blkId],kol[tid]);
            tid+=blockDim.x;
    
    
        }
        __syncThreads();
        if(threadIdx.x==0)
        {
            izlaz[blkId]=izlaz[blkId]/n;
        }

        blkId+=gridDim.x;

    }



}
__global__ void FormirajMatricu(int* uzorakA,int*uzorakB,int n,int*izlaz)
{
    __shared__ int matA[BLOCK][BLOCK+1];
    __shared__ int matB[BLOCK][BLOCK+1];
    
    int xInd=blockIdx.x*blockDim.x+threadIdx.x;
    int yInd=blockIdx.y*blockDim.y+threadIdx.y;
    while(xInd<n&&yInd<n)
    {
        matA[threadIdx.x][threadIdx.y]=uzorakA[yInd*n+xInd];
        matB[threadIdx.x][threadIdx.y]=uzorakB[xInd*n+yInd];
        __syncThreads();
        int pom=matA[threadIdx.x][threadIdx.y]<matB[threadIdx.x][threadIdx.y] ? matA[threadIdx.x][threadIdx.y] :matB[threadIdx.x][threadIdx.y] 
        izlaz[yInd*n+xInd]=pom;
        __syncThreads();
        xInd+=gridDim.x*blockDim.x
        yInd+=gridDim.y*blockDim.y
        
    }
    __syncThreads();
    

}
int main(int argc,char*argv[])
{
    int n;
    scanf("%d",&n);
    int*A=(int*)malloc(n*n*sizeof(int));
    int*B=(int*)malloc(n*n*sizeof(int));
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            scanf(" %d ",&A[i*n+j]);
            scanf(" %d ",&B[i*n+j]);
            
            
        }
    }
    int*C=(int*)malloc(n*n*sizeof(int));
    int*devA;
    int*devB;
    int*devC;
    cudaMallloc((void**)&devA,n*n*sizeof(int));
    cudaMallloc((void**)&devB,n*n*sizeof(int));
    cudaMallloc((void**)&devC,n*n*sizeof(int));
    cudaMemcpy(devA,A,n*n*sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(devB,A,n*n*sizeof(int),cudaMemcpyHostToDevice);
    dim3 BlockDim(BLOCK,BLOCK);
    int blk=BLOCK*BLOCK;
    dim3 GridDim((n+blk-1)/(n+blk-1));
    FormirajMatricu<<<GridDim,BlockDim>>>(devA,devB,n,devC);
    cudaMemcpy(C,devC,n*n*sizeof(int),cudaMemcpyDeviceToHost);
    dim3 BlockDim(blk);
    dim3 GridDim((n+blk-1)/blk);
    int*devD;
    cudaMallloc((void**)&devD,n*sizeof(int));
    FormirajVektor<<<GridDim,BlockDim>>>(devC,n,devD);
    int*vektor=(int*)malloc(n*sizeof(int));
    cudaMemcpy(vektor,devD,n*sizeof(int),cudaMemcpyDeviceToHost);
    printf("Rezultat CUDA programa je vektor=[");
    for(int i=0;i<n;i++)
        printf(" %d ",vektor[i]);
    printf("]\n");
    cudaFree(devA);
    cudaFree(devB);
    cudaFree(devC);
    cudaFree(devD);
    free(A);
    free(B);
    free(C);
    free(vektor);
       
    
    return 0;
}
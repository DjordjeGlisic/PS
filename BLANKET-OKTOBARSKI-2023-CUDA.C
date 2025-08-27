#include<cuda.h>
#include<stdio.h>
#include<malloc.h>
#define BLK 16
#define BLOK 256
__global__ void FormrmirajVektor(int*uzorak,int n,int*izlaz)
{
    __shared__ int niz[blockidx.x];
    __shared__ int min=MAX_INT;
    int blkId=blockIdx.x;
    int tid=threadIdx.x+blockIdx.x*blockDim.x;
    while(blkId<n)
    {
        while(tid<n)
        {
            niz[threadIdx.x]=uzorak[blkId*n+tid];
            __syncThreads();
            if(niz[threadIdx.x]<min)
            {
                atomicMin(&min,niz[threadIdx.x]);
            }
            __syncThreads();
            tid+=gridDim.x*blockDim.x;

        }
        if(threadIdx.x==0)
        {

            uzorak[blkId]=min;
        }
        blkId+=gridDim.x;


    }

}
__global__ void FormrmirajMatricu(int*uz1,int n,int* uz2,int*rez)
{
    __shared__ matA[BLK][BLK+1];
    __shared__ matB[BLK][BLK+1];
    int xInd=threadIdx.x+blockDim.x*blockIdx.x;
    int yInd=threadIdx.y+blockDim.y*blockIdx.y;
    while(xInd<n&&yInd<n)
    {
        matA[threadIdx.x][threadIdx.y]=uz1[yInd*n+xInd];
        matB[threadIdx.x][threadIdx.y]=uz2[xInd*n+yInd];
        __syncThreads();
        rez[yInd*n+xInd]=matA[threadIdx.x][threadIdx.y]>matB[threadIdx.x][threadIdx.y] ? matA[threadIdx.x][threadIdx.y] : matB[threadIdx.x][threadIdx.y];
        xInd+=gridDim.x*blockDim.x;
        yInd+=gridDim.y*blockDim.y;
        

    }
    
    
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
            scanf("%d",&A[i*n+j]);
            scanf("%d",&B[i*n+j]);
            
        }
    }
    int*C=(int*)malloc(n*n*sizeof(int));
    int* vekt=(int*)malloc(n*sizeof(int));
    int*devA;
    int*devB;
    int* devC;
    int* devVe;
    cudaMalloc((void**)&devA,n*n*sizeof(int));
    cudaMalloc((void**)&devB,n*n*sizeof(int));
    cudaMalloc((void**)&devC,n*n*sizeof(int));
    cudaMalloc((void**)&devVe,n*sizeof(int));
    cudaMemcpy(devA,A,n*n*sizeof(int),cudaMemcpyHostToDevice);
    cudaMemcpy(devB,B,n*n*sizeof(int),cudaMemcpyHostToDevice);
    dim3 BlockDim(BLK,BLK);
    int g=(BLOK-1+n)/BLOK;
    dim3 GridDim(g,g)
    FormrmirajMatricu<<<GridDim,BlockDim>>>(devA,n,devB,devC);
    cudaMemcpy(C,devC,n*n*sizeof(int),cudaMemcpyDeviceToHost);
    
    dim3 GridDim(g);
    dim3 BlockDim(BLOK);
    FormrmirajVektor<<<GridDim,BlockDim>>>(devC,n,devVe);
    cudaMemcpy(vekt,devVe,n*sizeof(int),cudaMemcpyDeviceToHost);
    printf("Rezultujuci vektor:=[");
    for(int i=0;i<n;i++)
        printf(" %d ",&vekt[i]);
    printf("]\n");
    cudaFree(devA);
    cudaFree(devB);
    cudaFree(devC);
    cudaFree(devVe);
    free(A);
    free(B);
    free(C);
    free(vekt);
    
    
    
    
}
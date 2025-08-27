#include<cuda.h>
#include<stdio.h>
#include<malloc.h>
#define BLK 16
#define BLOK 256
__global__ void  Redukcija(int*ulaz,int n,int*izlaz)
{
    __shared__ int parMin[blocDim.x+1];
    int i=blockIdx.x*(blockDim.x*2)+threadIdx.x;
    parMin[threadIdx.x]=ulaz[i]+ulaz[i+blockDim.x];
    __syncThreads();
    int s=blcokDim.x/2;
    while(s>0)
    {
        if(threadIdx.x<s)
        {
            parMin[threadIdx.x]=parMin[threadIdx.x]<parMin[threadIdx+s] ? parMin[threadIdx.x] : parMin[threadIdx.x+s];
        }
        __syncThreads();
        s/=2;
    }
    if(threadIdx.x==0)
    {
        izlaz[blockIdx.x]=parMin[0];
    }

}
__global__ void FormirajVektor(int*uzorak,int n,int m,int*izlaz)
{
    int xInd=blockIdx.x*blockDim.x+threadIdx.x;
    int yInd=blockIdx.y*blockDim.y+threadIdx.y;
    while(xInd<n&&yInd<m)
    {
        if(xInd==yInd)
        {
            int tid=yInd*m+xInd;
            izlaz[yInd]=uzorak[tid];
        }
        xInd+=blockDim.x*gridDim.x;
        yInd+=blockDim.y*gridDim.y;
        
    }
    

}
    
int main(int argc,char*argv[])
{
    int n;
    scanf("%d",&n);
    int*A=(int*)malloc(n*n*sizeof(int));
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<n;j++)
        {
            scanf("%d",&A[i*n+j]);
        }
    }
    int*devA;
    int*devVekt;
    cudaMalloc((void**)&devA,n*n*sizeof(int));
    cudaMalloc((void**)&devVekt,n*sizeof(int));
    cudaMemcpy(devA,A,n*n*sizeof(int),cudaMemcpyHostToDevice);
    int minimum;
    dim3 GridDim((BLOK+n-1)/BLOK,(BLOK+n-1)/BLOK);
    dim3 BlockDim(BLK,BLK);
    FormirajVektor<<<GridDim,BlockDim>>>(devA,n,n,devVekt);
    dim3 BlockDim(BLK);
    int grid=n/BLK;
    grid/=2;
    di3 GridDim(grid);
    Redukcija<<<GridDim,BlockDim>>>(devVekt,n,devVekt);
    dim3 GridDim(1);
    dim3 BlockDim(BLK);
    Redukcija<<<GridDim,BlockDim>>>(devVekt,n,devVekt);
    cudaMemcpy(&minimum,devVekt,sizeof(int),cudaMemcpyDeviceToHost);
    printf("Minimalni element glavne dijagonale matrice iznosi %d\n",minimum);
    

    
    return 0;
}
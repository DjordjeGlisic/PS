#include<stdio.h>
#include<cuda.h>
#include<mallo.c>
#define BLOK 256
#define BLK 16
__global__ void AntiAliasing(int*ulaz,n,m,int*izlaz)
{
    __shared__ int matrix[blocDim.y][blockDim.x];
    int xInd=blockIdx.x*blockDim.x+threadIdx.x;
    int yInd=blockIdx.y*blockDim.y+threadIdx.y;
    while(xInd<m&&yInd<n)
    {
        matrix[threadidx.y][threadIdx.x]=ulaz[yInd*m+xInd];
        __syncThreads();
        int tid=yInd*m+xInd;
        int sum=0;
        int count=0;
        //gore levo
        if(xInd-1>=0&&yInd-1>=0)
        {
            if(threadIdx.y-1>=0&&threadIdx.x-1>=0)
                sum+=matrix[threadIdx.y-1][threadIdx.x-1];
            else
                sum+=ulaz[(yInd-1)*m+(xInd-1)];
            count++;
        }
        //levo
        if(xInd-1>=0)
        {
            if(threadIdx.x-1>=0)
                sum+=matrix[threadIdx.y][threadIdx.x-1];
            else
                sum+=ulaz[yInd*m+(xInd-1)];
                count++;
        }
        // dole levo
        if(xInd-1>=0&& yInd+1<n)
        {
            if(threadIdx.x-1>=0&&threadIdx.y+1<n)
                sum+=matrix[threadIdx.y][threadIdx.x-1];
            else
                sum+=ulaz[(yInd+1)*m+(xInd-1)];
                count++;
        }
         //gore desno
         if(xInd+1<m&&yInd-1>=0)
         {
             if(threadIdx.y-1>=0&&threadIdx.x+1<blockDim.x)
                 sum+=matrix[threadIdx.y-1][threadIdx.x+1];
             else
                 sum+=ulaz[(yInd-1)*m+(xInd+1)];
                 count++;
         }
         //desno
         if(xInd+1<m)
         {
             if(threadIdx.x+1<blockDim.x)
                 sum+=matrix[threadIdx.y][threadIdx.x+1];
             else
                 sum+=ulaz[yInd*m+(xInd+1)];
                 count++;
         
         }
         // dole desno
         if(xInd+1<m&& yInd+1<n)
         {
             if(threadIdx.x+1<m&&threadIdx.y+1<n)
                 sum+=matrix[threadIdx.y+1][threadIdx.x+1];
             else
                 sum+=ulaz[(yInd+1)*m+(xInd+1)];
                 count++;
         
         }
         //iznad
         if(yInd-1>=0)
         {
            if(threadIdx.y-1>=0)
                sum+=matrix[threadIdx.y-1][threadId.x];
            else
                sum+=ulaz[(yInd-1)*m+xInd];
                count++;
         }
         //ispod
         if(yInd+1<n)
         {
            if(threadIdx.y+1<blockDim.y)
                sum+=matrix[threadIdx.y+1][threadId.x];
            else
                sum+=ulaz[(yInd+1)*m+xInd];
                count++;
         }
         izlaz[yInd*m+xInd]=sum/count;
         xInd+=gridDim.x*blockDim.x;
         yInd+=gridDim.y*blockDim.y

    }

}
int main(int argc,char*argv[])
{
    int n;
    int m;
    scanf("%d",&n);
    scanf("%d",&m);
    int*matrica=(int*)malloc(n*m*sizeof(int));
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            int pom;
            scanf("%d",&pom);
            matrica[i*m+j]=pom%2;
        }
    }
    int*devMatIn;
    cudaMalloc((void**)&devMatIn,n*m*sizeof(int));
    cudaMemcpy(devMatIn,matrica,n*m*sizeof(int),cudaMemcpyHostToDevice);
    int*devMatOut;
    cudaMalloc((void**)&devMatOut,n*m*sizeof(int));
    int gridx=(m+BLOK-1)/BLOK;
    int gridy=(n+BLOK-1)/BLOK;
    printf("Unesite parametar K\n");
    int k;
    scanf("%d",&k);
    dim3 GridDim(gridx,gridy);
    dim3 BlockDim(BLK,BLK);
    for(int i=0;i<k;i++)
    {
        AntiAliasing<<<GridDim,BlockDim>>>(devMatIn,n,m,devMatOut);
        cudaMemcpy(devMatIn,devMatOut,n*m*sizeof(int),cudaMemcpyDeviceToDevice);   
    }
    int*rezultujuca=(int*)malloc(n*m*sizeof(int));
    cudaMemcpy(rezultujuca,devMatOut,n*m*sizeof(int),cudaMemcpyDeviceToHost);
    printf("Nakon izostravanja matrica\n");
    for(int i=0;i<n;i++)
    {
        printf("[");
        for(int j=0;j<m;j++)
        {
            printf(" %d ",&rezultujuca[i*m+j]);
        }
        printf("]\n");
    }
    cudaFree(devMatIn);
    cudaFree(devMatOut)
    return 0;
}
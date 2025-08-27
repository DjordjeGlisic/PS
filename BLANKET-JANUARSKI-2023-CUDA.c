#include<stdio.h>
#include<cuda.h>
#define N 1000
#define BLOK 256
__global__ vid vecAddKernel(float*A_d,float*B_d,float*C_d,int n)
{
    __shared__ float mem[blockDim.x+1];
    __shared__ float sum;
    __shared__ float count;
    int tid=threadIdx.x+blockDim.x*blockIdx.x;
    while(tid<n)
    {
        sum=0;
        count=0;
        mem[threadIdx.x]=A_d[tid]+B_d[tid];
        atomicAdd(&sum,&mem[threadIdx.x]);
        atomicAdd(&count,1);
        __syncThreads();
        if(threadIdx.x==0)
        {
            int id=tid/blockDim.x;
            C_d[blockDim.x*id]=sum/count;

        }
        tid+=blockDim.x*gridDim.x;        

    }
}
__host__ void vectAdd(float*A,float*B,float*C,int n)
{
    int size=n*sizeof(float);
    float*A_d;
    float*B_d;
    float*C_d;
    cudaMalloc((void**)&A_d,size);
    cudaMalloc((void**)&B_d,size);
    cudaMalloc((void**)&C_d,size);
    cudaMemcpy(A_d,A,size,cudaMemcpyHostToDevice);
    cudaMemcpy(B_d,B,size,cudaMemcpyHostToDevice);
    int grid=n/BLOK;
    vecAddKernel<<<grid,BLOK>>>(A_d,B_d,C_d,n);
    cudaMemcpy(C,C_d,size,cudaMemcpyDeviceToHost);
    
    
    

}
int main(int argc,char*argv[])
{
    float*A=(float*)malloc(N*sizeof(float));
    float*B=(float*)malloc(N*sizeof(float));
    float*C=(float*)malloc(N*sizeof(float));
    
    for(int i=0;i<N;i++)
    {
        scanf("%d",&A[i]);
        scanf("%d",&B[i]);
        
    }
    vectAdd(A,B,C,N);
    return 0;
}
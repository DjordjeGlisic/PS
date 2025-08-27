#include<stdio.h>
#include<cuda.h>
#include<math.h>
#include<malloc.h>
#define N 100
#define BLOCK 65536
#define GRID 256
__global__ void  pronadjiNajblize(int*ulaz,int* izlaz,int N,int x,int y)
{
    int tid=threadIdx.x+blockIdx.x*blockDim.x;
    int s=tid;
    izlaz[tid]=-1;
    while(s<N)
    {
        if(ulaz[s]!=-1&&ulaz[s+N]!=-1)
        {

            int deltaX=(x-ulaz[s])*(x-ulaz[s]);
            int deltaY=(y-ulaz[s+N])*(y-ulaz[s+N]);
            int d=deltaX+deltaY;
            int ras=int(sqrt(d));
            if(izlaz[tid]==-1||izlaz[tid]>ras)
            {
                izlaz[tid]=ras;
            }
        }

        s+=GRID*BLOCK;
    }

}
__global__ void pronadjiNajblizu(int* ulaz,int N,int* izlaz)
{
    __shared__ int parNiz[N];
    parNiz[threadIdx.x]=ulaz[blockIdx.x*(blockDim.x*2)+threadIdx.x]+ulaz[blockIdx.x*(blockDim.x*2)+threadIdx.x+blockDim.x];
    __syncThreads();
    int s=blockDim.x/2;
    while(s>0)
    {
        if(threadIdx.x<s)
        {
            parNiz[threadIdx.x]+=parNiz[threadIdx.x+s];
        }
        s/=2;
        __syncThreads();
    }
    if(threadIdx.x==0)
    {
        izlaz[blockIdx.x]=parNiz[0];
    }
}
__host__ int nadjiTacku(int ras,int x,int y)
{
    int index=-1;
    for(int i=0;i<N;i++)
    {
        int deltaX=(x-ulaz[i])*(x-ulaz[i]);
            int deltaY=(y-ulaz[i+N])*(y-ulaz[i+N]);
            int d=deltaX+deltaY;
            int rast=int(sqrt(d));
            if(rast==ras)
            {
                index=i;
                break;
            }
    }
    
    return index;
}
int main(int argc,char*argv[])
{
    int*tacke=(int*)malloc(N*2*sizeof(int));
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(i==0)
                printf("Unesite X koordinatu tacke %d\n",j);
            else
                printf("Unesite Y koordinatu tacke %d\n",j);

            scanf("%d",&tacke[i*N+j]);
        }
    }
    int x;
    int y;
    printf("Unesite koordiatne tacke za koju se traze 10 najblizih");
    scanf("%d",&x);
    scanf("%d",&y);
    int*devMat;
    cudaMalloc((void**)&devMat,2*N*sizeof(int));
    cudaMemcpy(devMat,tacke,2*N*sizeof(int),cudaMemcpyHostToDevice);
 
    int*devRas;
    cudaMalloc((void**)devRas,GRID*BLOK*sizeof(int));
    
    printf("10 najblizih tacaka tacki (%d,%d) su\n",x,y);
    int* devNiz;
    cudaMalloc((void**)&devNiz,GRID*sizeof(int));
    for(int i=0;i<10;i++)
    {
        pronadjiNajblize<<<GRID,BLOK>>>(devMat,devRas,N,x,y);
        int grid=N/BLOK/2;
        pronadjiNajblizu<<<grid,BLOK>>>(devRas, N,devNiz);
        pronadjiNajblizu<<<1,BLOK>>>(devNiz,N,devNiz);
        int ras;
        cudaMemcpy(&ras,devNiz,1*sizeof(int),cudaMemcpyDeviceToHost);
        int index=nadjiTacku(ras,x,y);
        
        printf("Tacka%d=(%d,%d)\n",index,tacke[index],tacke[N+index]);
        tacke[i]=-1;
        tacke[i+N]=-1;
        cudaMemcpy(devMat,tacke,2*N*sizeof(int),cudaMemcpyHostToDevice);
    }
    
    cudaFree(devMat);
    cudaFree(devRas);
    cudaFree(devNiz);
}
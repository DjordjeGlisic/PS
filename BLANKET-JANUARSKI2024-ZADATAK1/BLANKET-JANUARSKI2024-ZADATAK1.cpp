// BLANKET-JANUARSKI2024-ZADATAK1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include<malloc.h>
#include <stdio.h>
#include<mpi.h>
#include<math.h>
#define K 7
#define N 4

int main(int argc,char*argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
   int* B = (int*)malloc(N * sizeof(int));
    int* parcA = (int*)malloc(N * pow(2, rank) * sizeof(int));
    if (rank == 0)
    {
        int Ap[K][N] = { {1,1,1,0},{0,1,0,0},{1,0,1,0},{1,1,0,0 },{0,0,1,1},{1,1,0,1},{0,1,0,1} };
        int* A = (int*)malloc(K * N * sizeof(int));
        for (int i = 0;i < K;i++)
        {
            for (int j = 0;j < N;j++)
                A[i * N + j] = Ap[i][j];
        }
        
        int br = 1;
        for (int i = 0;i < N;i++)
            B[i] = br++;
        for (int i = 0;i < N;i++)
            parcA[i] = A[i];
        int pocetna = 1;
       
        for (int i = 1;i < size;i++)
        {
            int kolkoVrsta = pow(2, i);
            
            MPI_Send(&A[N * pocetna], kolkoVrsta * N, MPI_INT, i, i, MPI_COMM_WORLD);
           
            pocetna += kolkoVrsta;

        }
    }
    else
    {
        MPI_Recv(parcA, pow(2, rank) * N * sizeof(int), MPI_INT, 0, rank, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
    }
    
    MPI_Bcast(B, N, MPI_INT, 0, MPI_COMM_WORLD);
    /*printf("Proces%d primio je podmatricu\n", rank);
    for (int i = 0;i < pow(2, rank);i++)
    {
        printf("[");
        for (int j = 0;j < N;j++)
        {
            printf("%d ", parcA[i * N + j]);
        }
        printf("]\n");
    }*/
    
    int* parcD = (int*)malloc(pow(2, rank) * sizeof(int));
    for (int i = 0;i < pow(2, rank);i++)
    {
        parcD[i] = 0;
        for (int j = 0;j < N;j++)
        {
            parcD[i] += parcA[i * N + j] * B[j];
        }

    }
    int* D = nullptr;
    if (rank != size - 1)
    {
        MPI_Send(parcD, pow(2, rank), MPI_INT, size - 1, rank, MPI_COMM_WORLD);
    }
    else
    {
        
        D = (int*)malloc(K * sizeof(int));
        int numV = size-1;
        int numK = pow(2, size);
        int* mat = (int*)malloc(numV *numK * sizeof(int));
        for (int i = 0;i < size-1;i++)
        {
            int poc = i * numK;
            MPI_Recv(&mat[poc], pow(2, i), MPI_INT, i, i, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

        }
        int index = 0;
        for (int i = 0;i < size;i++)
        {
            if (i != size - 1)
            {
                for (int j = 0;j < pow(2, i);j++)
                {
                    int vrsta = i * numK;
                    D[index] = mat[vrsta + j];
                    index++;
                }
            }
            else
            {
                for (int j = 0;j < pow(2, size - 1);j++)
                {
                    D[index] = parcD[j];
                    index++;
                }

            }
        }
        printf("Proces%d stampa proizvod mnozenja matrice i vektora\n",rank);
        printf("[");
        for (int i = 0;i < K;i++)
        {
            printf("%d ", D[i]);
        }
        printf("]\n");
    }
    MPI_Finalize();


}

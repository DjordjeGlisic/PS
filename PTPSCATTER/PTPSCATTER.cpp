#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int data[100];  // Niz podataka koji će biti distribuiran
    MPI_Status status;

    int num_data = size;  // Početni broj podataka kod rank 0
    int local_data[100]; // Privremeni niz za prijem podataka

    if (rank == 0) {
        // Inicijalizacija niza podataka
        for (int i = 0; i < size; i++) {
            data[i] = i;  // Proces 0 poseduje sve podatke
        }
    }

    for (int i = 0; i < log2(size); i++) {
        int step = 1 << i;  // 2^i

        if (rank % (2 * step) == 0) {  // Proces koji šalje
            if (rank + step < size) {
                int send_count = num_data / 2;  // Polovina podataka se šalje
                MPI_Send(&data[send_count], send_count, MPI_INT, rank + step, 0, MPI_COMM_WORLD);
                num_data -= send_count;  // Preostali podaci u trenutnom procesu
            }
        }
        else if (rank % step == 0) {  // Proces koji prima podatke
            int recv_count = num_data / 2;
            MPI_Recv(local_data, recv_count, MPI_INT, rank - step, 0, MPI_COMM_WORLD, &status);
            num_data = recv_count;  // Broj podataka koje proces sada ima

            // Kopiramo primljene podatke u lokalni niz
            for (int j = 0; j < recv_count; j++) {
                data[j] = local_data[j];
            }
        }
    }

    // Ispis rezultata
    printf("Proces %d ima podatke: ", rank);
    for (int i = 0; i < num_data; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}

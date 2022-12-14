#define _GNU_SOURCE
#include <math.h>
#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "aux.h"

#define ROOT 0
#define NUM_THREADS 4

void totalDivs(int *array, int size) {
    int number, divisor, k;
    int sqrtNumber;
#pragma omp parallel for num_threads(NUM_THREADS) private(number, divisor, k, sqrtNumber)
    for (int i = 0; i < size; i++) {
        number = array[i];
        k = 2;
        sqrtNumber = sqrt(number);
#pragma omp parallel for num_threads(NUM_THREADS) reduction(+:k)
        for (divisor = 2; divisor <= sqrtNumber; divisor++) {
            if (number % divisor == 0) {
                k++;
                if (divisor != (number / divisor))
                    k++;
            }
        }
        array[i] = k;
    }
}

int main(int argc, char **argv) {
    int rank, size;
    double start, end;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n, chunk;
    int *data = NULL;
    int *chunkData = NULL;

    if (argc != 2) {
        if (rank == ROOT)
            fprintf(stderr, "Uso: mpirun <size> %s <filename>\n", argv[0]);
        MPI_Finalize();
        exit(0);
    }

    if (rank == ROOT) {
        n = getNLines(argv[1]);
        chunk = n / size;
        data = calloc(n, sizeof(int));
        getData(argv[1], data);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    MPI_Bcast(&chunk, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
    chunkData = calloc(chunk, sizeof(int));

    MPI_Scatter(data, chunk, MPI_INT, chunkData, chunk, MPI_INT, ROOT, MPI_COMM_WORLD);
    printf("Processo: %d -> Data: %d numbers\n", rank, chunk);
    totalDivs(chunkData, chunk);
    MPI_Gather(chunkData, chunk, MPI_INT, data, chunk, MPI_INT, ROOT, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == ROOT) {
        double runtime = end - start;
        printf("Runtime: %fs\n", runtime);
        saveData(data, n);
        saveTime(size, runtime);
    }

    if (data)
        free(data);
    free(chunkData);
    MPI_Finalize();

    return 0;
}
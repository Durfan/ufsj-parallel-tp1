#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "aux.h"

#define ROOT 0

void totalDivs(int *array, int size) {
    int number, divisor, k;
    for (int i = 0; i < size; i++) {
        number = array[i];
        k = divisor = 2;
        while (divisor <= sqrt(number)) {
            if (number % divisor == 0) {
                k++;
                if (divisor != (number / divisor))
                    k++;
            }
            divisor++;
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

    int nlines;
    int chunk;
    int *data = NULL;
    int *chunkData = NULL;

    if (argc != 2) {
        if (rank == ROOT)
            fprintf(stderr, "Uso: mpirun <size> %s <filename>\n", argv[0]);
        MPI_Finalize();
        exit(0);
    }

    if (rank == ROOT) {
        nlines = getNLines(argv[1]);
        chunk = nlines / size;
        data = calloc(nlines, sizeof(int));
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
        printf("Runtime: %fs\n", end - start);
        saveData(data, nlines);
    }
    free(data);
    free(chunkData);

    MPI_Finalize();
    return 0;
}
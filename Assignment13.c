#include <cstdlib>
#include <cstdio>
#include "mpi.h"

int** malloc_array(long int n) {
    int** matrix = new int*[n];
    for (int i = 0; i < n; i++)
        matrix[i] = new int[n];
    return matrix;
}

int** rand_init_matrix(int **matrix, long int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = rand() / 1000.0;
	return matrix;
}

int** zero_init_matrix(int **matrix, long int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matrix[i][j] = 0.0;
	return matrix;
}

int main(int argc, char **argv) {
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// matrix initialization for each process
	int n = 500;
	int **A, **B, **C;

	A = malloc_array(n);
    B = malloc_array(n);
    C = malloc_array(n);

	A = rand_init_matrix(A, n);
	B = rand_init_matrix(B, n);
	C = zero_init_matrix(C, n);

	MPI_Barrier(MPI_COMM_WORLD); //barrier process synchronization
	
	double start_time = MPI_Wtime(); // start timing for each process
	
	// matrix multiplication
	for (int i = 0; i < n; i++)
		for (int k = 0; k < n; k++)
			for (int j = 0; j < n; j++)
				C[i][j] += A[i][k] * B[k][j];
	
	// end of timing
	double stop_time = MPI_Wtime();
	
	// output the execution time of matrix multiplication at each process
	printf("process: %d, execution time: %.5f s", rank, (stop_time - start_time));
	MPI_Finalize();
}
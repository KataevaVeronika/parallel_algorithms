#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <cmath>

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;

	for (int i = 0; i <= 6; i++) {
		int n = 10;
		double array_size = pow(10, i);
		// size of array in bytes
		double L = array_size * sizeof(double); 
		// memory allocation for array
		double *A = (double*)malloc(L);

		int tag_1 = 0;
		int tag_2 = 1;

		double start_time = MPI_Wtime();
	
		for (int i = 0; i < n; i++){
			if (rank == 0) {
				MPI_Send(A, array_size, MPI_DOUBLE, 1, tag_1, MPI_COMM_WORLD);
				MPI_Recv(A, array_size, MPI_DOUBLE, 1, tag_2, MPI_COMM_WORLD, &status);
			}
			if(rank == 1){
				MPI_Recv(A, array_size, MPI_DOUBLE, 0, tag_1, MPI_COMM_WORLD, &status);
				MPI_Send(A, array_size, MPI_DOUBLE, 0, tag_2, MPI_COMM_WORLD);
			}
		}
		if (rank == 0) {
			double stop_time = MPI_Wtime();
			double elapsed_time = stop_time - start_time;
			double bandwidth = 2 * n * L / elapsed_time;
			double latency = elapsed_time / (2.0 * n);
			
			printf("Array size = %.0f; ", array_size);
			printf("bandwidth = %.4f; ", bandwidth);
			printf("latency = %.9f\n", latency);
			free(A);
		}
	}
	MPI_Finalize();
	return 0;
}
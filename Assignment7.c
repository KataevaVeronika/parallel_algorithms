#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int* create_array(long int n) {
    int* matrix = new int[n];
    for (long int i = 0; i < n; i++) {
        matrix[i] = 1;
		// matrix[i] = rand() / 100;
    }
    return matrix;
}

int dot(int *A, int *B, int n) {
    int dot_product = 0;
    for (int i = 0; i < n; i++) {
        dot_product += A[i] * B[i];
    }
    return dot_product;
}

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);
    double prod;
    int rank, size, *A, *B;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  	int chunk_size = N / size;

    if (rank == 0) {
        A = create_array(N);
		// for (int i = 0; i < N; i++) {
        // 	printf("%d ", A[i]);
    	// }
		// cout << endl;
	    B = create_array(N);
		// for (int i = 0; i < N; i++) {
        // 	printf("%d ", B[i]);
    	// }
		// cout << endl;
    } 
  	int *sub_A = (int*)malloc(sizeof(int) * chunk_size);
  	int *sub_B = (int*)malloc(sizeof(int) * chunk_size);
  	MPI_Scatter(A, chunk_size, MPI_INT, sub_A, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
  	MPI_Scatter(B, chunk_size, MPI_INT, sub_B, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
  	
	double local_dot;
  	local_dot = dot(sub_A, sub_B, chunk_size);
  	MPI_Reduce(&local_dot, &prod, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  
	if (rank == 0) {
		printf("Dot product = %f\n", prod);
	}
	MPI_Finalize();

    return 0;
}
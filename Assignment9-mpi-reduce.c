/* Parallel dot product */
#include <mpi.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
	int rank, size, n, chunk_size, sum, *array;
	double start_time;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0) {
		start_time = MPI_Wtime();
		n = 1000000 * size;
		array = new int[n];
		for (int i = 0; i < n; ++i) {
			array[i] = 1;
		}		
		chunk_size = n / size;
	}
    
    MPI_Bcast(&chunk_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int *chunk = new int[chunk_size];
  	MPI_Scatter(array, chunk_size, MPI_INT, chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = 0;

	for (int i = 0; i < chunk_size; i++) {
		local_sum += chunk[i];
	}

  	MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  
	if (rank == 0) {
		printf("Time: %.5f s\n", MPI_Wtime() - start_time);
		cout << "Sum: " << sum << endl;
	}
	MPI_Finalize();

    return 0;
}
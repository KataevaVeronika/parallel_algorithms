#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
	int rank, size, n, chunk_size;
	int* array;
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

	int* chunk = new int[chunk_size];
	MPI_Scatter(array, chunk_size, MPI_INT, chunk, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

	int total = 0;

	for (int i = 0; i < chunk_size; i++) {
		total += chunk[i];
	}

	int temp;
	for (int i = 1; i < size; i *= 2) {
		if (rank % (i * 2) != 0) {
			MPI_Send(&total, 1, MPI_INT, rank - i, 0, MPI_COMM_WORLD);
			break;
		}
		else if (rank + i < size) {
			MPI_Recv(&temp, 1, MPI_INT, rank + i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			total += temp;
		}
	}
	if (rank == 0) {
		printf("Time: %.5f s\n", MPI_Wtime() - start_time);
		cout << "Sum: " << total << endl;
	}
	MPI_Finalize();
	return 0;
}
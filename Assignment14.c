#include <stdio.h>
#include <algorithm> 
#include "mpi.h"
#define n 1000
using namespace std;
void custom_max(void *a, void *b, int *l, MPI_Datatype *type) {
	for (int i = 0; i < *l; i++)
		((int*)b)[i] = max(((int*)a)[i], ((int*)b)[i]);
}

int main(int argc, char **argv)
{
	int rank, size, i;
	int a[n];
	int b[n];
	int c[n];
	MPI_Init(&argc, &argv);
	MPI_Op op;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	for (i = 0; i < n; i++) a[i] = i + rank + 1;
	MPI_Op_create(&custom_max, 1, &op);
	MPI_Reduce(a, b, n, MPI_INT, op , 0, MPI_COMM_WORLD);
	MPI_Op_free(&op);
	MPI_Reduce(a, c, n, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	printf("process %d a[0] = %d\n", rank, a[0]);
	printf("process %d a[500] = %d\n", rank, a[500]);
	if (rank == 0) {
		printf("b[0] = %d\n", b[0]);
		printf("c[0] = %d\n", c[0]);
		printf("b[500] = %d\n", b[500]);
		printf("c[500] = %d\n", c[500]);
	}
	MPI_Finalize();
}
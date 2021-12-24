#include <stdio.h>
#include <string.h>
#include "mpi.h"
#include <cstdlib>
#include <iostream>
#define BUFSIZE 100

using namespace std;

int main(int argc, char **argv)
{
	int num, sum;
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_File fh;
	char buf[BUFSIZE];
	const char *str = "some random words";
	size_t str_length = strlen(str);
	printf("string = %s\n", str);
	printf("string len = %d\n", str_length);

	MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
	MPI_File_write(fh, str, str_length, MPI_CHAR, &status);
	MPI_File_close(&fh);
	
	MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_RDONLY | MPI_MODE_DELETE_ON_CLOSE, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	
	sum = 0;
	do {
		MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
		MPI_Get_count(&status, MPI_CHAR, &num);
		printf("read buf = %s \n", buf);
		sum += num;
	} while (num >= BUFSIZE);
	
	MPI_File_close(&fh);
	printf("num = %d\n", num);
	MPI_Finalize();
}
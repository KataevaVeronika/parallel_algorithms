#include <stdio.h>
#include "mpi.h"
#define MAX_LENGTH 256

int main(int argc, char **argv)
{
	int r;
	MPI_Init(&argc, &argv);
	char str[MAX_LENGTH];
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Status status;
	MPI_Comm intercomm;
	MPI_Open_port(MPI_INFO_NULL, port_name);
	printf("portname: %s\n", port_name);
	MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
	MPI_Recv(&str, MAX_LENGTH, MPI_INT, 0, 0, intercomm, &status);
	MPI_Comm_free(&intercomm);
	MPI_Close_port(port_name);
	printf("Клиент отправил %s\n", str);
	MPI_Finalize();
	return 0;
}
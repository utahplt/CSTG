#include <cstdio>
#include <mpi.h>
#include "Stacktracer.h"

void foo(){
	int rank, size;
	char tag[100];
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	printf( "Hello world from process %d of %d\n", rank, size);
        sprintf(tag, "rank_%d", rank);
	printStacktrace("stack_trace.txt", tag); // instrumentation
}

int main(int argc, char *argv[]){
	MPI_Init (&argc, &argv);
	foo();
	MPI_Finalize();	
	return 0;
}

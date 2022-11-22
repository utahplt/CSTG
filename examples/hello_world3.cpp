#include <cstdio>
#include <mpi.h>
#include "Stacktracer.h"

cstg acstg; // cstg object variable

void bar(){
	int rank, size;
	char tag[100];
	MPI_Comm_rank (MPI_COMM_WORLD, &rank);
	MPI_Comm_size (MPI_COMM_WORLD, &size);
	printf( "Hello world from process %d of %d\n", rank, size);
        sprintf(tag, "rank_%d", rank);
	acstg.addStacktrace(tag); // instrumentation
}

int main(int argc, char *argv[]){
	MPI_Init (&argc, &argv);
	bar();
	MPI_Finalize();	
	acstg.write("stack_trace3.dot"); // CSTG file creation
	acstg.generatePDF("stack_trace3.dot", "stack_trace_3.pdf");
	return 0;
}

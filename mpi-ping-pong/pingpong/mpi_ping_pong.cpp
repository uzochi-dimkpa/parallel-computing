#include <mpi.h>
#include <iostream>

int main (int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr<<"usage: mpirun "<<argv[0]<<" <value>"<<std::endl;
    return -1;
  }
  
  MPI_Init (&argc, &argv); int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size); MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  /**/
  double out;
  if (rank == 0) {
    double val = atof(argv[1]);
    
    MPI_Send(&(val), 1, MPI_DOUBLE, 1, 123, MPI_COMM_WORLD);
    MPI_Status s0;
    MPI_Recv(&(val), 1, MPI_DOUBLE, 1, 123, MPI_COMM_WORLD, &s0);
    
    std::cout << val << std::endl; // answer
  } else if (rank == 1) {
    MPI_Status s1;
    MPI_Recv(&(out), 1, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD, &s1); out += 2.;
    MPI_Send(&(out), 1, MPI_DOUBLE, 0, 123, MPI_COMM_WORLD);
  }
  /**/
  
  MPI_Finalize();
  
  return 0;
}

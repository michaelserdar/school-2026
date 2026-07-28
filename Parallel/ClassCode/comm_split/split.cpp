#include <mpi.h>
#include <iostream>

using namespace std;

// int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm * newcomm)

int main(int argc, char **argv)
{
  int size, rank;
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  // input: MPI_COMM_WORLD; color = ?, key = ? , newcomm ?
  MPI_Comm subset;

  int color, key;
  color = rank & 1;
  key   = rank;

  int newsize, newrank;

  MPI_Comm_split(MPI_COMM_WORLD, color, key, &subset);
  MPI_Comm_size(subset, &newsize);
  MPI_Comm_rank(subset, &newrank);
  cout << rank << ": I am now in a second communicator (subset): " << newsize << "," << newrank << endl;

  MPI_Barrier(subset);
  
  MPI_Comm_free(&subset);
  MPI_Finalize();

  return 0;
}


#include <mpi.h>
#include <iostream>

using namespace std;

// int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm * newcomm)

int main(int argc, char **argv)
{
  int size, rank;
  int P, Q, p, q;
  int rowsize, colsize;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(0 == rank)
  {
    do
    {
      cout << "Enter grid shape (multiplies out to the size=" << size << "): ";
      cin >> P;
      cin >> Q;
    }
    while (P*Q != size);
  }

  int share_vector[2];
  if(0 == rank)
  {
     share_vector[0] = P;
     share_vector[1] = Q;;
  }
   
  MPI_Bcast(share_vector, 2, MPI_INT, 0, MPI_COMM_WORLD);
  if(0 != rank)
  {
     P          = share_vector[0];
     Q          = share_vector[1];
  }
  
  MPI_Comm rowcomm, colcomm;
  
  // one use case is
  // PxQ=size of MPI_COMM_WORLD; grid of shape PxQ
  MPI_Comm_split(MPI_COMM_WORLD, rank/Q, rank, &rowcomm);
  MPI_Comm_split(MPI_COMM_WORLD, rank%Q, rank, &colcomm);
  
  MPI_Comm_size(rowcomm, &rowsize); MPI_Comm_rank(rowcomm, &q);
  MPI_Comm_size(colcomm, &colsize); MPI_Comm_rank(colcomm, &p);
  cout << rank << ": my grid location is ("<<p<<","<<q<<")" << endl;

  MPI_Comm_free(&rowcomm);
  MPI_Comm_free(&colcomm);
  MPI_Finalize();

  return 0;
}


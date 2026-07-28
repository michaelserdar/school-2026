#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

//#define DEBUG_PROGRAM

using namespace std;

template<typename T>
void print_matrix(T *matrix, int M, int N, int rowlow, int rowhigh, int collow, int colhigh)
{
  int start_i = (rowlow < 0) ? 0 : rowlow;
  int end_i   = (rowhigh <= M) ? rowhigh : M; if(end_i < 0) end_i = 0;

  int start_j = (collow < 0) ? 0 : collow;
  int end_j   = (colhigh <= N) ? colhigh : N; if(end_j < 0) end_j = 0;
  
  for(int i = start_i; i < end_i; ++i)
  {
    cout << start_i << ": ";
    for(int j = start_j; j < end_j; ++j)
      cout << matrix[i*N+j] << " ";
    cout << endl;
  }
}

int main(int argc, char **argv)
{
   int M=0, N=0;
   int size=0, rank=-1;

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   // Create derived datatype for a column: M elements, blocklength 1, stride (N+2)

   M = 10; N = 10;

   int *matrix = new int[M*N];

   if(0 == rank)
   {
     for(int i = 0; i < M; ++i)
        for(int j = 0; j < N; ++j)
	  matrix[i*N+j] = 1000*i + j; 

      cout << "Output matrix from process " << rank << ":" << endl;
      print_matrix(matrix, M, N, 0, M, 0, N);
      cout << endl;
   }
   
   MPI_Barrier(MPI_COMM_WORLD); // make sure printing is done from process 0
   
   //int MPI_Type_vector(int count, int blocklength, int stride,
   //                    MPI_Datatype oldtype, MPI_Datatype * newtype)

   MPI_Datatype column_type;
   MPI_Type_vector(M, 1, N, MPI_INT, &column_type);   // M row column from MxN row-major matrix.
   MPI_Type_commit(&column_type);
   
   if(0 == rank)
   {
      MPI_Request request;

      //int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest,
      //             int tag, MPI_Comm comm, MPI_Request *request)

      MPI_Isend(&matrix[0*N+0], 1, column_type, 1, 0, MPI_COMM_WORLD, &request); // column j=3
      // work possible in between, but none here.
      MPI_Wait(&request, MPI_STATUS_IGNORE);

   }
   else if(1 == rank)
   {
      MPI_Request request;

      for(int i = 0; i < M; ++i)
        for(int j = 0; j < N; ++j)
	  matrix[i*N+j] = 0;

      // int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
      //                MPI_Comm comm, MPI_Request *request)
      MPI_Irecv(&matrix[0*N+(N-1)], 1, column_type, 0, 0, MPI_COMM_WORLD, &request);
      MPI_Wait(&request, MPI_STATUS_IGNORE);

      cout << "Output matrix from process " << rank << ":" << endl;
      print_matrix(matrix, M, N, 0, M, 0, N);
   }
   
   MPI_Type_free(&column_type);
   
   delete[] matrix; matrix = nullptr;
   MPI_Finalize();

   return 0;
}



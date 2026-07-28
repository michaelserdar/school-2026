#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

#define PRINT_ITERATIONS
//#define DEBUG_PROGRAM

#include "haloarray.h"

using namespace std;

typedef haloArray<char> charArray;

// forward decls: mapping functions for linear distributions used in Life
// 1D just have complete rows in processes, and split evenly as possible
//
void partitionSize(int P, int N, int p, int &entries);

// 1D life updater
void do_update(MPI_Comm comm,  charArray &old_map, charArray &new_map, int m, int N, int my_rank);
void print_array(int iteration, const charArray &array, int M, int N, int Mdelta, int Ndelta);

//
// KNOWN LIMITATIONS:
// there must be at least one row in each process for this to work.
// there must be at least three total rows in the life world.
//
// M>=P, then there is always one row per process. But M>=3 also needed.
//

int main(int argc, char **argv)
{
   int M=0, N=0, iterations=0;
   int size=0, rank=-1;
   charArray *global_array = nullptr; // used only in process 0
   int *displs =nullptr, *thecounts=nullptr; // used in process zero for Scatterv/Gatherv

   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   if(0 == rank)
   {
      string filename;
      if(argc < 3)
      {  
         do
         { 
            cout << "Enter Iterations: ";
            cin >> iterations;
         }
         while(iterations < 0);

         do
         {
	    cout << "File name to read: ";
	    cin >> filename;

         } while(0);

      }
      else
      {
	 iterations = atoi(argv[1]);
	 filename = argv[2];
      }
#ifdef DEBUG_PROGRAM      
      cout << rank << ": Iterations = " << iterations << ", filename = " << filename << endl;
#endif      
      
      // we need to read the initial map from a file, right?
      ifstream file(filename);
      if (file.is_open())
      {
        string line;

	getline(file, line);
        stringstream ss(line);
	ss >> M >> N;

#ifdef DEBUG_PROGRAM	
	cout << "M = " << M << ", N = " << N << endl;
#endif	

	// process zero makes a global array (not scalable choice):
	global_array = new charArray(M,N,0,0); // no halos!
	
        for (int row = 0; getline(file, line); ++row)
	{
   	    const char *str = line.c_str();  // must update this reference each time!
	    
	    if(row == M)
	    {
	      cout << "Reached end of file with too many rows! Ignoring extra data!" << endl;
	      break;
	    }

            cout << row << ": " << line << endl;

            int max_length = ((int)line.length() > N) ? N : (int)line.length();
           
	    for(int k = 0; k < max_length; ++k)
	      (*global_array)[row,k] = (str[k] == '*') ? '*' : ' ';

	    for(int k = max_length; k < N; ++k)
	      (*global_array)[row,k] = ' ';     // pad to the right with empty if a line is short
		
        }
	
        file.close();
      }
      else
      {
         cerr << "Unable to open file";
 	 MPI_Abort(MPI_COMM_WORLD, -1);
      }
   }

#ifdef DEBUG_PROGRAM
   if(0 == rank)
   {
      cout << "This is the global array:" << endl;
      print_array(-100, *global_array, M, N, 0, 0);
   }
#endif   
   
   int share_vector[3];
   if(0 == rank)
   {
      share_vector[0] = M;
      share_vector[1] = N;
      share_vector[2] = iterations;
   }
   
   MPI_Bcast(share_vector, 3, MPI_INT, 0, MPI_COMM_WORLD);
   if(0 != rank)
   {
     M          = share_vector[0];
     N          = share_vector[1];
     iterations = share_vector[2];
   }

#ifdef DEBUG_PROGRAM   
   cout << rank << ": M = " << M << ", N = " << N << ", iterations = " << iterations << endl;
#endif   

   if(iterations > 0)
   {
     // all processes know the M, N size of the life world.
     // and # of iterations to do.

     int my_rows; // linear load balanced distribution gives me this many rows:
     partitionSize(size, M, rank, my_rows);

#ifdef DEBUG_PROGRAM
     cout << rank  << ": my_rows=" << my_rows << ", N = " << N << endl;
#endif     

     // create the old and new worlds
     charArray *new_world = new charArray(my_rows,N,1,0);  // row halos only of depth 1 (top and bottom)
     charArray *old_world = new charArray(my_rows,N,1,0); 

#ifdef DEBUG_PROGRAM
     cout << "Array sizes: " << (my_rows+2)*N << endl;
     
     cout << "Filling local buffers with @, #" << endl;
     for(int ii = -1; ii <= my_rows; ++ii)
       for(int jj = 0; jj < N; ++jj)
       {
	 (*old_world)[ii,jj] = '@';
	 (*new_world)[ii,jj] = '#';
       }
     cout << "Done filling local buffers with @, #" << endl;
     cout << "old_world size: " << (*old_world).size() << endl;
     cout << "new_world size: " << (*new_world).size() << endl;
     
#endif
     
     // Split the world too using MPI_Scatterv():
     //int MPI_Scatterv(const void *sendbuf, const int *thecounts, const int *displs,
     //              MPI_Datatype sendtype, void *recvbuf, int recvcount,
     //              MPI_Datatype recvtype, int root, MPI_Comm comm)

     if(0 == rank)
     {
       displs    = new int[size];
       thecounts = new int[size];
	 
     // let process zero compute the displacements & counts
     // these arguments only value at the root (zero)
       displs[0] = 0;
       thecounts[0] = my_rows*N;
       for(int p = 1; p < size;  ++p)
       { 
         partitionSize(size, M, p, thecounts[p]); thecounts[p] *= N;

         displs[p] = displs[p-1] + thecounts[p-1];
       }
     }

#ifdef DEBUG_PROGRAM
     if(0 == rank)
     {
       cout << "displs:" <<endl;
       for(int ii = 0; ii < size; ++ii)
	 cout << "displs[" << ii << "] = " << displs[ii] << endl;
       cout << endl;

       cout << "thecounts:" <<endl;
       for(int ii = 0; ii < size; ++ii)
	 cout << "thecounts[" << ii << "] = " << thecounts[ii] << endl;
       cout << endl;

     }
     
     if(0 == rank)
     {  
       cout << "Before Scatterv (old_world): " << (*old_world).size() << endl;
        print_array(-1, *old_world, my_rows, N, 1, 0);
       cout << "Before Scatterv, but after print_array, (old_world): " << (*old_world).size() << endl;
     }
#endif     

     // receive into old_world at the halo-adjusted offset (skip top row)
     // note: one can only derference global_array in rank=0, hence the guard!
     void* sendbuf = (rank == 0) ? static_cast<void*>(&(*global_array)[0,0]) : nullptr;
     MPI_Scatterv(sendbuf, thecounts, displs, MPI_CHAR, &((*old_world)[0,0]), my_rows*N,
		  MPI_CHAR, 0, MPI_COMM_WORLD);
     // keep displs and thecounts for printing function to be done later!

#ifdef DEBUG_PROGRAM
     if(0 == rank)
     {  
       cout << "After Scatterv (old_world):" << (*old_world).size() <<  endl;
        print_array(-1, *old_world, my_rows, N, 1, 0);

     }
#endif     

     double t_min_local = 0.0, t_max_local = 0.0, t_sum_local = 0.0;

     for(int i = 0; i < iterations; ++i)
     {
#ifdef DEBUG_PROGRAM
       cout << "Iteration start: " << i << "old_world size: " << (*old_world).size() << ",  new_world size: " << (*new_world).size() << endl;
#endif

        double t_start = MPI_Wtime(), t_end;		      
        do_update(MPI_COMM_WORLD, *old_world, *new_world, my_rows, N, rank);
	t_end = MPI_Wtime();
	
	double delta_t = t_end - t_start;
	t_sum_local += delta_t;
	if(i == 0) t_min_local = delta_t; else if(t_min_local > delta_t) t_min_local = delta_t;
	if(t_max_local < delta_t) t_max_local = delta_t;

#ifdef PRINT_ITERATIONS
	// if we want to print, we need to aggregate map back to process 0,
	// and print... use MPI_Gatherv()...

	//int MPI_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
        //      void *recvbuf, const int recvcounts[], const int displs[],
        //      MPI_Datatype recvtype, int root, MPI_Comm comm)

	// skip the halo at top when gathering:

#ifdef DEBUG_PROGRAM
	if(0 == rank)
	{
	    for(int ii = 0; ii < M; ++ii)
    	      for(int jj = 0; jj < N; ++jj)
		(*global_array)[ii,jj] = 'x';
	}

	cout << rank << ": Before MPI_Gatherv()" << endl;
#endif	

        // note: one can only derference global_array in rank=0, hence the guard!
	void* recvbuf = (rank == 0) ? static_cast<void*>(&(*global_array)[0,0]) : nullptr;
	MPI_Gatherv(&((*new_world)[0,0]), my_rows*N, MPI_CHAR, recvbuf, thecounts, displs,
		    MPI_CHAR, 0, MPI_COMM_WORLD);

#ifdef DEBUG_PROGRAM
	cout << rank << ": After MPI_Gatherv()" << endl;
#endif
	
	if(0 == rank)
	{
 	   print_array(i, *global_array, M, N, 0, 0);
	}
#endif	

	// swap state:
	charArray *tmp = old_world;
	old_world = new_world;
	new_world = tmp;

      }

     //int MPI_Reduce(const void *sendbuf, void *recvbuf, int count,
     //               MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm)

     double t_min_global =0.0, t_max_global =0.0, t_sum_global = 0.0;
     MPI_Reduce(&t_min_local, &t_min_global, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
     MPI_Reduce(&t_max_local, &t_max_global, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
     MPI_Reduce(&t_sum_local, &t_sum_global, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

     if(0 == rank)
     {  
       double t_avg = t_sum_global/(double)(iterations * size);

       cout << "Performance Statistics:" << endl;
       cout << "Minimum time across all processes for any one iteration: " << t_min_global << endl;
       cout << "Maximum time across all processes for any one iteration: " << t_max_global << endl;
       cout << "Average time across all processes per iteration        : " << t_avg        << endl << endl;
     }

     delete old_world;
     delete new_world;

     old_world = nullptr;
     new_world = nullptr;
   }

   if(0 == rank)
   {
      delete global_array;
      global_array = nullptr;

      delete[] displs;
      delete[] thecounts;
      displs = nullptr;
      thecounts = nullptr;
   }

   MPI_Finalize();
}


// life specific:

bool update_element(charArray &old_world, int m, int N, int i, int j) // m not used here.
{
  // if dead, and three neighbors, life next iteration
  // if alive, 2 or 3 neighors, stay alive
  //
  int sum = (old_world[(i-1),j]           == '*') + // N
            (old_world[(i+1),j]           == '*') + // S
            (old_world[i    ,((j-1+N)%N)] == '*') + // W
            (old_world[i    ,((j+1)%N)]   == '*') + // E
            (old_world[(i-1),((j-1+N)%N)] == '*') + // NW
            (old_world[(i-1),((j+1)%N)]   == '*') + // NE
            (old_world[(i+1),((j-1+N)%N)] == '*') + // SW
            (old_world[(i+1),((j+1)%N)]   == '*');  // SE

  bool health = ((old_world[i,j] == '*') && (sum==2 || sum==3))
                || (sum==3);

  return health;
  
}

void do_update(MPI_Comm comm, charArray &old_map, charArray &new_map, int m, int N, int my_rank)
{
  int size;
  MPI_Request requests[4];

  MPI_Comm_size(comm, &size);

  const int TOP    = 1;
  const int BOTTOM = 2;


#ifdef DEBUG_PROGRAM
  cout << my_rank << ": Entering do_update()" <<endl;
#endif
  
  // halo exchange up and down :-)
  // fill in halos of old_map so that the inner computation works.

  // int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag,
  //              MPI_Comm comm, MPI_Request *request)

  // Best practice: post receives before sends to avoid unexpected receives!
  
  // Receive from process below into my bottom halo:
  //WAS:  MPI_Irecv(&old_map[(m+1)*N+0], N, MPI_CHAR, (my_rank+1)%size, TOP, comm, &requests[0]);
  MPI_Irecv(&old_map[m,0], N, MPI_CHAR, (my_rank+1)%size, TOP, comm, &requests[0]);

#ifdef DEBUG_PROGRAM
  cout << my_rank << ": Checkpoint #1" << endl;
#endif
  
  // Receive from process above into my top halo:
  // WAS: MPI_Irecv(&old_map[0*N+0], N, MPI_CHAR, (my_rank+size-1)%size, BOTTOM, comm, &requests[1]);
  MPI_Irecv(&old_map[-1,0], N, MPI_CHAR, (my_rank+size-1)%size, BOTTOM, comm, &requests[1]);

#ifdef DEBUG_PROGRAM
  cout << my_rank << ": Checkpoint #2" << endl;
#endif

  // send my top row to process one above me into its halo row
  //int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest,
  //          int tag, MPI_Comm comm, MPI_Request *request)
  // WAS: MPI_Isend(&old_map[1*N+0], N, MPI_CHAR, (my_rank+size-1)%size, TOP, comm, &requests[2]);
  MPI_Isend(&old_map[0,0], N, MPI_CHAR, (my_rank+size-1)%size, TOP, comm, &requests[2]);

#ifdef DEBUG_PROGRAM
  cout << my_rank << ": Checkpoint #3" << endl;
#endif
  
  // send my bottom row to process one below me into its halo
  //WAS: MPI_Isend(&old_map[(m-1+1)*N+0], N, MPI_CHAR, (my_rank+1)%size, BOTTOM, comm, &requests[3]);
  MPI_Isend(&old_map[(m-1),0], N, MPI_CHAR, (my_rank+1)%size, BOTTOM, comm, &requests[3]);

#ifdef DEBUG_PROGRAM
  cout << my_rank << ": Checkpoint #4" << endl;
#endif
  

  // complete all 4 transfers.
  MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);

#ifdef DEBUG_PROGRAM
  cout << my_rank << ": Checkpoint #5" << endl;
#endif
  

  for(int i = 0; i < m; ++i) // rows
  {
    for(int j = 0; j < N; ++j)
    {
      bool health = update_element(old_map, m, N, i, j);
      new_map[i,j] = health ? '*' : ' ';
    }
  }

#ifdef DEBUG_PROGRAM
  cout << my_rank << ": Exiting do_update()" <<endl;
#endif

}

// linear mapping functions (partition only):

void partitionSize(int P, int N, int p, int &entries)
{
   int L = N/P; // nominal amount of entries per partition
   int R = N%P; // any extras don't fit
   if (p < R)
     entries = (L+1);
   else
     entries = L;
}

void print_array(int iteration, const charArray &array, int M, int N, int Mdelta=0, int Ndelta=0)
{
  cout << "Iteration " << iteration << ":" << ", M = " << M << ", N = " << N << ", size= " << array.size() <<  /* std::hex << */ endl;
  for(int i = 0; i < M; ++i)
  {
    for(int j = 0; j < N; ++j)
    {
#if 1      
      cout << array[i,j];
#else      
      int ix = array[i,j];
      cout << ix << " ";
#endif      
    }
    cout << endl;
  }
  //cout << std::dec;
  
}

    

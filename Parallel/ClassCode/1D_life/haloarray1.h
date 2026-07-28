#pragma once

//#define DEBUG_PRINT(__x) __x
#define DEBUG_PRINT(__x)

#include <vector>

using namespace std;

template <typename T> // Rule of 5 if fully populated; Rule of 0 behavior too 
class haloArray
{
 public:
 haloArray(int _rows, int _cols, int _row_halo_depth, int _col_halo_depth)
   : rows(_rows), cols(_cols),
    row_halo_depth(_row_halo_depth), col_halo_depth(_col_halo_depth),
    row_extent((2*row_halo_depth)+rows), col_extent((2*col_halo_depth)+cols), 
     data(row_extent*col_extent,0)  {DEBUG_PRINT(cout << "haloArray(): entries allocated="<< row_extent*col_extent<<endl);}

 // destructor:
 virtual ~haloArray() {} // Rule of zero behavior, nothing to free.

 // copy constructor (deep copy): [we can add later]
 haloArray(haloArray& other) = delete;

 // copy assignment (copy-and-swap or deep copy): [we can add later]
 haloArray& operator=(const haloArray& other) = delete;

 // move constructor: [we can add later]
 haloArray(haloArray&& other) = delete;

 // move assignment operator: [we can add later]
 haloArray& operator=(const haloArray&& other) = delete;

 // accessors:
 // no bounds checking
 T&operator()(int i, int j)       {return data[((i+row_halo_depth)*col_extent)+(j+col_halo_depth)];}  
 T operator()(int i, int j) const {return data[((i+row_halo_depth)*col_extent)+(j+col_halo_depth)];}

#if 1
 // throws exceptions for out of bounds (requires C++23):
 T&operator[](int i, int j)       {return data.at(((i+row_halo_depth)*col_extent)+(j+col_halo_depth));}  
 T operator[](int i, int j) const {return data.at(((i+row_halo_depth)*col_extent)+(j+col_halo_depth));}
#endif  
  
 int M() {return rows;}
 int N() {return cols;}
 int M_halo() {return row_halo_depth;}
 int N_halo() {return col_halo_depth;}

  int size() const {return data.size();}

 protected:

  int rows, cols;                     // dimensions
  int row_halo_depth, col_halo_depth; // added padding
  int row_extent, col_extent;
  vector<T> data;
  
 private:
};



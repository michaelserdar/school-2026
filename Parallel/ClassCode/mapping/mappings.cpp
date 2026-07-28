#include "mappings.h"
#include <iostream>
using namespace std;

#undef DEBUG_PROGRAM
#ifdef DEBUG_PROGRAM
#define DEBUG_OUTPUT(__x) __x
#else
#define DEBUG_OUTPUT(__x)
#endif

void forwardLinearMapping(int P, int N, int I, int &p, int &i)
{

   int L = N/P; // nominal amount of entries per partition
   int R = N%P; // any extras don't fit

   // for an index I, what partition does it go into?
   // and at what offset?

   // for b!=0, (a/b)*b = a - a mod b
   if(R == 0)
   {
      p = I/L;
      i = I-p*L;
   }
   else // non-divisibility
   {
     if(I < (L+1)*R)
     {
        p = I/(L+1);
        i = I - (L+1)*p;
     }
     else
     {
        p = R + (I -(L+1)*R)/L;
        i = I - R*(L+1) - (p-R)*L;
     }
   }

   DEBUG_OUTPUT(cout << I << "-> p = " << p << " i = " << i << endl;)

}

int partitionSize(int P, int N, int p)
{
   int entries;
  
   int L = N/P; // nominal amount of entries per partition
   int R = N%P; // any extras don't fit
   if (p < R)
     entries = (L+1);
   else
     entries = L;

   DEBUG_OUTPUT(cout << "partitionSize(P = " <<P << ", N = " << N << ", p = " << p << ", L = " << L << "R =  " << R << "entries = " << entries << endl;)
   return entries;
}

void inverseLinearMapping(int P, int N, int p, int i, int &I)
{

   int L = N/P; // nominal amount of entries per partition
   int R = N%P; // any extras don't fit

   // for an index I, what partition does it go into?
   // and at what offset?

   // for b!=0, (a/b)*b = a - a mod b
   if(R == 0)
   {
      I = i + p*L;
   }
   else // non-divisibility
   {
     if(I < (L+1)*R)
     {
        I = i + (L+1)*p;
     }
     else
     {
       // i = I - R*(L+1) - (p-R)*L;
       I = i + R*(L+1) + (p-R)*L;
     }
   }

   DEBUG_OUTPUT(cout << "p -> " << p << ", i = " << i << "--> " << I << endl;)
}

void forwardScatterMapping(int P, int N, int I, int &p, int &i)
{
   p = I % P;
   i = I / P;
   
   DEBUG_OUTPUT(cout << "p -> " << p << ", i = " << i << "--> " << I << endl;)

}

void inverseScatterMapping(int P, int N, int p, int i, int &I)
{
   I = i*P + p;
}




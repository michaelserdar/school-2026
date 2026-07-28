#ifndef _MAPPINGS_H
#define _MAPPINGS_H

void forwardLinearMapping(int P, int N, int I, int &p, int &i);
void inverseLinearMapping(int P, int N, int p, int i, int &I);
void forwardScatterMappint(int P, int N, int I, int &p, int &i);
void inverseScatterMapping(int P, int N, int p, int i, int &I);

int partitionSize(int P, int N, int p);

#endif


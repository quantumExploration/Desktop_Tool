#include <iostream>

using namespace std;
void dftuv(int M, int N, double *U, double *V)
{
   double *u = new double[M];
   double *v = new double[N];

   for(int i=0;i<M;i++)
   {
      if(i > M/2)
      {
         u[i] = i- M;
      }
      else
      {
         u[i] = i;
      }
   }
   for(int i=0;i<N;i++)
   {
      if(i > N/2)
      {
         v[i] = i - N;
      }
      else
      {
         v[i] = i;
      }
   }

   int index = 0;
   for(int i=0;i<M;i++)
   {
        for(int j=0;j<N;j++)
        {
           V[index] = v[j];
           index++;
        }
   }

   for(int i=0;i<N;i++)
   {
        for(int j=0;j<M;j++)
        {
           U[i+j*N] = u[j];
        }
   }


   delete [] u;
   delete [] v;
}



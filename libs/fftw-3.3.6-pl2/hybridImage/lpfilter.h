#include <cmath>
#include "dftuv.h"


void lpfilter(int M, int N, double D0, double *H)
{
   double *U = new double[M*N];
   double *V = new double[M*N];

   dftuv(M,N,U,V);

   double *D = new double[M*N];

   int index= 0;
   for(int i=0;i<M;i++)
   {
      for(int j=0;j<N;j++)
      {
           D[index] = U[index] * U[index] + V[index]*V[index];
           D[index] = sqrt(D[index]);

           H[index] = -(D[index] * D[index])/(2. * D0 * D0);
           H[index] = exp(H[index]);

           index++;
      }
   }

   delete [] D;
   delete [] U;
   delete [] V;
}

void hpfilter(int M, int N, double D0, double *H)
{
   lpfilter(M,N,D0,H);

   int index = 0;
   for(int i=0;i<M;i++)
   {
      for(int j=0;j<N;j++)
      {
         H[index] = 1 - H[index];

         index++;
      }

   }
}

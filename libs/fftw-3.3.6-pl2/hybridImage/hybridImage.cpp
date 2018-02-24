#include "filter.h"
#include <fstream>
#include "fftw3.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argv, char **argc)
{
   char *lowFile = strdup(argc[1]);
   char *highFile = strdup(argc[2]);
   double cutoff = atof(argc[3]);
   char *out = strdup(argc[4]);

    int N, M;
    fftw_plan imagePlan;

    ofstream outfile(out);
    ifstream infile1(lowFile);
    infile1>>N;
    ifstream infile2(highFile);
    infile2>>M;
    outfile<<N<<endl;

    for(int i=0;i<N;i++)
    {
        int col,row;
        infile1>>row>>col;
        infile2>>row>>col;

        fftw_complex *R1,*R2;
        fftw_complex *G1,*G2;
        fftw_complex *B1,*B2;
        R1 = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        G1 = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        B1 = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        R2 = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        G2 = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        B2 = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));

        fftw_complex *outR1, *outR2, *outG1, *outG2, *outB1, *outB2;
        outR1 = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        outG1 = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        outB1 = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        outR2 = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        outG2 = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        outB2 = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));

//================================Read Files=================================//
        int index= 0;
        for(int j=0;j<row;j++)
        {
            for(int t=0;t<col;t++)
            {
                infile1>>R1[index][0]>>G1[index][0]>>B1[index][0];
                infile2>>R2[index][0]>>G2[index][0]>>B2[index][0];
                R1[index][1] = 0;
                G1[index][1] = 0;
                B1[index][1] = 0;
                R2[index][1] = 0;
                G2[index][1] = 0;
                B2[index][1] = 0;
                index++;
            } 
        }

//===============================FFT============================================//
        imagePlan = fftw_plan_dft_2d(row, col,
                                         R1, outR1,
                                         FFTW_FORWARD,FFTW_ESTIMATE);
        fftw_execute(imagePlan);
        imagePlan = fftw_plan_dft_2d(row, col,
                                         B1, outB1,
                                         FFTW_FORWARD,FFTW_ESTIMATE);
        fftw_execute(imagePlan);
        imagePlan = fftw_plan_dft_2d(row, col,
                                         G1, outG1,
                                         FFTW_FORWARD,FFTW_ESTIMATE);
        fftw_execute(imagePlan);
        imagePlan = fftw_plan_dft_2d(row, col,
                                         R2, outR2,
                                         FFTW_FORWARD,FFTW_ESTIMATE);
        fftw_execute(imagePlan);
        imagePlan = fftw_plan_dft_2d(row, col,
                                         B2, outB2,
                                         FFTW_FORWARD,FFTW_ESTIMATE);
        fftw_execute(imagePlan);
        imagePlan = fftw_plan_dft_2d(row, col,
                                         G2, outG2,
                                         FFTW_FORWARD,FFTW_ESTIMATE);
        fftw_execute(imagePlan);


//=====================================High/low pass=================================//
        double *HL = new double[col * row];
        lpfilter(row, col, cutoff, HL);
        double *HH = new double[col * row];
        hpfilter(row, col, cutoff, HH);

        fftw_complex *GR, *GG, *GB;
        GR = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        GG = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        GB = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
 
        index = 0;
        for(int j=0;j<row;j++)
        {
          for(int t=0;t<col;t++)
          {
              GR[index][0] = HL[index] * outR1[index][0]+HH[index] * outR2[index][0]; 
              GR[index][1] = HL[index] * outR1[index][1]+HH[index] * outR2[index][1];
              GG[index][0] = HL[index] * outG1[index][0]+HH[index] * outG2[index][0];
              GG[index][1] = HL[index] * outG1[index][1]+HH[index] * outG2[index][1];
              GB[index][0] = HL[index] * outB1[index][0]+HH[index] * outB2[index][0];
              GB[index][1] = HL[index] * outB1[index][1]+HH[index] * outB2[index][1];
              index++;
          }
        }

//=================================IFFT==========================================//
        fftw_complex *R, *G, *B;
        R = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        G = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));
        B = (fftw_complex*) fftw_malloc(row * col * sizeof(fftw_complex));

        imagePlan = fftw_plan_dft_2d(row, col,
                                         GR, R,
                                         FFTW_BACKWARD,FFTW_ESTIMATE);
        fftw_execute(imagePlan);
        imagePlan = fftw_plan_dft_2d(row, col,
                                         GG, G,
                                         FFTW_BACKWARD,FFTW_ESTIMATE);
        fftw_execute(imagePlan);
        imagePlan = fftw_plan_dft_2d(row, col,
                                         GB, B,
                                         FFTW_BACKWARD,FFTW_ESTIMATE);
        fftw_execute(imagePlan);


        index = 0;
        double sum = row * col;
        outfile<<row<<" "<<col<<endl;
        for(int j=0;j<row;j++)
        {
          for(int t=0;t<col;t++)
          { 
              R[index][0] = R[index][0]/sum;
              G[index][0] = G[index][0]/sum;
              B[index][0] = B[index][0]/sum;
              if(R[index][0]<0) R[index][0] = 0;
              if(G[index][0]<0) G[index][0] = 0;
              if(B[index][0]<0) B[index][0] = 0;
              outfile<<(int)R[index][0]<<" "<<(int)G[index][0]<<" "<<(int)B[index][0]<<endl;
              index++;
          }
        }


        delete [] HL;
        delete [] HH;
        fftw_free(R);
        fftw_free(G);
        fftw_free(B);
        fftw_free(GR);
        fftw_free(GG);
        fftw_free(GB);
        fftw_free(R1);
        fftw_free(G1);
        fftw_free(B1);
        fftw_free(outR1);
        fftw_free(outG1);
        fftw_free(outB1);
        fftw_free(R2);
        fftw_free(G2);
        fftw_free(B2);
        fftw_free(outR2);
        fftw_free(outG2);
        fftw_free(outB2);
    }

    fftw_destroy_plan(imagePlan);
 
    outfile.close();
    infile1.close();
    infile2.close();

    return 0;
}


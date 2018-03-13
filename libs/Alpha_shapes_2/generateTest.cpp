#include <cmath>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main()
{
  srand(1);
  double pi = 3.1415926;
  double r = 1;
  cout<<216<<endl;
  for(int i=0;i<360;i+=5)
  {
    double degree = (double)i /360. * 2.;
    cout<<r * cos(pi * (double)degree)
        <<" "<<r * sin(pi * (double)degree)<<endl;
  }
  for(int i=0;i<360;i+=5)
  {
    double degree = (double)i /360. * 2.;
    cout<<0.8 * r * cos(pi * (double)degree)
        <<" "<<0.8*r * sin(pi * (double)degree)<<endl;
  }
  for(int i=0;i<360;i+=5)
  {
    double degree = (double)i /360. * 2.;
    cout<<0.9 * r * cos(pi * (double)degree)
        <<" "<<0.9*r * sin(pi * (double)degree)<<endl;
  }


  for(int i=0;i<1000;i++)
  {
    int randd = rand()%360;
    int randr = rand()%3+17;
    double degree = (double) randd/360. * 2. ;
    double tempr = r/20. * (double)randr; 
   // cout<<tempr * cos(pi * (double)degree)
    //    <<" "<<tempr * sin(pi * (double)degree)<<endl;
  }
  return 0;
}

#include "iofile.h"

#include <iostream>
#include <fstream>
#include <string.h>
using namespace std;

void readinputqdot1(char *infile,
                    int &seed,
                    svVector3Array *pos,
                    svVector3Array *dir,
                    svScalarArray  *den,
                    svScalarArray  *exp,
                    svScalarArray  *coe,
                    svIntArray *atom,
                    svIntArray *region){
     ifstream in(infile);
     int num;
     in>>num;
     seed = num;
     for(int i=0;i<num;i++){
       int n;
       in>>n;
       for(int j=0;j<n;j++){
         svVector3 p, v;
         svScalar d;
         int a, r;
        
         infile>>p[0]>>p[1]>>p[2]
               >>v[0]>>v[1]>>v[2]
               >>d>>a>>r;
         v.normalize();
         pos[i].add(p);
         dir[i].add(v);
         den[i].add(d);
         atom[i].add(a);
         region[i].add(r);

         svScalar tmpexp = getNumOfIntegerDigits(d);
         svScalar tmpcoe = d/pow(10.,tmpexp);
         exp[i].add(tmpexp);
         coe[i].add(tmpcoe);
        }
     }     
     in.close();
}

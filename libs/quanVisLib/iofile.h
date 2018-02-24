#ifndef _IO_FILE_H
#define _IO_FILE_H

#include "svArray.h"

#define  SVL_FORMAT  100 // default
#define  RBF_INFOR_FORMAT 110
using namespace __svl_lib;

void readinputqdot1(char *infile,
                    int &seed,  
                    svVector3Array *pos,
                    svVector3Array *dir,
                    svScalarArray  *den,
                    svScalarArray  *exp,
                    svScalarArray  *coe,
                    svIntArray *atom,
                    svIntArray *region);


#endif //_IO_FILE_H


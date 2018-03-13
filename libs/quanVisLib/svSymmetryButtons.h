#ifndef __SV_SYMMETRY_BUTTONS_H
#define __SV_SYMMETRY_BUTTONS_H

#include <vector>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"
#include "svUtil.h"
#include "svColors.h"
#include "svIcons.h"

namespace __svl_lib{
  class svSymmetryButtons{
public:
     svSymmetryButtons(svRawSymmetryData *data);//
     ~svSymmetryButtons(){};
     void Init();

     void Reshape(int left, int top);
     void SetColor(svColors *color){color->Get8Colors(buttoncolors);}
     void Render();

     void UpdateState();

     svRawSymmetryData *myData;
     State * myState;
     svVector3 pos[9];
     bool select[9];
     int left;
     svScalar bw;
     svScalar bh;
     int dataSize;
     int splitSize[9];
private:
     svVector4 *buttoncolors;
  };
}


#endif

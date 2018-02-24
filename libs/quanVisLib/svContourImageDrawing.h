
#ifndef __SV_CONTOUR_IMAGE_DRAWING_H
#define __SV_CONTOUR_IMAGE_DRAWING_H

#include "svContourImage.h"

namespace __svl_lib {

class svContourImageDrawing : public svContourImage 
{
 public:
  svContourImageDrawing();
  svContourImageDrawing(svQDOTData *d);
  virtual ~svContourImageDrawing(){}
  
  virtual void Render();
  virtual void Mouse(int mousex, int mousey,
                                  int &layer, int &index, svScalar &den);

  virtual void Move();
  virtual void Move(int mousex, int mousey); 
 private:

 protected:
};

}

#endif // __SV_IMAGE_H

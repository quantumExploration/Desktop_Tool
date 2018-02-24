/*
*contour
*children: in 2D and 3D
 * */
#ifndef __SV_LINE_IMAGE_H
#define __SV_LINE_IMAGE_H

#include "svImage.h"
#include "svContourQDOTData.h"

namespace __svl_lib {

class svLineImage : public svImage
{
 public:
  svLineImage(svRawQDOTData *d, svContourQDOTData *c);
  virtual ~svLineImage();

  virtual void GenerateLines();
  virtual void New(svRawQDOTData *d, svContourQDOTData *c);  
  virtual void RenderLines();
  virtual void Render3D();

 protected:
  void cleanup();
  svContourQDOTData *contourData;
  svVector3ArrayPtr *lines;
  svVector3ArrayPtr *lines3d;
};

}

#endif // __SV_LINE_IMAGE_H

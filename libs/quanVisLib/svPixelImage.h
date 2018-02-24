/*
render pixel 
* */
#ifndef __SV_PIXEL_IMAGE_H
#define __SV_PIXEL_IMAGE_H

#include "svImage.h"

namespace __svl_lib {

class svPixelImage : public svImage
{
 public:
  svPixelImage(svRawQDOTData *d);
  ~svPixelImage();

  virtual void New(svRawQDOTData *d);  

  virtual void LowPass(float cutoff, char *inputfile, char *output);
  virtual void RenderPixel();
  virtual void Render3D();
 private:
  
 protected:
  void cleanup();
  imagePlane *lowImagePlane;
};

}

#endif // __SV_IMAGE_H

/*
 *base function of image
 *for raw qdot data
 *assume the grid distributions on different iamges are the same
 * */
/*
 *one image
 * */
 //todo: double check the contour generation
 //when the plane is not parallel to x-y plane
#ifndef __SV_HYBRID_IMAGE_H
#define __SV_HYBRID_IMAGE_H

#include "svImage.h"

namespace __svl_lib {

class svHybridImage:public svImage
{
 public:
  svHybridImage();
  virtual ~svHybridImage(){}

  void SetContourColors(float r){}
  void GenerateHybridImage(string ifname1, string ifname2,
                           float cutoff, string ofname);//, int sliceIndex);


protected:
  void SaveImage(string ofname);//, int sliceIndex);
  void SaveContours(string ofname);//, int sliceIndex);
  void ReadColors(string ofname);// int sliceIndex);
  //virtual void Render2D();
};

}

#endif // __SV_HYBRID_IMAGE_H

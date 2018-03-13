/*
 *base function of image
 *for raw qdot data
 *assume the grid distributions on different iamges are the same
*The data must be in a "regular" grid

 *For example, the minimun distances between two points in terms of x and y must be the same, such as 0.5
 * */
/*
 *one image
 * */
#ifndef __SV_IMAGE_H
#define __SV_IMAGE_H

#include "svColors.h"
#include "svQDOTData.h"

//TODO: fix svSliceData format in svQDOTData.h
namespace __svl_lib {

class svImage
{
 public:
  svImage();
  virtual ~svImage(){cleanup();}

  virtual void New();

  virtual void GenerateDisplay(int list2D, int list3D);
  virtual void Render3D();
  virtual void Render3DList();//int sliceIndex);
  virtual void Render2D();//int sliceIndex);

  virtual void GenerateHybridImage(string ifname1, string ifname2,
                                          float cutoff, string ofname){}
                                         // int sliceIndex){}
  virtual void ReadColors(string dir, string file);//, int sliceIndex);
  virtual void SaveImage(string dir, string file);//, int sliceIndex);

  void cleanup();

  int row;
  int column;
  svScalar sidex;//side of one image
  svScalar sidey;
  svScalar tranx;
  svScalar trany;
  svScalar scale;
  svVector3 iminD;
  bool isSelectable;
  bool isSelected; //current selected layer
  bool isHighlighted;//selected layer
  bool is3D;
  bool isRotatable;
//  int selectpixel[2];
//  int selectlabel;
  Matrix4f rotateM;
  float pixelSize;
  int sliceIndex;
  svRawSliceData *myData;
  svContourData *myContour;
  int displaylist2D;
  int displaylist3D;
protected:
  virtual void RenderPixel();//int sliceIndex);
  virtual void RenderBar(){}

  //vector<svSliceData> image;//the left bottom location
};

}

#endif // __SV_IMAGE_H

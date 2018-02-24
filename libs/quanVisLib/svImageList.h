//connect to data: svRawSliceData
//connect to interaction: svImageSelection

#include "svHybridImage.h"

#ifndef __SV_IMAGE_LIST_H
#define __SV_IMAGE_LIST_H

//type of images
enum {
  magnitude_image,
  cluster_image
 };

namespace __svl_lib{

class svImageList
{
public:
  svImageList(svRawSliceData *data);
  ~svImageList();

  void New(svRawSliceData *data);
  virtual void Reshape(int segment, int width, int left, int top);
  virtual void Reshape(int left, int top, int width);
  void SetImageType(int type){imageType = type;}
  void SetColors(svColors *color);
  void AddHybridImage(svContourData *myContour, string dir,
                    float cutoff);
  void GenerateGrouping(int binSize){}
  void GenerateGroupingByColor(float percentage);

  void Render2D();
  void Render3D();
  void RenderGrid();

  void SetLocations();

  void cleanup(){}

  int imageType;
  int left;
  int top;
  int width;
  vector<int> selectlayer;
  State *myState;
  svRawSliceData *myData;
  vector<svHybridImage *> imageList;
  //vector<svVector3> imagePos;<-can be replaced by tranx in svImage
  int groupSize;
  list< vector<int> > groupList;
  bool isGrid;
protected:
  bool GenerateGroupingByColor(int image1, int image2, float percentage);
};

}
#endif

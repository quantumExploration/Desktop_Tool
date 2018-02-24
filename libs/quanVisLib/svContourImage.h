#ifndef __SV_CONTOUR_IMAGE_H
#define __SV_CONTOUR_IMAGE_H

#include "svImage.h"
#include "svContourTree.h"

namespace __svl_lib {


class svContourImage : public svImage 
{
 public:
  svContourImage(svQDOTData *d);
  svContourImage();
  virtual ~svContourImage();

  virtual void ResetContourTree();
  virtual void ResetContourTree(int seed);
  virtual void GenerateContourTree(ContourTreeProperty property);  
  virtual void GenerateContours(char *vtkdir, char *planename);
  virtual void GenerateContour(char *vtkdir, char *planename);
  virtual void GenerateSelectedContours(ContourProperty property);
  virtual void GenerateVisibility(svInt bound, svScalar threshold);
  virtual void SetBound(svInt v1, svInt v2, svInt step);
  virtual void SetContourPosition();

  virtual void SaveContourtoFile(char *file);
 private:

 protected:
  void cleanupcontour();
  void cleanupselectcontour();

  virtual void GenerateContour(char *contourfile, char *vtkdir, char *planename, int layer, float contourValue);
  virtual void GenerateContour(char *contourfile, char *vtkdir, char *planename, int layer, float contourValue, int index);
  virtual void SetContours(char *svlfile, int layer, int index);
  virtual void SetContours(char *svlfile, int layer);
  virtual void GeneratePixel();

  svVector3ArrayPtr *contour_pixel;
  svVector3ArrayPtr *contour;
  svIntArray contour_count;
  svIntArray *contour_display;

  svVector3Array *contour_valid;
  svVector3Array *contour_valid_pixel;

  vector<ContourTree *> contour_tree;
};

}

#endif // __SV_CONTOUR_IMAGE_H

#ifndef __SV_CONTOUR_QDOT_DATA_H
#define __SV_CONTOUR_QDOT_DATA_H

#include "svContourTree.h"
#include "svRawQDOTData.h"

namespace __svl_lib {

class svContourQDOTData : public svQDOTData
{
 public:
  svContourQDOTData(svRawQDOTData *raw);
  virtual ~svContourQDOTData();

  virtual void New();
  virtual void Reset();
  virtual void ResetData(int layer); 

  virtual void GenerateContours(ContourProperty property);

  virtual void GenerateContourClusters(svIntArray *cluster,svIntArray *a,
                                        svIntArray *r, svVector3Array *pos){};//TODO
  virtual void SetContourSampling(svInt frequency);
  virtual void SetContourVisible(int contour);
  virtual void SetContourVisible(int layer, int contour);

  virtual void GenerateContourTree(ContourTreeProperty property);  
  virtual void GenerateContourTreeContours(ContourProperty property);
  virtual void GenerateVisibility(svInt bound, svScalar threshold);
  virtual void SetBound(svInt v1, svInt v2, svInt step);

  virtual void SaveContour(ContourProperty property);

  svInt GetContourListSize(){return contourList.size();}
  svVector3ArrayPtr *GetContours(){return contour;}
  svInt GetContourCount(int index){return contour_count[index];}
 protected:
  void clean();
  void clean(int j);

  virtual void SetContourData(char *infName, int layer);
  virtual void SetContourLine(char *svlfile, int layer);
  virtual void SetContourTree(char *svlfile, int layer, int index);
  virtual void SetContourLabel(int cindex, int lindex,
                               int layer, int in);
  virtual void SetContourFactors(){}//TODO

  virtual void GenerateContourLine(char *contourfile, char *vtkdir, char *planename, int layer, float contourValue);
  virtual void GenerateContour(char *contourfile, char *vtkdir, char *planename, int layer, float contourValue);

private:
  svRawQDOTData *raw;

  svVector3ArrayPtr *contour;
  svIntArray contour_count;
  svIntArray *contour_display;
  vector<ContourTree *> contour_tree;

  svVector3Array *contour_valid;//save contour lines
  svScalarArray contourList;
  svIntArray *contourLabel;
  svIntArray *contourLineLabel;
};

}

#endif // __SV_CONTOUR_QDOT_DATA_H

#ifndef __SV_CONTOUR_CLUSTER_DATA_H
#define __SV_CONTOUR_CLUSTER_DATA_H

#include "svContourSliceData.h"

namespace __svl_lib {

class svContourClusterData : public svQDOTData {

public:
  svContourClusterData(svRawQDOTData *raw);
  virtual ~svContourClusterData();

  virtual void New();
  virtual void Reset();
  virtual void ResetData(int layer); 

  virtual void SetInputConnection(svContourSliceData *d){contourSliceData = d;}
  virtual void UpdateClusterList();
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
  svContourSliceData *contourSliceData;

  svIntArray *clusterList;//store index of contour slide data;
  svVector3 *clusterPos;
  svVector3 *clusterDir;
  svVector3 *clusterMag;
};

}

#endif // __SV_CONTOUR_QDOT_DATA_H

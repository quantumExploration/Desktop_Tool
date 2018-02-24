/*
 * State
 */

//#ifndef __SV_STATE_H
//#define __SV_STATE_H

#include <map>
#include <vector>
#include <queue>
#include "svType.h"
#include "svParticle.h"
#include "svArray.h"
#include "svUtil.h"
#include "svQDOT.h"

#ifndef SYMMETRY_TYPE
#define SYMMETRY_TYPE 8
#endif

#ifndef QDOT_DIMENSION
#define QDOT_DIMENSION 7 //px, py, pz, vx, vy, vz, mag
#endif

using namespace std;

namespace __svl_lib {
/*
enum
{
    _XSYSZS = 0,
    _XSYSZA = 1,
    _XSYAZS = 2,
    _XSYAZA = 3,
    _XAYSZS = 4,
    _XAYSZA = 5,
    _XAYAZS = 6,
    _XAYAZA = 7,
};
*/

#define   CONTOUR_RATIO  0
#define  CONTOUR_ABSOLUTE  1
#define  CONTOUR_GLOBAL 2
#define  CONTOUR_LOCAL  3


struct SliceProperty{
  svVector3 pos;
  svVector3 dir;
  svVector3 x;
  svVector3 y;
  svScalar distance;
};

struct ROIProperty{
  svScalar maxMag;
  svScalar minMag;
  vector<bool> splitVisible;
  svScalar maxX;
  svScalar maxY;
  svScalar maxZ;
  svScalar minX;
  svScalar minY;
  svScalar minZ;
  svScalar maxPX;
  svScalar maxPY;
  svScalar maxPZ;
  svScalar minPX;
  svScalar minPY;
  svScalar minPZ;
//  svScalar qdotDistance;
};

struct KmeansProperty {
   vector<bool> isNormalize;
   vector<int> dimension;
   vector<int> numCluster;
   vector<bool> isSplit;//clustering each svQDOTData.splitData[i]
   vector<bool> isUpdate;//TODO
   vector<svScalarArray> weights;
   vector<ROIProperty *> roiP;
   //svIntArray clusterLayer;
   //svScalarArray clusterWeight;
   //svChar *file1;
   //svChar *file2;
   //svInt layer1;
   //svInt layer2;
   //svInt bywhole;
   //svScalar maxMag;
   //svScalar minMag;
};

struct ContourProperty {
   int seed_num;
   svScalarArray *values;
   vector<bool> isUpdate; //detect if we already have the contours;
   svIntArray *type;//ratio or absolute
//   svChar *outputfile;
//   svChar *vtkdir;
//   svChar *wholefile;
//   svChar *planename;
};

struct SymmetryProperty:SliceProperty{
//   svChar *inputfile;
//   svChar *datafile;
//   svChar *outputdir;
//   svVector3 pos; //a point on symmetry plane
//   svVector3 dir; //symmetry plane normal direction
//   svVector3 x; //xaxis on the symmetry plane
//   svVector3 y; // yaxis on the symmetry plane
   svVector3 planepos;//slicepos: it is just used to reduce the searching time. the default is (0,0,0)
   svVector3 planedir;//slicedir. default: (0,0,1);
   svScalar planedistance;//slice direction
   svScalar angle_uncertain;
   svScalar mag_uncertain;
   //svChar *outputfile[SYMMETRY_TYPE];
   //bool regenerate;
};

struct NeighborProperty{
   svChar *inputfile;
   svChar *outputfile;
   svVector3Array svectors;
   vector<svVector3Array> dvectors;
   svVector3 planepos;
   svVector3 planedir;
   svScalar planedistance;
   svScalarArray zvalues;
};

/*
typedef struct ContourTreeProperty{
   svChar *dir;
   svChar *planename;
   int seed_num;
};
*/

struct svQDOTData;

struct State{
//display state
   ROIProperty *rP;
   map<Spin *, bool> qdotVisible;
   map<Spin *, bool> sampleVisible;
//data state
   KmeansProperty *kP;
   SymmetryProperty *symP;
   ContourProperty *cP;
   SliceProperty *sP;

   bool updateSlice;
   bool updateVisible;
   bool updateCluster;
   bool updateContour;
   bool updateSymmetry;

   State();
   ~State();
//contour property=====================================
   void NewContourProperty(int seed_num,
                              vector<double> v,//values
                              vector<int> t,//type:
                              vector<svScalar> sliceDen,//max density
                              vector<bool> gltype,
                              vector<int> *localIndex=NULL);
   void AddContour(vector<double> v,//values
                              vector<int> ratype,//type:ratio or absolute
                              vector<svScalar> sliceDen,//max density
                              vector<bool> gltype, //global or local
                              vector<int> *localIndex=NULL);
   void DeleteContour(double avalues, vector<double> sliceData);
   void UpdateContour(vector<double> v,//values
                              vector<int> ratype,//type:ratio or absolute
                              vector<svScalar> sliceDen,//max density
                              vector<bool> gltype, //global or local
                              vector<int> *localIndex);
    void UpdateContour(int layer, svScalar den){}//TODO
//cluster property=======================================
   void NewClusterProperty(vector<int> num,
                               vector<bool> split,
                               double **weights,
                               vector<ROIProperty *> roiP);
   void AddCluster(int num, bool split, double *weights, ROIProperty *roiP=NULL);
   void DeleteCluster(int index);
   void UpdateCluster(int index, int num, bool split, double *weights, ROIProperty *roiP=NULL);
//Symmetyrproperty=======================================
//So far only support one symmetry plane
   void NewSymmetryProperty(svVector3 pos, svVector3 dir,
                                svVector3 x, svVector3 y,
                                svVector3 planepos, svVector3 planedir,
                                svScalar planedistance,
                                svScalar angle, svScalar mag);
//Slice property=============================================
   void NewSliceProperty(svVector3 pos, svVector3 dir, svScalar distance);
//ROI property=============================================
   void NewROIProperty(svScalar maxMag, svScalar minMag,
                       svScalar maxX, svScalar maxY, svScalar maxZ,
                       svScalar minX, svScalar minY, svScalar minZ,
                       svScalar maxPX, svScalar maxPY, svScalar maxPZ,
                       svScalar minPX, svScalar minPY, svScalar minPZ,
                       vector<bool> splitVisible);
  void NewROIProperty(svScalar maxMag, svScalar minMag,
                       svScalar maxX, svScalar maxY, svScalar maxZ,
                       svScalar minX, svScalar minY, svScalar minZ,
                       vector<bool> splitVisible);

   void NewROIProperty(svScalar maxMag, svScalar minMag,
                       svScalar maxX, svScalar maxY, svScalar maxZ,
                       svScalar minX, svScalar minY, svScalar minZ,
                       svScalar maxPX, svScalar maxPY, svScalar maxPZ,
                       svScalar minPX, svScalar minPY, svScalar minPZ,
                       int splitSize);
   void NewROIProperty(svScalar maxMag, svScalar minMag,
                       svScalar maxX, svScalar maxY, svScalar maxZ,
                       svScalar minX, svScalar minY, svScalar minZ,
                       int splitSize);

   void UpdateSplitVisible(vector<bool> splitVisible);
   void UpdateSplitVisible(vector<int> selectedlayer){}//TODO
   void UpdateClusterVisible(vector<int> selectedlabel){}//TODO
   void UpdateSplitVisible(int lsplit, int hsplit);
   void UpdateMag(svScalar maxMag, svScalar minMag);
   void UpdateAngle(svScalar maxX, svScalar maxY, svScalar maxZ,
                           svScalar minX, svScalar minY, svScalar minZ);
   void UpdatePos(svScalar maxX, svScalar maxY, svScalar maxZ,
                           svScalar minX, svScalar minY, svScalar minZ);
//Update visible label===========================================
   void NewVisible(svQDOT *myQDOT);
   void UpdateVisible(svQDOT *myData);
//Release memory===============================================
   void clean();
};

}
//#endif // __SV_STATE_H

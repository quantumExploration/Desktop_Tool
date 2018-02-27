//TODO:
//1. add/detect contours
//2. reload a new data (Test)
#ifndef __SV_QDOT_DATA_H
#define __SV_QDOT_DATA_H


#define NUM_TOP_MAG 4


#include "svQDOTProcess.h"
#include "svQDOT.h"
#include "svContourTree.h"

using namespace std;

namespace __svl_lib {

struct svRawClusterData;
struct svRawSliceData;
struct svRawSymmetryData;
struct svRawSummaryData;
struct svContourData;
struct svContourClusterData;

struct svMeshData;

typedef struct svSliceData{
  svVector3 ipos;//image pos
  //svVector3 idir; //<--no need to have a copy;
  svScalar iheight;
  svInt ilabel;
  svVector4 icolor;
  svInt torindex;//image -> raw index
  Spin *qdotPoint;
} svSliceData;

//===============Implementation=========================//
typedef struct svQDOTData{ //interface
 svQDOTData(){
   state = new State();
 }
 virtual ~svQDOTData(){cleandata();}
 virtual void UpdateVisibleBySplit();
 virtual void GenerateAverage();
 void cleandata();

 vector< vector<Spin *> > splitData;
// map<Spin *, bool> visibleLabel;
 vector<bool> isSelectable;
 vector<bool> isVisible;
 vector<bool> isSelected;
 vector<bool> isHighlighted;
 int data_structure_type;

 svScalar maxExp;
 svScalar minExp;
 svScalar maxMag;
 svScalar minMag;
 //svScalar minXDistance;
 //svScalar minYDistance;
 //svScalar minZDistance;
 svQDOT *myQDOT;
 State *state;
 map<Spin *, int> *clusterList;
 vector< map<Spin *, vector<Spin *> > > *symmetryList;
 map<Spin *, int> *sliceList;
 vector<svVector3> averageDir;
 vector<svScalar> averageMag;
 vector<svVector3> averagePos;
 list< vector<svScalar> > distributeExp; //percentage
} svQDOTData;

//====================Implement mesh data=============//
typedef struct meshGlyph{
 svVector3 pos;
 svVector3 dir;
 svScalar inDen;
 svScalar outDen;
 svVector3 inDir;
 svVector3 outDir;
 svScalar denDiff;
 svScalar dirDiff;
} meshGlyph;
/*//TODO
typedef struct meshVertice{
 svVector3 pos;
 vector<meshFacet *> facet;
}meshVertice;
*/
typedef struct meshFacet{
 svVector3 pos[3];
 svVector3 norm;
} meshFacet;

typedef struct svMeshData{
 svMeshData(svQDOT *d){myQDOT=d;}
 ~svMeshData(){};
 void SetData(char *dir);
 void SetOBJ(char *dir);
 void GenerateMeshGlyphs(char *dir);
 void ProcessBoundary();
 void ReadGlyphsFile(int region, char *file);
 void SaveGlyphstoFile(int region, char *file);

 svQDOT *myQDOT;
 vector< vector<meshGlyph> > glyphs;
 vector< vector<meshFacet> > facets;
 vector< vector<Spin> > boundPoints;
}svMeshData;

//===========implement svQDOTData================//

typedef struct svRawSymmetryData : svQDOTData{
 svRawSymmetryData(svQDOT *qdot);
 virtual ~svRawSymmetryData(){cleanup();}
//==================API=======================//
 void New(svQDOT *qdot);
 void Init(char *dir);
//==================API end===================//
 void GenerateSymmetry(char *dir);
 void SplitData();
 void cleanup(){}

 svRawSliceData *mySlice;
 svRawClusterData *myCluster;
} svRawSymmetryData;

struct svRawClusterData : svQDOTData{
 svRawClusterData(svQDOT *qdot);
 virtual ~svRawClusterData(){cleanup();}
//=====================API========================//
 void New(svQDOT *qdot);
 void ResetCluster();
 void SetSymmetry(svRawSymmetryData *mySymmetry){this->mySymmetry=mySymmetry;}
 void SetSlice(svRawSliceData *mySlice){this->mySlice = mySlice;}
 void Init(char *folder);//<-new cluster (generate clusters)
 void GenerateSampling(int frequency);//sampling by x,y and z
 void GenerateClusterSampling(int percentage);//sampling each cluster
//===============API end=====================//
 void GenerateBoundary(int splitlevel, char *dir);
// void GenerateHistogram();
 void GenerateSampleLabel(char *folder);
 void GenerateClustersBySymmetry(char *folder);
 void GenerateCluster(vector< vector<Spin*> > tmpdata,
                                       char *dir, char *clusterfname);
 void SetClusterLabel(vector<Spin*> tmpdata, char *file){}//NEED TO BE FIXED
 void SetClusterLabel(vector< vector<Spin*> > tmpdata, int **tmplist);
 void cleanup(){}

 list< vector<int> > boundaryIndex;
 list< vector<int> > connectIndex;//connected component
 vector<int> numOfComponents;//number of connected components in each cluster
 map<Spin *, int> sampleLabel;
 svRawSliceData *mySlice;
 svRawSymmetryData *mySymmetry;
};

struct svRawSummaryData : svQDOTData{
  svRawSummaryData(svQDOT *data);
  virtual ~svRawSummaryData();
  //===================API=========================//
  virtual void GenerateClusterSummary(svRawClusterData *cluster);
  //==================API end======================//
  //list < vector<int> > label;
  void cleanup();
};

struct svRawSliceData : svQDOTData{
 svRawSliceData(svQDOT *data);
 virtual ~svRawSliceData(){cleanup();}
//=====================API========================//
 void New(svQDOT *data);
 void SetSymmetry(svRawSymmetryData *mySymmetry){this->mySymmetry=mySymmetry;}
 void SetCluster(svRawClusterData *myCluster){this->myCluster=myCluster;
                                      this->clusterList=myCluster->clusterList;}
 void Init(svVector3 &center, svVector3 &dir, svScalar &distance);

 void GenerateSampling(int frequency);//sampling by symmetry
 void SaveSlicetoVTK(char *dir, char *file);
 void SaveSliceDensitytoFile(char *dir);
//===============API end=====================//
 void SplitData(svVector3 &center, svVector3 &dir, svScalar &distance);
 void GenerateAxis(svVector3 &center, svVector3 &dir, svScalar &distance);
 void GenerateHistogram();
 void GenerateOrigins();
 void GenerateSlice();
 void GenerateImage();
 void cleanup();

 svRawSymmetryData *mySymmetry;
 svRawClusterData *myCluster;
 vector<svScalar> sliceDen;
 svVector3 sliceCenter;
 svVector3 sliceDir;
 svVector3 sliceMinCenter;
 svScalar sliceDistance;
 svVector3 sliceAxis[3];
 svVector3 ilbbox;//2d image
 svVector3 irbbox;//2d image
 svVector3 iminD;// 2d image
 svInt imageRow;
 svInt imageColumn;
 svScalarArray * histovalues;
 vector<svVector3> sliceOrigin;
 vector< vector<svSliceData> > sliceData;//
 vector< vector<int> > imageData;//transfer to image data
};

//splitdata[0] means the label of cluster is -1
typedef struct svContourClusterData : svQDOTData{
  svContourClusterData(svQDOT *qdot, svContourData *data);
  ~svContourClusterData(){cleanup();}
/*===========API=================================*/
  void New(svQDOT *qdot, svContourData *data);
  void ResetCluster();
  void Init(char *folder);//<--generate clusters
  void GenerateSampling(int frequency){}
//*=========API end=============================*/
  void GenerateClusters(char *folder);
  void GenerateCluster(char *dir, char *clusterfname);//{}
  void SetClusterLabel(char *file){}//NEED TO BE FIXED
  void SetClusterLabel(int **tmplist);
  void cleanup(){}
  svContourData *myData;
} svContourClusterData;

typedef struct svContourSummaryData : svQDOTData{ //no symmetry detection now
  svContourSummaryData(){}
  ~svContourSummaryData(){cleanup();}
  void GenerateClusterSummary(svContourClusterData *cluster);
  void cleanup(){}
  //svContourData *myData;
  //list< vector<int> > label;
} svContourSummaryData;

//TODO
//need an interface to add or delete contours
typedef struct svContourData : svQDOTData{
  svContourData(svQDOT *qdot, svRawSliceData *data);
  virtual ~svContourData(){cleanup();}
//===============API==============================//
  void New(svQDOT *qdot, svRawSliceData *data);
  void GenerateSampling(int frequency);
  void SetCluster(svContourClusterData *cluster){myCluster=cluster;
                          this->clusterList = cluster->clusterList;}
  void Init();
  void GenerateContours(/*ContourProperty &property,*/char *dir);
  void GenerateContoursByContourTree(char *dir,
                                     int step,
                                     bool ratio);
  void GenerateContourTree(char *dir);
  void GenerateContourTreeContours(char *dir);
//=======================API end=================//
  void GenerateContour(char *contourfile, char *vtkdir, char *planename, int layer, float contourValue);
  void GenerateContourLine(char *contourfile, char *vtkdir, char *planename, int layer, float contourValue);
  void SetContours(char *contourfile);
  void SetContourTreeData(char *svlfile, int layer);
  void SetContourLines(char *svlfile, int layer);
  void SplitData();
  void cleanup();

  svRawSliceData *myData;
  vector<bool> isUpdate;
  vector<ContourTree *> contourcompute;
  svContourClusterData *myCluster;
  vector< vector<Spin> > contourTreeData;
  vector< vector<Spin> > contourLines;
  vector<Spin> contourData;
} svContourData;


}
#endif // __SV_QDOT_DATA_H

/*
 * QDOT Process
 */

#ifndef __SV_QDOT_PROCESS_H
#define __SV_QDOT_PROCESS_H

#include <vector>
#include <queue>
#include <list>
#include "svType.h"
#include "svParticle.h"
#include "svArray.h"
#include "ANN.h"
#include "svUtil.h"
#include "svQDOT.h"
#include "svState.h"


using namespace std;

namespace __svl_lib {
typedef enum
{
    _XSYSZS = 0,
    _XSYSZA = 1,
    _XSYAZS = 2,
    _XSYAZA = 3,
    _XAYSZS = 4,
    _XAYSZA = 5,
    _XAYAZS = 6,
    _XAYAZA = 7,
}SYMMETRYTYPE;
/*
typedef struct _KmeansProperty {
   svBool isNormalize;
   svInt dimension;
   svInt numCluster;
   svIntArray clusterLayer;   
   svScalarArray clusterWeight;
   svChar *file1;
   svChar *file2;
   svInt layer1;
   svInt layer2;
   svInt bywhole;
   svScalar maxMag;
   svScalar minMag;
} KmeansProperty;

typedef struct _ContourProperty {
   svInt seed_num;
   svScalarArray *contourValues;
   svIntArray isUpdate; //detect if we already have the contours;   
   svIntArray type;
   svChar *outputfile;
   svChar *vtkdir;
   svChar *wholefile; 
   svChar *planename;
} ContourProperty; 

typedef struct _SymmetryProperty{
   svChar *inputfile;
   svChar *datafile;
   svChar *outputdir;
   svVector3 pos;
   svVector3 dir;
   svVector3 x;
   svVector3 y; 
   svVector3 planepos;
   svVector3 planedir;
   svScalar planedistance;
   svScalar angle_uncertain;
   svScalar mag_uncertain;
   svChar *outputfile[SYMMETRY_TYPE];
   bool regenerate;
} SymmetryProperty;

typedef struct _NeighborProperty{
   svChar *inputfile;
   svChar *outputfile;
   svVector3Array svectors;
   vector<svVector3Array> dvectors;
   svVector3 planepos;
   svVector3 planedir;
   svScalar planedistance;
   svScalarArray zvalues;
} NeighborProperty;

typedef struct _ContourTreeProperty{
   svChar *dir;
   svChar *planename;
   int seed_num;
} ContourTreeProperty;
*/
typedef struct svContour {
    svContour();//svVectorField *inputField);
    void ComputeContours(char *vtkfName, char *contourfName, float contourValue);
    void ComputeContourLines(char *vtkfName, char *contourfName, float contourValue);
    void ProcessContour(char *contourfile, char *planename, char *dir, int layer);
} svContour;

typedef struct svKmeans{
    svKmeans();
    vector<Spin> Normalization(int klevel, double *mean, double *stdev, vector< vector<Spin> > data, KmeansProperty &property, vector<int> &index, vector<int> &slice);
    vector<Spin> Normalization(int klevel, double *mean, double *stdev, vector<Spin> data, KmeansProperty &property, vector<int> &index, vector<int> &slice, int sliceindex);
    void SavetoNormalFile(vector<Spin> data, char *normalizefName);
    void SetClusterLabel(char *clusterfName, int **clusterList,
                                vector<int> index1, vector<int> index2, int level,
                                int &maxCluster);
    void SortClusterLabel(vector< vector<Spin> > data, bool isSplit, int **clusterList, int &maxCluster);
    int ComputeClusters(char *datafName, char *normalizefName, 
                                 char *clusterfName,
                                KmeansProperty &property,
                                int ** clusterList);//<---API
//============not used ========================//
    void Normalization(char *datafName, char *normalizefName, int dim, int dataSize, svScalarArray weight);
    void ComputeClusters2(char *datafName, char *clusterfName, int dim, int dataSize,int numCluster);
} svKmeans;

typedef struct svSymmetry{
       svSymmetry();//svVectorField *inputfield);
       int GetType(svVector3 pos1, svVector3 end1, svScalar mag1,
                         svVector3 pos2, svVector3 end2, svScalar mag2,
                         svScalar angle_uncertain, svScalar mag_uncertain);
        void SymmetryPair(svVector3 pos, svVector3 end,
                    svVector3 &pair, svVector3 &pairend,
                    SYMMETRYTYPE type);
       void SymmetryPair(SymmetryProperty &property, 
                    svVector3 pos, svVector3 dir,
                    svVector3 &pair, svVector3 &pairdir,
                    SYMMETRYTYPE type);
       svVector3 CoordinateMatrix(svVector3 p,
                             svVector3 origin,
                             svVector3 x,
                             svVector3 y,
                             svVector3 z);
       int GetType(svVector3 pos1, svVector3 end1,
                   svVector3 pos2, svVector3 end2);
       void ComputeSymmetry(SymmetryProperty &property);
       void ComputeSymmetry(SymmetryProperty &property,
                                svQDOT *myQDOT, svVector3 planepos,
                                svVector3 planedir,
                                char *dir);
} svSymmetry;
  
typedef struct svNeighbor{
      svNeighbor();//svVectorField *inputfield);
      void ComputeNeighbors(NeighborProperty &property);
}svNeighbor;

typedef struct svNear{
      svNear();
      void ComputeNear(vector<Spin *> pos1, vector<Spin *> pos2, int *label);
      void ComputeComponents(vector<Spin *> data, 
                             int &numOfComponents,
                             vector<int> &connectIndex,
                             svScalar minD);
      void ComputeNear(vector<svVector3> pos1,
                       vector<svVector3> pos2,
                       int *index);
      void ComputeNear(vector<svVector3> pos1,
                       vector<svVector3> pos2,
                       vector<svVector3> norm,
                       int *index);
      void ComputeNear(svVector3Array *pos1, svVector3Array *pos2,
                       int seed, svInt *index);
      void ComputeNear(svVector3Array *pos1, int seed1,
                       svVector3Array pos2, int region,
                       svIntArray &layer, svIntArray &index);
      void ComputeNear(svVector3Array pos1,
                         svVector3Array pos2,
                         svIntArray &index);
      void ComputeNear(svVector3Array pos1,
                         svVector3Array pos2,
                         svIntArray *index);
} svNear;

typedef struct svContourTree{
      svContourTree();//svVectorField *inputfield);
      void ComputeContourTree(char *dir, char *planename, int seed_num);//ContourTreeProperty &property);
} svContourTree;

typedef struct svClusterConnect{
      svClusterConnect();
      void ComputeConnect(svVector3Array *pos, 
                     svIntArray *cluster,
                     svIntArray *newcluster,
                     int maxCluster, int seed,
                     svVectorIntArray *nlayer,
                     svVectorIntArray *nindex);
} svClusterConnect;

typedef struct svMeshGenerator{
  svMeshGenerator(){};
  void GenerateMesh(char *ifname, char *ofname, float sbottom, float pbottom); 
  void GenerateBoundaryPoints(char *inputfile, char *outputfile);
  void GenerateBoundaryPoints2D(char *inputfile, char *outputfile);
  void GenerateSurface(char *inputfile, char *vtkfile, char *pointfile, char *surfacefile);
} svMeshGenerator;
}
#endif // __SV_QDOT_PROCESS_H

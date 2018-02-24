//**************************************
//**************************************
//*Read and write the input script******
//*Update the input script from GUI*****
//*Data, contour, cluster(weights)******
//*Compare current and previous status**
//*If the contents in the input script**
//and GLUI are different, use the script
//**************************************
//**************************************

#include "svVectorField.h"
#include <vector>

#define LABEL_NUM 23

#define DATADIR 1
#define DATANAME 2
#define TMP 3
#define DATATYPE 4
#define PC 5
#define PD 6
#define IC 7
#define CONTOUR 8
#define CLUSTER 9
#define STEP1 10
#define CWEIGHTS 11
#define CLAYER 12
#define CWHOLE 13
#define CMAG 14
#define CNUM 15
#define STEP2 16
#define SYMMETRY 17
#define SPX 18
#define SPY 19
#define SANGLE 20
#define SMAG 21
#define ROI 22
#define CUTTINGPLANE 23

using namespace __svl_lib;

typedef struct ConfigProperty{
        char *rawDir;
        char *rawFile;
        char *storeDir;
        char *planename;

        svVector3 plane_center;
        svVector3 plane_vector;
        svScalar plane_distance;

        svBool isContour;

        KmeansProperty step1_kmeansproperty;
        KmeansProperty step2_kmeansproperty;
        SymmetryProperty symmetryproperty;
        ContourProperty contourproperty;
        ContourTreeProperty contourtreeproperty;
//        NeighborProperty neighborproperty;

        vector<string> contourname;
        int  zmin;
        int  zmax;
        int  nonz;
        int layer_repeat;
        svScalarArray *magrange;
        int format;
} ConfigProperty;
/*
string variables[] = 
{
 "datadir",
 "dataname",
 "foldertostorefiles",
 "type",
 "cuttingplane",
 "planecenter",
 "planedirection",
 "ifcontour",
 "contours",
 "clusters",
 "step1",
 "weights",
 "layer",
 "bywhole",
 "magrange",
 "numofclusters",
 "step2",
 "symmetry",
 "planex",
 "planey",
 "angleuncertainty",
 "maguncertainty",
 "roi"
};
*/
class Paser
{
public:
   Paser();
   ~Paser();

  void ReadFiles(char *file);
  
  void SetPlane(svScalar distance, svVector3 c);
  void GetContent();

  void SetSeedNum(int seed);//{seed_num = seed;}
  
  void SetContours(int layer, double value);
  void SetContours(double value);
  void SetContourValues(svScalarArray density);

  void ProcessContour();
  void ProcessClusterLayers();

  void SetData(int data);
  void SetLayer(int zmin, int zmax, int nonz, int repeat);

  void SavetoFiles(char *file);
  void SavetoFile(char *file);

  char *GetRawDir(){return property.rawDir;}
  char *GetRawFile(){return property.rawFile;}
  char *GetStoreDir(){return property.storeDir;}
  char *GetPlaneName(){return property.planename;}
  char *GetContourTreeDir(){return property.contourtreeproperty.dir;}
  svVector3 GetPlaneCenter(){return property.plane_center;}
  svVector3 GetPlaneVector(){return property.plane_vector;}
  svScalar GetPlaneDistance(){return property.plane_distance;}
  svVector3 GetSymmetryPos(){return property.symmetryproperty.pos;}
  svVector3 GetSymmetryDir(){return property.symmetryproperty.dir;}
  svVector3 GetSymmetryX(){return property.symmetryproperty.x;}
  svVector3 GetSymmetryY(){return property.symmetryproperty.y;}
  bool GetIsContour(){return property.isContour;}
  int GetFormat(){return property.format;}
  KmeansProperty GetStep1Property(){return property.step1_kmeansproperty;}
  KmeansProperty GetStep2Property(){return property.step2_kmeansproperty;}
  ContourProperty GetContourProperty(){return property.contourproperty;}
  ContourTreeProperty GetContourTreeProperty(){return property.contourtreeproperty;}
  SymmetryProperty GetSymmetryProperty(){return property.symmetryproperty;}
  svScalar GetMagRange00(){return property.magrange[0][0];}
  svScalar GetMagRange01(){return property.magrange[0][1];}
  svScalar GetMagRange10(){return property.magrange[1][0];}
  svScalar GetMagRange11(){return property.magrange[1][1];}
  int GetZmin(){return property.zmin;}
  int GetZmax(){return property.zmax;}
  int GetNonz(){return property.nonz;}
  int GetRepeated(){return property.layer_repeat;}

  bool GetUpdateContour(){return update_contour;}
  bool GetUpdateData(){return update_data;}
  bool GetUpdateCluster(){return update_cluster;}
  bool GetUpdateRender(){return update_render;}
  bool GetUpdateSymmetry(){return update_symmetry;} 

  void AddContour(int layer, svScalar value){}//TODO update 
  void DeleteContour(int layer, svScalar value){}//TODO
 
  void ResetBool()
  {
	 update_cluster = false;
	 update_contour = false;
	 update_render = false;
	 update_symmetry = false;
	 update_data = false;
  }  
  void cleanup();
  
protected:
  void AddContourName(int layer, double value);
  void AddContourName(double value);
  string GetContourString(string data);
  string LowerCase(string data);
  vector<double> StringtoNumbers(string str);
  void InitFiles();
  void SetFiles();
  void SetVariables();
  void GetContours(string str, bool &isGlobal, int &type,
                 double &value, int &layer1, int &layer2);
  void GetVariable(int &level1, int level2, int index);
  void GetType(int &level1, int &level2, int index);

  int GetLineLoc(int label, int &lineHead);

  void cleanfiles();
  
private:
  vector<string> line;  
  vector<bool> isData;

  ConfigProperty property;
  
  vector<string> precontourname;
  vector<string> speciallines;
  int layer1[2];
  int whole1;
  int layer2[2];
  int whole2;  
 
  int seed_num;

  string clusterline1;
  string clusterline2;
  vector<string> prespeciallines;
  vector<string> configtext; 
  vector<int> textindex;
 
  bool update_contour;
  bool update_data;
  bool update_cluster;
  bool update_symmetry;
  bool update_render;
};

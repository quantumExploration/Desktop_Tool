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
//        NeighborProperty neighborproperty;

        vector<string> contourname;
        ContourProperty contourproperty;
        ContourTreeProperty contourtreeproperty;

        svScalarArray *magrange;
        int format;
}

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

class Paser
{
public:
  string LowerCase(string data);
  void ReadFiles(char *file);
  void SavetoFiles(char *ifile, char *ofile);
  void GetContent();

  string GetDataDir();
  string GetDataName();
  string GetTmpName();

  int GetDataType();
  
  void GetCPlane(double *c, double *d);
  
  bool GetIsContour(); 
  void GetContours(int *type, double *value, int num);
   
  void GetStep1(double *w, int *layer, bool &whole,
                double *mag, int &num);
  void GetStep2(double *w, int *layer, bool &whole,
                double *mag, int &num);
  
  void GetSymmetry(double *c, double *x, double *y, double *z,
                   double &angle. double &mag);
  void GetROI(int &zmin, int &zmax, int &repeat);
protected:
  void GetType(int &level1, int &level2, int index);
private:
  vector<string> line;  
  vector<bool> isData;

  ConfigProperty property;

  string datadir;
  string dataname;
  string tmpfolder;
  int datatype;
  double cplanecenter[3];
  double cplanedirection[3];

  bool ifcontour;
  int contourline;
  vector<double> global_value;
  vector<int> global_type;
  vector<double> local_value;
  vector<int> local_type;
  vector<double> local_layer[2];

  double step1_w[7];
  int step1_layer[2];  
  bool step1_whole;
  double step1_mag[2];
  int step1_num;
  double step2_w[7];
  int step2_layer[2];
  bool step2_whole;
  double step2_mag[2];
  int step2_num;

  double splanecenter[3];
  double splanedirection[3];
  double splanex[3];
  double splaney[3];
  double sangle;
  double smag;
 
  vector<int> roi;
};


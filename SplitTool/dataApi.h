#include "svRawQDOTData.h"
#include "svContourQDOTData.h"

using namespace std;
using namespace __svl_lib;

class dataApi{
public:
  dataApi();
  ~dataApi();

//  void init(char *configFile);

  void testvis(int layer, int index, svVector3 l, svVector3 r);

  void resetData(){raw->Reset();contour->Reset();}
  void init(char *dir, char *file, char *tmpdir,
                        svVector3 pcenter, svVector3 pvector,
                        svScalar pdistance, int format);
  void setContourData(ContourProperty cproperty);//
                            // SymmetryProperty sproperty);
  void setContourData(svChar *dir, svInt layer, svScalar value);
  void setSingleContour(svChar *dir, svChar *planename,
                               ContourProperty property,
                               svInt contour_layer, svScalar contour_mag);
  void setRawData(char *dir, char *file, SymmetryProperty sproperty);
  void setContourTree(ContourTreeProperty property,
                      ContourProperty cproperty);
/*svInt vbound0, svInt vbound1, svInt vstep,
                         svInt vbound, ContourTreeProperty property,
                         ContourProperty cproperty,
                         svScalar threshold);
*/
  void resetVisible();
  void setVisible(vector<int> symmetrytype,
                         svIntArray visiblelayer,
                         int frequency, svScalar min_mag, svScalar max_mag,
                         svVector3 l, svVector3 r,
                         svScalar min_xangle,svScalar max_xangle,
                         svScalar min_yangle, svScalar max_yangle,
                         svScalar min_zangle, svScalar max_zangle,
                         int samplesize);
  void resetContourVisible();
  void setContourVisible(
                         svIntArray visiblelayer,
                         int frequency, svScalar min_mag, svScalar max_mag,
                         svVector3 l, svVector3 r,
                         svScalar min_xangle,svScalar max_xangle,
                         svScalar min_yangle, svScalar max_yangle,
                         svScalar min_zangle, svScalar max_zangle,
                         int samplesize);
  void setCluster(svScalar min_mag1, svScalar min_mag2,
                         svScalar max_mag1, svScalar max_mag2,
                         KmeansProperty kproperty1, KmeansProperty kproperty2,
                         SymmetryProperty sproperty, vector<int> symmetrytype);
  void setContourCluster(svScalar min_mag1, svScalar min_mag2,
                         svScalar max_mag1, svScalar max_mag2,
                         KmeansProperty kproperty1, KmeansProperty kproperty2);
  void setContourVariables(svIntArray *cluster, svIntArray *a,
                                        svIntArray *r, svVector3Array *pos);

  svRawQDOTData *getRaw(){return raw;}
  svContourQDOTData *getContour(){return contour;}
  svIntArray *getCluster(){return raw->GetClusterLabels();}
  svIntArray *getAtom(){return raw->GetAtom();}
  svIntArray *getRegion(){return raw->GetRegion();}
  svVector3Array *getPos(){return raw->GetGlyphs();} 
  svVector3 getPlaneC(){return raw->GetMinPlane();}
  svVector3 getPlaneV(){return raw->GetPlaneDir();}
  svScalar getPlaneD(){return raw->GetPlaneDistance();}
  svInt getSeed(){return raw->GetPlaneNum();}
  svScalarArray getContourValues(char *str){return raw->GetMaxDensity(str);}  
  void getBoundary(svVector3 &lb, svVector3 &rb){raw->GetPhysicalDimension(&lb,&rb);}  
 
  int getScaling(){return raw->GetScaling();}
  int getNumPower(){return raw->GetNumPower();}
private:
  svRawQDOTData *raw;
  svContourQDOTData *contour;
  //svScalarArray *symmetrycount;
};

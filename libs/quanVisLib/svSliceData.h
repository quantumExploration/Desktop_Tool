
#ifndef __SV_QDOT_DATA_H
#define __SV_QDOT_DATA_H

#include "svUtil.h"
#include "svArray.h"
#include "svList.h"
#include "svException.h"
#include "svUtil.h"
#include "svCCL.h"
#include "svQDOT.h"

using namespace std;

namespace __svl_lib {

class svQDOTData : public svQDOT {
 public:
  svQDOTData();//svChar *dir, svChar *inf1, svChar *inff2, 
            //svVectorField* f, int numPlane);
  virtual ~svQDOTData();

  virtual void New();//svVectorField* f, int numPlane);//svChar *indir, svChar *inf1, svChar *inf2,
  virtual void Reset();
  virtual void ResetData(int layer);
  virtual void ResetCluster();

  virtual void SetData(char *infName){}
  virtual void SaveToFile(char *fname); // save for later rendering

//cluster generation
  virtual void GenerateClusters(svChar *inf);
  virtual void GenerateClusters(svIntArray *cluster);
  virtual void GenerateClusters(KmeansProperty & property);

//sampling
  virtual void SetSampleData(int size);

//visible
  virtual void ResetVisible();
  virtual void SetAngleVisible(svScalar min_xangle,svScalar max_xangle,
                         svScalar min_yangle, svScalar max_yangle,
                         svScalar min_zangle, svScalar max_zangle);
  virtual void SetVisible(svIntArray layer);
  virtual void SetLayerVisible(svIntArray &layer);
  virtual void SetClusterVisible(int layer, int data);
  virtual void SetVisible(svScalar z1, svScalar z2);
  virtual void SetVisible(svVector3 l, svVector3 r);
  virtual void SetVisible(svScalar z1, svScalar z2, int frequency);

//roi
  virtual void SetROI();
  virtual void SetROI(svScalar mag1, svScalar mag2);

//get 
  svVector3 GetCenter(int index);
  svScalarArray *GetMagProb(svScalar &level);
  void GetBoundary(int index, svVector3 &l, svVector3 &r);
  svVector3Array *GetGlyphs() const {return glyph;}
  svVector3 GetGlyph(int seed, int index){return glyph[seed][index];}
  svIntArray *GetAtom() const {return atom;}
  svInt GetAtom(int seed, int index){return atom[seed][index];}
  svIntArray *GetRegion() const {return region;}
  svInt GetRegion(int seed, int index){return region[seed][index];}
  svVector3Array *GetDir() const {return dir;}
  svVector3 GetDir(int seed, int index){return dir[seed][index];}
  svScalarArray *GetMag() const {return mag;}
  svScalar GetMag(int seed, int index){return mag[seed][index];}
  svIntArray* GetClusterLabels(){return clusterLabel;}
  svInt GetClusterLabel(int i, int j){return clusterLabel[i][j];}
  svInt GetSeed(){return seed_num;}
  svInt GetSeed(int seed){return glyph[seed].size();}
  svIntArray *GetVisibleLabel(){return visibleLabel;}
  svInt GetVisibleLabel(int seed, int index){return visibleLabel[seed][index];}
  svInt GetSampleLabelSize(){return sampleLabel.size();}
  svInt GetSampleLabel(int i){return sampleLabel[i];}
  svScalar GetCoe(int seed, int index){return coe[seed][index];}
  svScalar GetExp(int seed, int index){return exp[seed][index];}
  svIntArray *getVisibleLabel(){return visibleLabel;}
  svInt GetMaxClusterLabel(){return maxClusterLabel;}
protected:
  virtual void clean(int j);
  virtual void clean();

  virtual void SaveToFile(char *outputfile, svIntArray clusterLayer, 
                     int dimension, int clusterIndex);
  virtual void GenerateCluster(char *clusterfile, char *immefile, bool isNormalize,
                                 int dimension, svScalarArray clusterWeight,
								  int numCluster);
  virtual void SetClusterLabel(char *clusterfile, svIntArray clusterLayer, 
                         int clusterIndex);
  virtual void ProcessClusterLabel();//order the cluster label from -1 to max

  svInt dataSize;
  vector<int> visibleCluster;
  svVector3Array *glyph; // positions
  svVector3Array *dir;   // direction
  svScalarArray  *mag;   // magnitude
  svScalarArray *exp;
  svScalarArray *coe;
  svVector4Array *color;
  svIntArray *atom;
  svIntArray *region;
 
  svInt glyphSize;

  int maxClusterLabel;
  svIntArray clusterLabelbymag;

  svIntArray    *clusterLabel;   // store cluster result
  svIntArray    *roiLabel; //inside roi; boolean
  svIntArray    *visibleLabel;
  svIntArray    sampleLabel;

};
}
#endif // __SV_QDOT_DATA_H

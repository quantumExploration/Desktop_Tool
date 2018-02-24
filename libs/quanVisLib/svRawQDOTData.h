
#ifndef __SV_RAW_QDOT_DATA_H
#define __SV_RAW_QDOT_DATA_H

#include "svQDOTData.h"

using namespace std;

namespace __svl_lib {

class svRawQDOTData: public svQDOTData {
 public:
  svRawQDOTData();//svChar *dir, svChar *inf1, svChar *inff2, 
            //svVectorField* f, int numPlane);
  virtual ~svRawQDOTData();

  virtual void Reset();
  virtual void ResetData(int layer);
  virtual void New();//svVectorField* f, int numPlane);//svChar *indir, svChar *inf1, svChar *inf2,

  virtual void SetData(char *infName);
  virtual void Set2DData(char *infName);
  virtual void SetFormat(char *format_fname);

//cluster generation
  virtual void GenerateClustersBySymmetry(KmeansProperty  property, 
                      SymmetryProperty  sproperty,
                      vector<int> symmetrytype);
//symmetry generation
  virtual void GenerateSymmetry(SymmetryProperty property);
//neighbors
  virtual void SetNeighborLabel(vector<int> symmetrytype,SymmetryProperty &property, svChar *outputdir);
  virtual void GenerateNeighbors(NeighborProperty &property);
  virtual void SetNeighbors(char *file);
//sampling
  virtual void SetSampling(svInt frequency);

//neighbours (surface)
  virtual void SetInside(char *directory, char *name); 

//visible
  virtual void SetSymmetryVisible(vector<int> symmetrytype);
  virtual void SetRegionVisible();

//savefiles
  virtual void SaveSymmetry(int symmetrytype,
                           char *str1, char *str2,
                           SymmetryProperty property);
//get 
  svScalarArray *GetSymmetryMagProb(int & topvalue);
  void GetBoundary(int index, svVector3 &l, svVector3 &r);
  svVector3 GetPixel(int seed, int index){return pixel[seed][index];}
  svInt GetRegionSize(int seed){return inglyph[seed].size();}
  svInt GetSType(int seed, int index){return stype[seed][index];}
  svVector3 GetPLb(){return plb;}
  svVector3 GetPRb(){return prb;}
  svVector3Array GetInGlyph(int r){return inglyph[r];}
  svVector3 GetInGlyph(int r, int index){return inglyph[r][index];}
  svScalar GetMagDiff(int r, int index){return magdiff[r][index];}
  svScalar GetDirDiff(int r, int index){return dirdiff[r][index];}
 
//  svScalarArray GetEntropyValues(){return entropy;}
  int *GetSymmetryCount(){return symmetrycount;}
  int GetSymmetryTopCount(){return symmetrytopcount;}
  int GetSymmetryCount(int index){return symmetrycount[index];} 
protected:
  virtual void clean(int j);
  virtual void clean();

  virtual void SetClusterLabelBySymmetry(char *clusterfile, svIntArray clusterLayer,
                         int clusterIndex, vector<int> symmetrytype);
private:
  svIntArray *stype;
  svVector3Array *pixel;

  svVector3Array *inglyph;
  svScalarArray *magdiff;
  svScalarArray *dirdiff;
  svIntArray *inlayer;
  svIntArray *inindex;
  svIntArray *outlayer;
  svIntArray *outindex;

  svIntArray *ccluster;//connectedcluster
  svVectorIntArray *nlayer;//neighbor layer
  svVectorIntArray *nindex;//neighbor index
 
  svIntArray *glyphFormat[3]; 

  svVector3 plb;
  svVector3 prb;


  svList *symmetrylist[SYMMETRY_TYPE];
  int symmetrycount[SYMMETRY_TYPE+1];
  int symmetrytopcount;
};
}
#endif // __SV_RAW_QDOT_DATA_H

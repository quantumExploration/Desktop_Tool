#ifndef __SV_CONTOUR_TREE_H
#define __SV_CONTOUR_TREE_H

#include "svUtil.h"
#include "svArray.h"
//#include "svParticle.h"
//#include "svPrimitive.h"

#include <vector>

namespace __svl_lib {

typedef struct ContourTreeArc{
  svScalar topvalue;
  svScalar bottomvalue;
  svInt topid;
  svInt bottomid;
  vector<int> topArc;
  vector<int> bottomArc;
  svInt volume;
  svInt depth;
  bool active;
} ContourTreeArc;

typedef struct ContourTreeNode{
  vector<int> topArc;
  vector<int> bottomArc;
} ContourTreeNode;

class ContourTree 
{
 public:
  ContourTree(){isoCount=1;isoArc=new svIntArray[1];
                volumebound[0]=10; volumebound[1]=100;
                volumestep=5;}
  ~ContourTree();

  void Construct(char *file);
  void SetVolumeBound(svInt v1, svInt v2, svInt step);
  void Prune();
  void PruneVolume();
  void GenerateIsoValues();

  vector<svScalar> GetIsoValues(){return isoValue;}

  void GetDisplayIndex(svInt bound, svScalar threshold, svIntArray &index);
  int GetNodeSize(){return isoValue.size();}//contour_tree.size();}
  svScalar GetIsoValue(int index);//{return contour_tree[index].bottomvalue;}
 protected:
  void MergeBranches();

 private:
  vector<ContourTreeNode> contour_node;
  vector<ContourTreeArc> contour_tree;
  svInt volumebound[2];
  svInt volumestep;
  svIntArray *isoArc;
  svInt isoCount;
  vector<svScalar> isoValue;
};

}

#endif // __SV_CONTOUR_TREE_H

/*
 * State
 */

#include "svState.h"

using namespace std;

namespace __svl_lib {

State::State(){
  rP = new ROIProperty();
  kP = new KmeansProperty();
  sP = new SliceProperty();
  cP = new ContourProperty();
  symP = new SymmetryProperty();

  updateSlice = true;
  updateVisible = true;
  updateCluster = true;
  updateContour = true;
  updateSymmetry = true;
}

State::~State(){
  clean();
}

void State::clean(){
  rP->splitVisible.clear();
  kP->isNormalize.clear();
  kP->dimension.clear();
  kP->numCluster.clear();
  kP->isSplit.clear();
  for(int i=0;i<kP->weights.size();i++) kP->weights[i].free();
  kP->weights.clear();
  for(int i=0;i<kP->roiP.size();i++){
     kP->roiP[i]->splitVisible.clear();
  }
  kP->roiP.clear();
  for(int i=0;i<cP->seed_num;i++){
    cP->values[i].free();
    cP->type[i].free();
  }
  delete [] cP->values;
  cP->isUpdate.clear();
  delete [] cP->type;
  qdotVisible.clear();
  sampleVisible.clear();
}
void State::NewContourProperty(int seed_num,
                              vector<double> v,//values
                              vector<int> ratype,//type:ratio or absolute
                              vector<svScalar> sliceDen,//max density
                              vector<bool> gltype, //global or local
                              vector<int> *localIndex)
{
  if(cP->seed_num>0){
    for(int i=0;i<seed_num;i++){
      cP->values[i].free();
      cP->type[i].free();
    }
    delete [] cP->values;
    cP->isUpdate.clear();
    delete [] cP->type;
  }
  cP->seed_num = seed_num;
  cP->values = new svScalarArray[seed_num];
  cP->isUpdate.resize(seed_num);
  cP->type = new svIntArray[seed_num];
  int localCount = 0;
  for(int i=0;i<v.size();i++){
      double value = v[i];
      if(gltype[i] == CONTOUR_GLOBAL){
        for(int j=0;j<seed_num;i++){
           cP->values[j].add(value);
           cP->type[j].add(ratype[i]);
        }
      }
      else if(gltype[i] == CONTOUR_LOCAL){
        for(int j=localIndex[0][localCount];j<=localIndex[1][localCount];j++){
           cP->values[j].add(value);
           cP->type[j].add(ratype[i]);
             localCount++;
        }
      }
  }
  std::fill(cP->isUpdate.begin(), cP->isUpdate.end(), true);
}
void State::AddContour(vector<double> v,//values
                              vector<int> ratype,//type:ratio or absolute
                              vector<svScalar> sliceDen,//max density
                              vector<bool> gltype, //global or local
                              vector<int> *localIndex)
{
  int localCount = 0;
  for(int i=0;i<v.size();i++){
      double value = v[i];
      if(gltype[i] == CONTOUR_GLOBAL){
        for(int j=0;j<cP->seed_num;i++){
           cP->values[j].add(value);
           cP->type[j].add(ratype[i]);
           cP->isUpdate[j]=false;
        }
      }
      else if(gltype[i] == CONTOUR_LOCAL){
        for(int j=localIndex[0][localCount];j<=localIndex[1][localCount];j++){
           cP->values[j].add(value);
           cP->type[j].add(ratype[i]);
           cP->isUpdate[j]=false;
             localCount++;
        }
      }
  }
}
//delete contours with the same absolute values
void State::DeleteContour(double avalues, vector<double> sliceData){
  for(int i=0;i<cP->seed_num;i++){
    for(int j=0;j<cP->values[i].size();j++){
        double value = cP->values[i][j];// * sliceData[i];
        if(cP->type[i][j] == CONTOUR_RATIO){
           value *= sliceData[i];
        }
        if(fabs(value-avalues)<1e-8){//isEqual(value, avalues)){
           cP->values[i].remove(j);
           cP->isUpdate[i] = true;
           cP->type[i].remove(j);
           j--;
        }
    }
  }
}
void State::UpdateContour(vector<double> v,//values
                              vector<int> ratype,//type:ratio or absolute
                              vector<svScalar> sliceDen,//max density
                              vector<bool> gltype, //global or local
                              vector<int> *localIndex)
{
  int localCount = 0;
  for(int i=0;i<v.size();i++){
      double value = v[i];
      if(gltype[i] == CONTOUR_GLOBAL){
        for(int j=0;j<cP->seed_num;i++){
           if(cP->values[j].size()>0){
             cP->values[j].free();
             cP->type[j].free();
           }
        }
      }
      else if(gltype[i] == CONTOUR_LOCAL){
        for(int j=localIndex[0][localCount];j<=localIndex[1][localCount];j++){
           if(cP->values[j].size()>0){
             cP->values[j].free();
             cP->type[j].free();
             localCount++;
           }
        }
      }
  }
  localCount = 0;
  for(int i=0;i<v.size();i++){
      double value = v[i];
      if(gltype[i] == CONTOUR_GLOBAL){
        for(int j=0;j<cP->seed_num;i++){
           cP->values[j].add(value);
           cP->type[j].add(ratype[i]);
           cP->isUpdate[j]=false;
        }
      }
      else if(gltype[i] == CONTOUR_LOCAL){
        for(int j=localIndex[0][localCount];j<=localIndex[1][localCount];j++){
           cP->values[j].add(value);
           cP->type[j].add(ratype[i]);
           cP->isUpdate[j]=false;
           localCount++;
        }
      }
  }
}
void State::NewClusterProperty(vector<int> num,
                               vector<bool> split,
                               double **weights,
                               vector<ROIProperty *> roiP){
  kP->isNormalize.clear();
  kP->dimension.clear();
  kP->numCluster.clear();
  kP->isSplit.clear();
  for(int i=0;i<kP->weights.size();i++) kP->weights[i].free();
  kP->weights.clear();
  for(int i=0;i<kP->roiP.size();i++) kP->roiP[i]->splitVisible.clear();
  kP->roiP.clear();

  kP->roiP.resize(num.size());
  for(int i=0;i<num.size();i++){
     kP->roiP[i] = new ROIProperty();
     kP->isNormalize.push_back(true);
     kP->dimension.push_back(QDOT_DIMENSION);
     kP->isSplit.push_back(split[i]);
     kP->numCluster.push_back(num[i]);
     svScalarArray w;
     for(int j=0;j<QDOT_DIMENSION;j++){
         w.add(weights[i][j]);
     }
     kP->weights.push_back(w);
     w.free();
     if(roiP[i] == NULL){ kP->roiP[i] = NULL;continue;}
     for(int j=0;j<roiP[i]->splitVisible.size();j++)
         kP->roiP[i]->splitVisible.push_back(roiP[i]->splitVisible[j]);
     kP->roiP[i]->maxMag = roiP[i]->maxMag;
     kP->roiP[i]->minMag = roiP[i]->minMag;
     kP->roiP[i]->maxX = roiP[i]->maxX;
     kP->roiP[i]->maxY = roiP[i]->maxY;
     kP->roiP[i]->maxZ = roiP[i]->maxZ;
     kP->roiP[i]->minX = roiP[i]->minX;
     kP->roiP[i]->minY = roiP[i]->minY;
     kP->roiP[i]->minZ = roiP[i]->minZ;
  }
}
void State::AddCluster(int num, bool split, double *weights, ROIProperty *roiP)
{
  kP->isNormalize.push_back(true);
  kP->dimension.push_back(QDOT_DIMENSION);
  kP->isSplit.push_back(split);
  kP->numCluster.push_back(num);
  svScalarArray w;
  for(int i=0;i<QDOT_DIMENSION;i++) w.add(weights[i]);
  kP->weights.push_back(w);
  w.free();
  if(roiP ==NULL){kP->roiP.push_back(NULL);return;}
  kP->roiP.push_back(new ROIProperty());
  int size = kP->roiP.size()-1;
  for(int i=0;i<roiP->splitVisible.size();i++){
    kP->roiP[size]->splitVisible.push_back(roiP->splitVisible[i]);
  }
  kP->roiP[size]->maxMag = roiP->maxMag;
  kP->roiP[size]->minMag = roiP->minMag;
  kP->roiP[size]->maxX = roiP->maxX;
  kP->roiP[size]->maxY = roiP->maxY;
  kP->roiP[size]->maxZ = roiP->maxZ;
  kP->roiP[size]->minX = roiP->minX;
  kP->roiP[size]->minY = roiP->minY;
  kP->roiP[size]->minZ = roiP->minZ;
}
void State::DeleteCluster(int index){
  kP->isNormalize.erase(kP->isNormalize.begin()+index);
  kP->dimension.erase(kP->dimension.begin()+index);
  kP->isSplit.erase(kP->isSplit.begin()+index);
  kP->numCluster.erase(kP->numCluster.begin()+index);
  kP->weights[index].free();
  kP->weights.erase(kP->weights.begin()+index);
  kP->roiP[index]->splitVisible.clear();
  kP->roiP.erase(kP->roiP.begin()+index);
}
void State::UpdateCluster(int index, int num, bool split, double *weights, ROIProperty *roiP){
  kP->isSplit[index] = split;
  kP->numCluster[index] = num;
  for(int i=0;i<QDOT_DIMENSION;i++){
    kP->weights[index][i] = weights[i];
  }
  if(roiP ==NULL){kP->roiP[index]=NULL;return;}
  int size = index;
  for(int i=0;i<roiP->splitVisible.size();i++){
    kP->roiP[size]->splitVisible[i] = roiP->splitVisible[i];
  }
  kP->roiP[size]->maxMag = roiP->maxMag;
  kP->roiP[size]->minMag = roiP->minMag;
  kP->roiP[size]->maxX = roiP->maxX;
  kP->roiP[size]->maxY = roiP->maxY;
  kP->roiP[size]->maxZ = roiP->maxZ;
  kP->roiP[size]->minX = roiP->minX;
  kP->roiP[size]->minY = roiP->minY;
  kP->roiP[size]->minZ = roiP->minZ;
}

void State::NewSymmetryProperty(svVector3 pos, svVector3 dir,
                                svVector3 x, svVector3 y,
                                svVector3 planepos, svVector3 planedir,
                                svScalar planedistance,
                                svScalar angle, svScalar mag){
  symP->pos = pos;
  symP->dir = dir;
  symP->x = x;
  symP->y = y;
  symP->planepos = planepos;
  symP->planedir = planedir;
  symP->planedistance = planedistance;
  symP->angle_uncertain = angle;
  symP->mag_uncertain = mag;
}

void State::NewSliceProperty(svVector3 pos, svVector3 dir, svScalar distance){
  sP->pos = pos;
  sP->dir = dir;
  sP->distance = distance;
}
void State::NewROIProperty(svScalar maxMag, svScalar minMag,
                           svScalar maxX, svScalar maxY, svScalar maxZ,
                           svScalar minX, svScalar minY, svScalar minZ,
                           int splitSize){
  rP->maxMag = maxMag;
  rP->minMag = minMag;
  rP->maxX = maxX;
  rP->minX = minX;
  rP->maxY = maxY;
  rP->minY = minY;
  rP->maxZ = maxZ;
  rP->minZ = minZ;
  rP->splitVisible.clear();
  for(int i=0;i<splitSize;i++)
    rP->splitVisible.push_back(true);//splitVisible[i]);
}
void State::NewROIProperty(svScalar maxMag, svScalar minMag,
                           svScalar maxX, svScalar maxY, svScalar maxZ,
                           svScalar minX, svScalar minY, svScalar minZ,
                           svScalar maxPX, svScalar maxPY, svScalar maxPZ,
                           svScalar minPX, svScalar minPY, svScalar minPZ,
                           int splitSize){
  rP->maxMag = maxMag;
  rP->minMag = minMag;
  rP->maxX = maxX;
  rP->minX = minX;
  rP->maxY = maxY;
  rP->minY = minY;
  rP->maxZ = maxZ;
  rP->minZ = minZ;
  rP->maxPX = maxPX;
  rP->minPX = minPX;
  rP->maxPY = maxPY;
  rP->minPY = minPY;
  rP->maxPZ = maxPZ;
  rP->minPZ = minPZ;
  if(rP->splitVisible.size()>0)
  rP->splitVisible.clear();
  for(int i=0;i<splitSize;i++)
    rP->splitVisible.push_back(true);//splitVisible[i]);
}

void State::NewROIProperty(svScalar maxMag, svScalar minMag,
                           svScalar maxX, svScalar maxY, svScalar maxZ,
                           svScalar minX, svScalar minY, svScalar minZ,
                           svScalar maxPX, svScalar maxPY, svScalar maxPZ,
                           svScalar minPX, svScalar minPY, svScalar minPZ,
                           vector<bool> splitVisible){
  rP->maxMag = maxMag;
  rP->minMag = minMag;
  rP->maxX = maxX;
  rP->minX = minX;
  rP->maxY = maxY;
  rP->minY = minY;
  rP->maxZ = maxZ;
  rP->minZ = minZ;
  rP->maxPX = maxPX;
  rP->minPX = minPX;
  rP->maxPY = maxPY;
  rP->minPY = minPY;
  rP->maxPZ = maxPZ;
  rP->minPZ = minPZ;
  rP->splitVisible.clear();
  for(int i=0;i<splitVisible.size();i++)
    rP->splitVisible.push_back(splitVisible[i]);
}

void State::NewROIProperty(svScalar maxMag, svScalar minMag,
                           svScalar maxX, svScalar maxY, svScalar maxZ,
                           svScalar minX, svScalar minY, svScalar minZ,

                           vector<bool> splitVisible){
  rP->maxMag = maxMag;
  rP->minMag = minMag;
  rP->maxX = maxX;
  rP->minX = minX;
  rP->maxY = maxY;
  rP->minY = minY;
  rP->maxZ = maxZ;
  rP->minZ = minZ;

  rP->splitVisible.clear();
  for(int i=0;i<splitVisible.size();i++)
    rP->splitVisible.push_back(splitVisible[i]);
}
void State::UpdateSplitVisible(vector<bool> splitVisible){
  for(int i=0;i<splitVisible.size();i++)
    rP->splitVisible[i] = splitVisible[i];
}
void State::UpdateSplitVisible(int lsplit, int hsplit){
  for(int i=0;i<rP->splitVisible.size();i++)
  {
    if(i>=lsplit && i<=hsplit)
       rP->splitVisible[i] = true;
    else
       rP->splitVisible[i] = false;
  }
}
void State::UpdateMag(svScalar maxMag, svScalar minMag){
  rP->maxMag = maxMag;
  rP->minMag = minMag;
}
void State::UpdateAngle(svScalar maxX, svScalar maxY, svScalar maxZ,
                           svScalar minX, svScalar minY, svScalar minZ){
  rP->maxX = maxX;
  rP->minX = minX;
  rP->maxY = maxY;
  rP->minY = minY;
  rP->maxZ = maxZ;
  rP->minZ = minZ;
}
void State::UpdatePos(svScalar maxX, svScalar maxY, svScalar maxZ,
                           svScalar minX, svScalar minY, svScalar minZ){
  rP->maxPX = maxX;
  rP->minPX = minX;
  rP->maxPY = maxY;
  rP->minPY = minY;
  rP->maxPZ = maxZ;
  rP->minPZ = minZ;
}
void State::NewVisible(svQDOT *myQDOT){
  qdotVisible.clear();
  sampleVisible.clear();
  for(int i=0;i<myQDOT->qdotData.size();i++){
    qdotVisible.insert(std::pair<Spin *, bool>(&(myQDOT->qdotData[i]),true));
    sampleVisible.insert(std::pair<Spin *, bool>(&(myQDOT->qdotData[i]),true));
  }
}
void State::UpdateVisible(svQDOT *myData){//svQDOT *myQDOT){
//  for(int i=0;i<myData->splitData.size();i++){
//   for(int j=0;j<myData->splitData[i].size();j++){
  for(int i=0;i<myData->qdotData.size();i++){
    Spin spin = myData->qdotData[i];//*(data->splitData[i][j]);
    if(isEqualLarger(spin.den,rP->minMag,true)
    && isEqualSmaller(spin.den,rP->maxMag,true)
    && isEqualLarger(spin.xangle,rP->minX,false)
    && isEqualSmaller(spin.xangle,rP->maxX,false)
    && isEqualLarger(spin.yangle,rP->minY,false)
    && isEqualSmaller(spin.yangle,rP->maxY,false)
    && isEqualLarger(spin.zangle,rP->minZ,false)
    && isEqualSmaller(spin.zangle,rP->maxZ,false)
/*    && isEqualLarger(spin.pos[0],rP->minPX,false)
    && isEqualSmaller(spin.pos[0],rP->maxPX,false)
    && isEqualLarger(spin.pos[1],rP->minPY,false)
    && isEqualSmaller(spin.pos[1],rP->maxPY,false)
    && isEqualLarger(spin.pos[2],rP->minPZ,false)
    && isEqualSmaller(spin.pos[2],rP->maxPZ,false)*/
    && qdotVisible.at(&(myData->qdotData[i]))//rP->splitVisible[i]
    && sampleVisible.at(&(myData->qdotData[i]))){//data->splitData[i][j])){
      qdotVisible.at(&(myData->qdotData[i]))=true;//data->splitData[i][j])= true;
    }
    else{
      //cout<<i<<endl;
      qdotVisible.at(&(myData->qdotData[i]))=false;//(data->splitData[i][j])= false;
    }
 }
//  }
}

}

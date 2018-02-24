#include "svContourQDOTData.h"
#include <fstream>
#include <string.h>

using namespace std;

namespace __svl_lib {

svContourQDOTData::svContourQDOTData(svRawQDOTData *raw):svQDOTData(){
   this->raw = raw;
   contour = NULL;
   contour_valid = NULL;
   contour_display = NULL;
/*
   contour = new svVector3ArrayPtr[1];
   contour[0] = new svVector3Array[1];
   contour_count.add(0);
   contour_valid = new svVector3Array[1];
   contour_display = new svIntArray[1];
   contour_tree.resize(1);
   contour_tree[0] = new ContourTree();
*/
   contourLabel = NULL;
}

void svContourQDOTData::Reset(){
  svQDOTData::Reset();
  this->clean();
}

void svContourQDOTData::ResetData(int layer){
  svQDOTData::ResetData(layer);
  clean(layer);
}

void svContourQDOTData::New(){
  contourLabel = new svIntArray[seed_num];
  contour_valid = new svVector3Array[seed_num];   
}

void svContourQDOTData::SetContourData(char *infName, int layer){
  ifstream infile(infName);
  int num;
  infile>>num;
  for(int i=0;i<num;i++){
                svVector3 pos, vec;
                svScalar den;
                int a, r, t;
                infile>>pos[0]>>pos[1]>>pos[2]
                                >>vec[0]>>vec[1]>>vec[2]
                                >>den;//>>a>>r;//>>t;
                vec.normalize();
                glyph[layer].add(pos);
                dir[layer].add(vec);
                mag[layer].add(den);
                atom[layer].add(-1);
                region[layer].add(-1);

                svScalar tmpexp = getNumOfIntegerDigits(den);
                svScalar tmpcoe;
                tmpcoe = den/pow(10.,tmpexp);
                exp[layer].add(tmpexp);
                coe[layer].add(tmpcoe);

                clusterLabel[layer].add(-1);
                roiLabel[layer].add(true);
               if(den>0) visibleLabel[layer].add(true);
               else visibleLabel[layer].add(false);
               dataSize ++;
  }
  infile.close();
}
void svContourQDOTData::SetContourTree(char *svlfile, int layer, int index)
{
    ifstream infile(svlfile);
    int n;
    infile>>n;
    for(int i=0;i<n;i++)
    {
         int m;
         infile>>m;
         for(int j=0;j<m;j++)
         {
            svVector3 pos;
            infile>>pos[0]>>pos[1]>>pos[2];
            contour[layer][index].add(pos);
         }
    }
    infile.close();
}


void svContourQDOTData::SetContourLine(char *svlfile, int layer){
    ifstream infile(svlfile);
    int n;
    infile>>n;
    for(int i=0;i<n;i++)
    {
         int m;
         infile>>m;
         for(int j=0;j<m;j++)
         {
            svVector3 pos;
            infile>>pos[0]>>pos[1]>>pos[2];
            contour_valid[layer].add(pos);
         }
    }
    infile.close();
}

void svContourQDOTData::SetContourLabel(int cindex, int lindex, 
                                        int layer, int in){
   for(int i=cindex;i<glyph[layer].size();i++){
        contourLabel[layer].add(in);
   }
   for(int i=lindex;i<contour_valid[layer].size();i++){
        contourLineLabel[layer].add(in);
   }
}


//==========================Generate  Contours======================//
void svContourQDOTData::GenerateContours(ContourProperty property){
    //contourList.free();
    //for(int i=0;i<property.contourValues[0].size();i++)
    //    contourList.add(0);
    dataSize = 0;
    for(int j=0;j<seed_num;j++) {
      if(property.isUpdate[j] == 1) {
        for(int i=0;i<property.contourValues[j].size();i++) {
           char *contourfile = new char[200];
           sprintf(contourfile,"%s/contour%s%d%6.2e.txt", property.vtkdir,property.planename, j,property.contourValues[j][i]);
           int cindex = glyph[j].size();
           GenerateContour(contourfile, property.vtkdir,property.planename,
                                              j, property.contourValues[j][i]);
           int lindex = contour_valid[j].size();
           sprintf(contourfile, "%s/contour2d%s%d%6.2e.svl", 
                                 property.vtkdir, property.planename, 
                                 j, property.contourValues[j][i]);  
           GenerateContourLine(contourfile, property.vtkdir, property.planename,
                            j,property.contourValues[j][i]);
           SetContourLine(contourfile, j);
           SetContourLabel(cindex, lindex, j, i);
           delete [] contourfile;
       }
     }
   }
}

void svContourQDOTData::GenerateContour(char *contourfile, char *vtkdir, char *planename, int layer, float contourValue){
                char *vtkName = new char[200];
                sprintf(vtkName, "%s/%s%d.vtk", vtkdir, planename, layer);

                svContour *contourField = new svContour();
                contourField->ComputeContours(vtkName, contourfile, contourValue);

                delete contourField;
                delete [] vtkName;
                ProcessContour(contourfile, planename, vtkdir, layer);
                SetContourData(contourfile, layer);
}
void svContourQDOTData::GenerateContourLine(char *contourfile, char *vtkdir, char *planename, int layer, float contourValue)
{
    char *vtkName = new char[200];
    sprintf(vtkName, "%s/%s%d.vtk", vtkdir, planename, layer);
    svVectorField *field = new svVectorField;
    svContour *contourField = new svContour();
    contourField->ComputeContourLines(vtkName, contourfile, contourValue);
//    SetContourLine(contourfile, layer);
    delete contourField;
    delete field;
    delete [] vtkName;
}

//========================Generate Contours Done================================
/*
void svQDOTData::GenerateContourClusters(svIntArray *cluster,svIntArray *a,
                                        svIntArray *r, svVector3Array *pos){
 svNear  *near = new svNear();
 svInt *index = new svInt [dataSize];

 near->ComputeNear(glyph, pos,  seed_num, index); //need to be implemented

 int count  =0;
 for(int i=0;i<seed_num;i++){
  count = 0;
  for(int j =0;j<glyph[i].size();j++){
   clusterLabel[i][j] = cluster[i][index[count]];
   atom[i][j] = a[i][index[count]];
   region[i][j] = r[i][index[count]];
   count++;
  }
 }
 delete [] index;
}
*/

void svContourQDOTData::SetContourSampling(svInt frequency){
   int count=0;
     for(int i=0;i<seed_num;i++){
       if(glyph[i].size()>0){
        for(int j=0;j<glyph[i].size();j++){
           if(count%frequency == 0 && visibleLabel[i][j])
                 visibleLabel[i][j] = 1;
           else
                 visibleLabel[i][j] = 0;
           count++;
        }
       }
   }
}

void svContourQDOTData::SetContourVisible(int contour){
    for(int i=0;i<seed_num;i++){
          for(int j=0;j<glyph[i].size();j++){
                 if(visibleLabel[i][j] && contour == contourLabel[i][j]){
                      visibleLabel[i][j] = true;//cerr<<contourLabel[i][j]<<" ";
                 }
                 else
                 {
                      visibleLabel[i][j] = false;
                 }
          }
    }
}

void svContourQDOTData::SetContourVisible(int layer, int contour){
     if(layer>seed_num-1) return;

     int i = layer;
          for(int j=0;j<glyph[i].size();j++){
                 if(visibleLabel[i][j] && contour == contourLabel[i][j]){
                      visibleLabel[i][j] = true;//cerr<<contourLabel[i][j]<<" ";
                 }
                 else
                 {
                      visibleLabel[i][j] = false;
                 }
          }
}
//=================ContourTree=============================//
void svContourQDOTData::SetBound(svInt v1, svInt v2, svInt step)
{
   contour_tree.resize(seed_num);
   for(int i=0;i<seed_num;i++)
   { 
      contour_tree[i] = new ContourTree();
      contour_tree[i]->SetVolumeBound(v1,v2,step);
   }
}

void svContourQDOTData::GenerateContourTree(ContourTreeProperty property){
   svVectorField *field;
   svContourTree *contourtree;

   contourtree = new svContourTree();
   contourtree->ComputeContourTree(property);
   for(int i=0;i<seed_num;i++)
   {
        char *str = new char[400];
        sprintf(str,"%s/contourtree%s%d.txt",property.dir, property.planename,i);
//cerr<<str<<endl;
        contour_tree[i]->Construct(str);
        contour_tree[i]->PruneVolume();
        delete [] str;
   }   
}

void svContourQDOTData::GenerateContourTreeContours(ContourProperty property){//char *vtkdir, char *planename){
    contour = new svVector3ArrayPtr[seed_num];
    contour_display = new svIntArray[seed_num];
    char *contourfile = new char[400];
    //#pragma omp parallel for
    for(int i=0;i<seed_num;i++){//cerr<<i<<" "<<seed_num<<endl;
         int size = contour_tree[i]->GetNodeSize(); 
         contour[i] = new svVector3Array[size];
         contour_count.add(size);
         //#pragma omp parallel for
         for(int j=0;j<size;j++){
//            char *contourfile = new char[400];
//cerr<<j<<" "<<size<<endl;
            if(contour_tree[i]->GetIsoValue(j)>0){
//cerr<<j<<" "<<size<<endl;
            sprintf(contourfile, "%s/contour2d%s%d%6.2e.svl", //name needs to be fixed
                                  property.vtkdir, property.planename, i,
                                  contour_tree[i]->GetIsoValue(j));
            ifstream infile(contourfile);
            if(!infile.is_open()){
                infile.close();
                  cerr<<" open"<<endl;
                  GenerateContour(contourfile, property.vtkdir, property.planename,
                            i, contour_tree[i]->GetIsoValue(j));
            }
            else{
               infile.close();
            }
            SetContourTree(contourfile, i,j);
//            contour_display[i].add(0);
            }
         //   delete [] contourfile;
 //           cerr<<"--"<<endl;
         }
    }
    delete [] contourfile;
//cerr<<"contour treedone!"<<endl;
}

void svContourQDOTData::GenerateVisibility(svInt bound, svScalar threshold)
{
    for(int i=0;i<seed_num;i++)
    {
//cerr<<contour_count[i]<<" "<<contour_display[i].size()<<endl;
        for(int j=0;j<contour_count[i];j++)
               contour_display[i][j] = 0;
//cerr<<i<<endl;
        svIntArray index;
        contour_tree[i]->GetDisplayIndex(bound, threshold, index);

        for(int j=0;j<index.size();j++)
        {
               contour_display[i][index[j]] = 1;
        }
        index.free();
//cerr<<i<<endl;
    }
}


 
svContourQDOTData::~svContourQDOTData()
{
   clean();
}

void svContourQDOTData::clean(int j){
       contourLabel[j].free();
       contourLineLabel[j].free();
       contour_valid[j].free();
}

void svContourQDOTData::clean()
{
   for(int i=0;i<seed_num;i++)
   {
       for(int j=0;j<contour_count[i];j++)
       {
           contour[i][j].free();
       }//cerr<<"tree"<<endl;
       contour_display[i].free();//cerr<<"tree"<<endl;
       delete [] contour[i];//cerr<<"tree"<<endl;
       delete contour_tree[i];//cerr<<"tree"<<endl;
   }
   contour_tree.clear();
   delete [] contour_display;
   delete [] contour;
   contour_count.free();

   for(int i=0;i<seed_num;i++)
   {
       contourLabel[i].free();
       contourLineLabel[i].free();
       contour_valid[i].free();
   }
   delete [] contourLabel;
   delete [] contourLineLabel;
   delete [] contour_valid;
   contourList.free();
}


void svContourQDOTData::SaveContour(ContourProperty property){
     ofstream outfile(property.wholefile);
     outfile<<seed_num<<endl;
     for(int i=0;i<seed_num;i++)
     {
        outfile<<glyph[i].size()<<endl;
        for(int j=0;j<glyph[i].size();j++)
        {
            outfile<<glyph[i][j][0]<<" "<<glyph[i][j][1]<<" "<<glyph[i][j][2];
            outfile<<" "<<dir[i][j][0]<<" "<<dir[i][j][1]<<" "<<dir[i][j][2];
            outfile<<" "<<mag[i][j]<<endl;
        }
     }
     outfile.close();
}


}

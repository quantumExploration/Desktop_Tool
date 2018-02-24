#include "dataApi.h"
#include <string.h>
dataApi::dataApi(){
  //myData = new svQDOTData();
  raw = new svRawQDOTData();
  contour = new svContourQDOTData(raw);
}
dataApi::~dataApi(){
  //delete myData;
  delete raw;
  delete contour;
}
void dataApi::init(char *dir, char *file, char *tmpdir,
                        svVector3 pcenter, svVector3 pvector,
                        svScalar pdistance, int format){
        char *qdot_format = new char[400];
        sprintf(qdot_format,"%s/%s/format",
                    tmpdir, file);
//preprocess the data
        raw->SetVTK(dir, file, 
                           tmpdir,
                           "sort",
                           "twod",
                           "format",
                           "density",
                           pcenter, pvector, pdistance, 
                           format);
//set the format, such as the number of layers etc.
        raw->NewFormat(qdot_format);
        contour->NewFormat(qdot_format);
        char *str = new char[200];
//generate qdot surface
        sprintf(str, "%s/%s/",tmpdir, file);
        raw->NewMesh(str, "point", "mesh");//TODO
      delete [] str;
      delete [] qdot_format;
      raw->New();
      contour->New();
}


void dataApi::setContourData(ContourProperty cproperty){//SymmetryProperty sproperty){
   contour->GenerateContours(cproperty);
}

void dataApi::setRawData(char *dir, char *file, SymmetryProperty sproperty){
 char *str = new char[400];
 svVector3 min_plane = raw->GetMinPlane();
 svVector3 plane_dir = raw->GetPlaneDir();
 sprintf(str, "%s/%s/sort%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f",
              dir, file, 
              min_plane[0], min_plane[1],min_plane[2],
              plane_dir[0], plane_dir[1], plane_dir[2]);
 raw->SetData(str);
 sprintf(str, "%s/%s/twod%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f",
              dir, file,
              min_plane[0], min_plane[1],min_plane[2],
              plane_dir[0], plane_dir[1], plane_dir[2]);
 raw->Set2DData(str);
 sprintf(str, "%s/%s/",dir, file);
 raw->SetInside(str, "compareinout");
 sprintf(str, "%s/%s/format",
              dir, file);
 raw->SetFormat(str);
 raw->GenerateSymmetry(sproperty);
 //symmetrycount = myData->GetSymmetryMagProb(symmetrytopvalue);
 delete [] str;
}
/*
void dataApi::setContourData(svChar *dir, svInt layer, svScalar value){
 char *planename = new char[100];
 svVector3 min_plane = contour->GetMinPlane();
 svVector3 plane_dir = contour->GetPlaneDir();
 sprintf(planename, "%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f",
               min_plane[0], min_plane[1],min_plane[2],
              plane_dir[0], plane_dir[1], plane_dir[2]);
 //contour->GenerateContour(dir, planename, layer ,value);
 delete [] planename;
}
*/
/*
void dataApi::setSingleContour(svChar *dir, svChar *planename,
                               ContourProperty property, 
                               svInt contour_layer, svScalar contour_mag){
 //contour->GenerateContour(dir, planename, contour_layer, contour_mag);
 contour->SaveContour(property);
}
*/
void dataApi::setContourTree(ContourTreeProperty property,
                             ContourProperty cproperty)
{//svInt vbound0, svInt vbound1, svInt vstep,
//                         svInt vbound, ContourTreeProperty property,
///                         ContourProperty cproperty,
//                         svScalar threshold){
  //contour->SetBound(vbound0, vbound1, vstep);
  contour->GenerateContourTree(property);
  contour->GenerateContourTreeContours(cproperty);
  //contour->GenerateVisibility(vbound, threshold);
/* myData->ResetContourTree();
 myData->GeneratePixels();
 myData->SetBound(vbound0, vbound1, vstep);
 myData->GenerateContourTree(property);
 myData->GenerateContours(property.dir);
 myData->GenerateVisibility(vbound, threshold);
*/}

void dataApi::resetVisible(){
  raw->ResetVisible();
}
void dataApi::setVisible(vector<int> symmetrytype, 
                         svIntArray visiblelayer,
                         int frequency, svScalar min_mag, svScalar max_mag,
                         svVector3 l, svVector3 r,
                         svScalar min_xangle,svScalar max_xangle,
                         svScalar min_yangle, svScalar max_yangle,
                         svScalar min_zangle, svScalar max_zangle,
                         int samplesize){
 raw->ResetVisible();
 raw->SetSampling(frequency);
 raw->SetSymmetryVisible(symmetrytype);
 raw->SetLayerVisible(visiblelayer);
 raw->SetVisible(min_mag, max_mag);
 raw->SetVisible(l, r);
 raw->SetAngleVisible(min_xangle, min_yangle, min_zangle,
                         max_xangle, max_yangle, max_zangle);
 raw->SetSampleData(samplesize);
}
void dataApi::resetContourVisible(){
  contour->ResetVisible();
}
void dataApi::setContourVisible(
                         svIntArray visiblelayer,
                         int frequency, svScalar min_mag, svScalar max_mag,
                         svVector3 l, svVector3 r,
                         svScalar min_xangle,svScalar max_xangle,
                         svScalar min_yangle, svScalar max_yangle,
                         svScalar min_zangle, svScalar max_zangle,
                         int samplesize){
 contour->ResetVisible();
 contour->SetContourSampling(frequency);   
 contour->SetLayerVisible(visiblelayer);
 contour->SetVisible(min_mag, max_mag);
 contour->SetVisible(l, r);
 contour->SetAngleVisible(min_xangle, min_yangle, min_zangle,
                         max_xangle, max_yangle, max_zangle);
 contour->SetSampleData(samplesize);
}

void dataApi::testvis(int layer, int index, svVector3 l, svVector3 r){
//svVector3 l(-25,-25,-2);
//svVector3 r(25,25,2);
// myData->SetVisible(l, r);
// myData->SetRegionVisible();
raw->SetClusterVisible(layer, index);
raw->SetVisible(l, r);
}

void dataApi::setCluster(svScalar min_mag1, svScalar min_mag2,
                         svScalar max_mag1, svScalar max_mag2,
                         KmeansProperty kproperty1, KmeansProperty kproperty2,
                         SymmetryProperty sproperty, vector<int> symmetrytype){
 raw->ResetCluster();
 raw->SetROI(min_mag1, max_mag1);
 raw->GenerateClustersBySymmetry(kproperty1, sproperty, symmetrytype);
 raw->SetROI(min_mag2, max_mag2);
 raw->GenerateClustersBySymmetry(kproperty2, sproperty, symmetrytype);
}

void dataApi::setContourCluster(svScalar min_mag1, svScalar min_mag2,
                         svScalar max_mag1, svScalar max_mag2,
                         KmeansProperty kproperty1, KmeansProperty kproperty2){
 contour->ResetCluster();
 contour->SetROI(min_mag1, max_mag1);
 contour->GenerateClusters(kproperty1);
 contour->SetROI(min_mag2, max_mag2);
 contour->GenerateClusters(kproperty2);
}

void dataApi::setContourVariables(svIntArray *cluster, svIntArray *a,
                                        svIntArray *r, svVector3Array *pos){
 contour->GenerateContourClusters(cluster, a, r, pos);
}

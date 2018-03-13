//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//
// Author: Louis Bavoil
// Email: sdkfeedback@nvidia.com
//
// Depth peeling is traditionally used to perform order independent transparency (OIT)
// with N geometry passes for N transparency layers. Dual depth peeling enables peeling
// N transparency layers in N/2+1 passes, by peeling from the front and the back
// simultaneously using a min-max depth buffer. This sample performs either normal or
// dual depth peeling and blends on the fly.
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma warning( disable : 4996 )

#include "testapp.h"

glyphInfo glyphinfo;
enableInfo enableinfo;
guiInfo guiinfo;
imageInfo imageinfo;
dataInfo datainfo;
sliceInfo sliceinfo;
symInfo syminfo;
clusterInfo clusterinfo;
contourInfo contourinfo;

using namespace std;

void initParameters()
{
  contourinfo.showcontour =false;

  glyphinfo.glyph = true;
  glyphinfo.type = splitvectorsencode;
  glyphinfo.color = c_magnitude;
  glyphinfo.arrowradius = 0.05 *1.2;
  glyphinfo.tuberadius = 0.05;
  glyphinfo.length = 0.05;
//  glyphinfo.encodetype = 0;
  glyphinfo.sample = 1;
//  glyphinfo.zmin = 0;
//  glyphinfo.zmax = 1000;

  guiinfo.guitype = gui_none;//-1;//gui_slice;//slider;
  guiinfo.left = g_imageWidth/6.;
  guiinfo.slice = 0;
  guiinfo.button = 0;
  guiinfo.widget =0;
  guiinfo.widgetrepeat = 0;
  guiinfo.slider = 0;

  imageinfo.percentage = 0.8;
  imageinfo.imageType = magnitude_image;
  imageinfo.imageMouseType = one_layer;//layer; 

  clusterinfo.roicluster = 0;

  enableinfo.enableMesh = false;
  enableinfo.enableClusterBoundary = true;
}

void readInput(char *file)
{
  string tmp;
  ifstream infile(file);
  infile>>tmp;
  infile>>datainfo.dir;
  infile>>tmp;  infile>>datainfo.tmpdir;
  infile>>tmp;  infile>>datainfo.name;
  infile>>tmp;  infile>>datainfo.type;

  mkdir((datainfo.tmpdir+datainfo.name).c_str(),
        S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  sliceinfo.showslice = true;
  infile>>tmp;  infile>>sliceinfo.pos[0]>>sliceinfo.pos[1]>>sliceinfo.pos[2];
  infile>>tmp;  infile>>sliceinfo.dir[0]>>sliceinfo.dir[1]>>sliceinfo.dir[2];
  sliceinfo.dir = normalize(sliceinfo.dir);
  infile>>tmp;  infile>>sliceinfo.distance;

  infile>>tmp;  infile>>contourinfo.type;
  infile>>tmp;  infile>>contourinfo.value;

  syminfo.showsymmetry = 1;
  infile>>tmp;  infile>>syminfo.pos[0]>>syminfo.pos[1]>>syminfo.pos[2];
  infile>>tmp;  infile>>syminfo.x[0]>>syminfo.x[1]>>syminfo.x[2];
  infile>>tmp;  infile>>syminfo.y[0]>>syminfo.y[1]>>syminfo.y[2];
  infile>>tmp;  infile>>syminfo.dir[0]>>syminfo.dir[1]>>syminfo.dir[2];
  syminfo.x = normalize(syminfo.x);
  syminfo.y = normalize(syminfo.y);
  syminfo.dir = normalize(syminfo.dir);

  infile>>tmp;  infile>>clusterinfo.split;
  infile>>tmp;  infile>>clusterinfo.num;
  infile>>tmp;  //for(int i=0;i<3;i++)infile>>clusterinfo.weight[i];
  infile>>clusterinfo.weight[0];clusterinfo.weight[1]=clusterinfo.weight[0];
  clusterinfo.weight[2]=clusterinfo.weight[0];
  infile>>clusterinfo.weight[3];clusterinfo.weight[4]=clusterinfo.weight[3];
  clusterinfo.weight[5]=clusterinfo.weight[3];
  infile>>clusterinfo.weight[6];

  cout<<tmp<<endl;

  infile.close();

  initParameters();
}

void init()
{
  cerr<<"set data"<<endl;
  myState = new State();
  roip1 = new ROIProperty();
  roip2 = new ROIProperty();
  myState->rP = roip2;

  string str = datainfo.tmpdir + datainfo.name;
  rawData = new svQDOT();
  rawData->SetData(datainfo.type, 
                   (char*)datainfo.dir.c_str(), 
                   (char *)str.c_str(),
                   (char*)datainfo.name.c_str());
  sliceside = (rawData->rbbox[0]-rawData->lbbox[0])*(rawData->rbbox[0]-rawData->lbbox[0])
         + (rawData->rbbox[1]-rawData->lbbox[1])*(rawData->rbbox[1]-rawData->lbbox[1])
         + (rawData->rbbox[2]-rawData->lbbox[2])*(rawData->rbbox[2]-rawData->lbbox[2]);
  sliceside = sqrt(sliceside);

cerr<<"meshData"<<endl;
  meshData = new svMeshData(rawData);
  meshData->SetData((char *)str.c_str());
  meshRender = new svMesh(meshData);
  meshRender->GenerateGlyph(viewproperty);
  meshRender->GenerateSurface();

cerr<<"sliceData"<<endl;
  sliceData = new svRawSliceData(rawData);
  sliceData->state = myState;

cerr<<"summetryData"<<endl;
  symmetryData = new svRawSymmetryData(rawData);
  button = new svSymmetryButtons(symmetryData);
  symmetryData->state = myState;

cerr<<"clusterData"<<endl;
  clusterData = new svRawClusterData(rawData);
  clusterData->state = myState;

cerr<<"update"<<endl;
  sliceData->SetCluster(clusterData);
  sliceData->SetSymmetry(symmetryData);  
  clusterData->SetSymmetry(symmetryData);
  clusterData->SetSlice(sliceData);
  sliceData->symmetryList = symmetryData->symmetryList;
  clusterData->symmetryList = symmetryData->symmetryList;

cerr<<"contourdata"<<endl;
 
  contourData = new svContourData(rawData, sliceData);
  contourData->state = myState;

  contourClusterData = new svContourClusterData(rawData, contourData);
  contourClusterData->state = myState;
  contourClusterData->mySlice = sliceData;
  contourData->SetCluster(contourClusterData);
//  contourData->GenerateContourTree((char*)str.c_str());

  contourClusterData->qdotVisible = contourData->qdotVisible;
  contourClusterData->sampleVisible = contourData->sampleVisible;

  symmetryData->qdotVisible = sliceData->qdotVisible;
  clusterData->qdotVisible = sliceData->qdotVisible;
  symmetryData->sampleVisible = sliceData->sampleVisible;
  clusterData->sampleVisible = sliceData->sampleVisible;

  SetSymmetryState();
  SetClusterState();

cerr<<"update"<<endl;
  Update();
 
cerr<<"color"<<endl; 
  myColor = new svColors();
  myColor->SetAlpha(1.);
  myColor->SetColorBlind(false);
  myColor->SetColorType(glyphinfo.color);

cerr<<"slice"<<endl;
  mySlice = new svImageList(sliceData);
  mySlice->SetColors(myColor);
  mySlice->AddHybridImage(contourData, str, 10);
  mySlice->myState = myState;
  
cerr<<"glyphs"<<endl;
  lc = new svLCArrow(sliceData);
  lc->SetRadius(glyphinfo.arrowradius*1.2);//0.1
  lc->SetScale(glyphinfo.length);
  lc->SetTubeRadius(glyphinfo.arrowradius);//0.05

  linear = new svLinearArrow(sliceData);
  linear->SetRadius(glyphinfo.arrowradius*1.2);
  linear->SetScale(glyphinfo.length*LINEAR_SCALE);
  linear->SetTubeRadius(glyphinfo.arrowradius);

  lt = new svLTArrow(sliceData);
  lt->SetRadius(glyphinfo.arrowradius*1.2);
  lt->SetScale(glyphinfo.length);
  lt->SetTubeRadius(glyphinfo.arrowradius);

  lc_contour = new svLCArrow(contourData);
  lc_contour->SetRadius(glyphinfo.arrowradius*1.2);//0.1
  lc_contour->SetScale(glyphinfo.length);
  lc_contour->SetTubeRadius(glyphinfo.arrowradius);//0.05

  linear_contour = new svLinearArrow(contourData);
  linear_contour->SetRadius(glyphinfo.arrowradius*1.2);
  linear_contour->SetScale(glyphinfo.length*LINEAR_SCALE);
  linear_contour->SetTubeRadius(glyphinfo.arrowradius);

  lt_contour = new svLTArrow(contourData);
  lt_contour->SetRadius(glyphinfo.arrowradius*1.2);
  lt_contour->SetScale(glyphinfo.length);
  lt_contour->SetTubeRadius(glyphinfo.arrowradius);

  cerr<<"Generate widgets"<<endl;
  slice = new svSlice();
  outline = new svOutline();
  outline_display_list = 9;

  widget = new svWidget(sliceData);
  widget->state = myState;
  widget->Repeat(guiinfo.widgetrepeat);//false);
  widget->SetIndex(0, sliceData->splitData.size()-1, sliceData->splitData.size()-1);

  slider = new svROISlider(sliceData);
  slider->state = myState;

  button->myState = myState;
  button->SetColor(myColor);

  mouseGroup = new svMouseGroup();
  mouseGroup->AddWidgetMouse(widget);
  mouseGroup->AddROISliderMouse(slider);
  mouseGroup->AddImageMouse(mySlice, imageinfo.imageMouseType);
  mouseGroup->AddButtonMouse(button);
  switch (guiinfo.guitype)
  {
       case (gui_widget): mouseGroup->currentMouse=mouse_widget; break;
       case (gui_slider): mouseGroup->currentMouse=mouse_slider; break;
       case (gui_slice): mouseGroup->currentMouse=mouse_image;break;
       case (gui_button): mouseGroup->currentMouse=mouse_button;break;
       case (gui_none): mouseGroup->currentMouse=-1;break;
  }

  mouseHandle = new svMouseInteraction();
  mouseHandle->group = mouseGroup;

  cerr<<"update render"<<endl;
  UpdateRender();

  cerr<<"processing done...."<<endl;
}

void ResetROI()
{
  roip1->minMag = 0; roip1->maxMag = 9e+9;
  roip1->splitVisible.clear();
  roip1->sliceVisible.clear();
  roip1->sliceVisible.resize(sliceData->splitData.size());
  roip1->splitVisible.resize(sliceData->splitData.size());
  for(int i=0;i<sliceData->splitData.size();i++) roip1->splitVisible[i]=true;
  for(int i=0;i<sliceData->splitData.size();i++) roip1->sliceVisible[i]=false;
  for(int i=0;i<9;i++) roip1->symmetryVisible[i]=true;
  roip1->maxX = 180; roip1->minX = 0;
  roip1->maxY = 180; roip1->minY = 0;
  roip1->maxZ = 180; roip1->minZ = 0;
  roip1->maxPX = rawData->rbbox[0]; roip1->minPX = rawData->lbbox[0];
  roip1->maxPY = rawData->rbbox[1]; roip1->minPY = rawData->lbbox[1];
  roip1->maxPZ = rawData->rbbox[2]; roip1->minPZ = rawData->lbbox[2];

  roip2->minMag = 0; roip2->maxMag = 9e+9;
  roip2->splitVisible.clear();
  roip2->sliceVisible.clear();
  roip2->sliceVisible.resize(sliceData->splitData.size());
  roip2->splitVisible.resize(sliceData->splitData.size());
  for(int i=0;i<9;i++) roip2->symmetryVisible[i]=true;
  for(int i=0;i<sliceData->splitData.size();i++) roip2->sliceVisible[i]=false;
  for(int i=0;i<sliceData->splitData.size();i++) roip2->splitVisible[i]=true;
  roip2->maxX = 180; roip2->minX = 0;
  roip2->maxY = 180; roip2->minY = 0;
  roip2->maxZ = 180; roip2->minZ = 0;
  roip2->maxPX = rawData->rbbox[0]; roip2->minPX = rawData->lbbox[0];
  roip2->maxPY = rawData->rbbox[1]; roip2->minPY = rawData->lbbox[1];
  roip2->maxPZ = rawData->rbbox[2]; roip2->minPZ = rawData->lbbox[2];
}

void Update()
{
  UpdateSlice();
  ResetROI();
  SetContourState();
  UpdateSymmetry();
  UpdateCluster();
  UpdateContour();
  UpdateContourCluster();
}

void UpdateSlice()
{
  cout<<"Generate Slice .."<<endl;
  sliceinfo.dir = normalize(sliceinfo.dir);
  cerr<<sliceinfo.dir[0]<<" "<<sliceinfo.dir[1]<<" "<<sliceinfo.dir[2]<<endl;
  sliceData->Init(sliceinfo.pos, sliceinfo.dir, sliceinfo.distance);
  sliceData->SaveSlicetoVTK((char*)datainfo.tmpdir.c_str(), (char*)datainfo.name.c_str());
  sliceData->SaveSliceDensitytoFile((char*)(datainfo.tmpdir+datainfo.name).c_str());
  sliceData->SetImageLabel(imageinfo.imageType);
}

void UpdateContour()
{
  cout<<"Generate Contours ..."<<endl;
  contourData->New(rawData, sliceData);
  contourData->GenerateContourTree((char*)(datainfo.tmpdir+datainfo.name).c_str());
  cout<<"Generate contours .."<<endl;
  contourData->GenerateContours((char *)(datainfo.tmpdir+datainfo.name).c_str());
}

void UpdateContourCluster()
{
  cout<<"Generate contour cluster.."<<endl;
  contourClusterData->New(rawData, contourData);
  contourClusterData->GenerateClusters();
}

void SetContourState()
{
  myState->cP->isUpdate.clear(); 
  if(myState->cP->seed_num>0){
    for(int i=0;i<myState->cP->seed_num;i++)
    {
      myState->cP->values[i].free();
      myState->cP->type[i].free();
    }
    delete [] myState->cP->values;
    delete [] myState->cP->type;
  }
  myState->cP->seed_num = sliceData->splitData.size();
  myState->cP->values = new svScalarArray[myState->cP->seed_num];
  myState->cP->type = new svIntArray[myState->cP->seed_num];
  for(int i=0;i<myState->cP->seed_num;i++)
    {
  myState->cP->type[i].add(contourinfo.type);
  myState->cP->values[i].add(contourinfo.value); 
  myState->cP->isUpdate.push_back(true);
  }
}

void SetSymmetryState()
{
  svVector3 planepos(0,0,-10);
  svVector3 planedir(0,0,1);
  svScalar angle = 10;
  svScalar mag = 1e-7;
  myState->NewSymmetryProperty(syminfo.pos,
                               syminfo.dir,
                               syminfo.x,
                               syminfo.y,
                               planepos, planedir, rawData->minZDistance, 
                               angle,mag);
}
void SetClusterState()
{
  vector<int> num;//um.push_back(40); //num.push_back(15);
  num.push_back(clusterinfo.num);
  if(clusterinfo.roicluster) num.push_back(clusterinfo.roinum);
  vector<bool> split;
  split.push_back(clusterinfo.split);
  split.push_back(clusterinfo.split);
  double **weights = new double*[2];
  weights[0]=new double[7];weights[1]=new double[7];
  for(int i=0;i<7;i++)weights[0][i] = clusterinfo.weight[i];
  for(int i=0;i<7;i++)weights[1][i] = clusterinfo.roiweight[i];
  vector<ROIProperty *>p; p.push_back(roip1);p.push_back(roip2);
  myState->NewClusterProperty(num,split,weights,p);
/*
  vector<bool> split; split.push_back(false); split.push_back(false);
  double **weights = new double*[2];weights[0]=new double[7];weights[1]=new double[7];
  for(int i=0;i<7;i++)weights[0][i] = 1; weights[0][6] = 1.;
  for(int i=0;i<7;i++)weights[1][i] = 1; weights[1][6] = 1.;
  ROIProperty *roiP1=new ROIProperty(); roiP1->maxMag =1; roiP1->minMag =0;
  roiP1->splitVisible.resize(109);
  for(int i=0;i<roiP1->splitVisible.size();i++) roiP1->splitVisible[i]=true;
  roiP1->maxX = 180; roiP1->minX = 0;
  roiP1->maxY = 180; roiP1->minY = 0;
  roiP1->maxZ = 180; roiP1->minZ = 0;
  vector<ROIProperty *>p; p.push_back(roiP1);
  ROIProperty *roiP2=new ROIProperty(); roiP2->maxMag =1; roiP2->minMag =1e-7;
  roiP2->splitVisible.resize(109);
  for(int i=0;i<roiP2->splitVisible.size();i++) roiP2->splitVisible[i]=true;
  roiP2->maxX = 180; roiP2->minX = 0;
  roiP2->maxY = 180; roiP2->minY = 0;
  roiP2->maxZ = 180; roiP2->minZ = 0;
  p.push_back(roiP2);
  myState->NewClusterProperty(num,split,weights,p);
*/
}

void UpdateSymmetry()
{
  cout<<"Generate Symmetry ..."<<endl;
  symmetryData->Init((char*)(datainfo.tmpdir+datainfo.name).c_str());
  button->Init();
}


void UpdateCluster()
{
  cout<<"Generate Cluster ..."<<endl;
  clusterData->Init((char*)(datainfo.tmpdir+datainfo.name).c_str());
  //if(enableinfo.enableClusterBoundary)clusterData->GenerateSampling(glyphinfo.sample);
}

/*void UpdateCluster()
{
  
//---------------------This part will be replaced by Parser------------//

  vector<int> num;num.push_back(40); //num.push_back(15);
  vector<bool> split; split.push_back(false); split.push_back(false);
  double **weights = new double*[2];weights[0]=new double[7];weights[1]=new double[7];
  for(int i=0;i<7;i++)weights[0][i] = 1; weights[0][6] = 1.;
  for(int i=0;i<7;i++)weights[1][i] = 1; weights[1][6] = 1.;
  ROIProperty *roiP1=new ROIProperty(); roiP1->maxMag =1; roiP1->minMag =0;
  roiP1->splitVisible.resize(109);
  for(int i=0;i<roiP1->splitVisible.size();i++) roiP1->splitVisible[i]=true;
  roiP1->maxX = 180; roiP1->minX = 0;
  roiP1->maxY = 180; roiP1->minY = 0;
  roiP1->maxZ = 180; roiP1->minZ = 0;
  vector<ROIProperty *>p; p.push_back(roiP1);
  ROIProperty *roiP2=new ROIProperty(); roiP2->maxMag =1; roiP2->minMag =1e-7;
  roiP2->splitVisible.resize(109);
  for(int i=0;i<roiP2->splitVisible.size();i++) roiP2->splitVisible[i]=true;
  roiP2->maxX = 180; roiP2->minX = 0;
  roiP2->maxY = 180; roiP2->minY = 0;
  roiP2->maxZ = 180; roiP2->minZ = 0;
  p.push_back(roiP2);
  myState->NewClusterProperty(num,split,weights,p);
  svVector3 pos(0,0,0);
  svVector3 dir(1,1,0); dir.normalize();
  svVector3 x(-1,1,0); x.normalize();
  svVector3 y(0,0,1); y.normalize();
  svScalar angle = 10;
  svScalar mag = 1e-7;
  svVector3 planepos(0,0,-10);
  svVector3 planedir(0,0,1);
  myState->NewSymmetryProperty(pos,dir,x,y,planepos, planedir, 0.25, angle,mag);
//-------------------------------------------------------------------//
}*/

void UpdateSampling()
{
   sliceData->GenerateSampling(glyphinfo.sample);
}

void UpdateGlyphs()
{
  lc->SetRadius(glyphinfo.arrowradius*1.2);//0.1
  lc->SetScale(glyphinfo.length);
  lc->SetTubeRadius(glyphinfo.arrowradius);//0.05

  linear->SetRadius(glyphinfo.arrowradius*1.2);
  linear->SetScale(glyphinfo.length*LINEAR_SCALE);
  linear->SetTubeRadius(glyphinfo.arrowradius);

  lt->SetRadius(glyphinfo.arrowradius*1.2);
  lt->SetScale(glyphinfo.length);
  lt->SetTubeRadius(glyphinfo.arrowradius);

  lc_contour->SetRadius(glyphinfo.arrowradius*1.2);//0.1
  lc_contour->SetScale(glyphinfo.length);
  lc_contour->SetTubeRadius(glyphinfo.arrowradius);//0.05

  linear_contour->SetRadius(glyphinfo.arrowradius*1.2);
  linear_contour->SetScale(glyphinfo.length*LINEAR_SCALE);
  linear_contour->SetTubeRadius(glyphinfo.arrowradius);

  lt_contour->SetRadius(glyphinfo.arrowradius*1.2);
  lt_contour->SetScale(glyphinfo.length);
  lt_contour->SetTubeRadius(glyphinfo.arrowradius);

  myColor->SetColorType(c_magnitude);
  lc->UpdateData();
  lc->UpdateIndex();
  lc->SetColors(myColor);
  lc->GenerateArrows();
  lc->GenerateTubes();
  lc->GenerateIndex();
  lc->UpdateArrowVBO();
  lc->UpdateTubeVBO();
  lc->UpdateIndexVBO();

  lc_contour->UpdateData();
  lc_contour->UpdateIndex();
  lc_contour->SetColors(myColor);
  lc_contour->GenerateArrows();
  lc_contour->GenerateTubes();
  lc_contour->GenerateIndex();
  lc_contour->UpdateArrowVBO();
  lc_contour->UpdateTubeVBO();
  lc_contour->UpdateIndexVBO();

  if(glyphinfo.color == 0)//magnitude)
     myColor->SetColorType(c_magnitude);
  else if(glyphinfo.color==1)
     myColor->SetColorType(c_cluster);
  else
     myColor->SetColorType(c_symmetry);

  linear->UpdateData();
  linear->UpdateIndex();
  linear->SetColors(myColor);
  linear->GenerateArrows();
  linear->GenerateTubes();
  linear->GenerateIndex();
  linear->GenerateVBO();
  
  linear_contour->UpdateData();
  linear_contour->UpdateIndex();
  linear_contour->SetColors(myColor);
  linear_contour->GenerateArrows();
  linear_contour->GenerateTubes();
  linear_contour->GenerateIndex();
  linear_contour->GenerateVBO();

  if(glyphinfo.color == 0)
    myColor->SetColorType(c_texture_cluster);
  else if(glyphinfo.color==1)
    myColor->SetColorType(c_texture_cluster);
  else
    myColor->SetColorType(c_symmetry);

  lt->UpdateData();
  lt->UpdateIndex();
  lt->SetColors(myColor);
  lt->GenerateArrows();
  lt->GenerateTubes();
  lt->GenerateIndex();
  lt->GenerateVBO();

  lt_contour->UpdateData();
  lt_contour->UpdateIndex();
  lt_contour->SetColors(myColor);
  lt_contour->GenerateArrows();
  lt_contour->GenerateTubes();
  lt_contour->GenerateIndex();
  lt_contour->GenerateVBO();
}

void UpdateImage()
{
                mySlice->SetImageType(imageinfo.imageType);//cluster_image);
                mySlice->SetColors(myColor);
                mySlice->GenerateGroupingByColor(imageinfo.percentage);
                mySlice->AddHybridImage(contourData, datainfo.tmpdir+datainfo.name, 10);
                mySlice->GenerateDisplay();
                sliceData->SetImageLabel(imageinfo.imageType);
                mouseGroup->ResetImageLabel();cerr<<"image"<<endl;

}

void UpdateColor()
{
  if(glyphinfo.color == c_magnitude)
     myColor->SetColorType(c_magnitude);
  else if(glyphinfo.color == 1)
     myColor->SetColorType(c_cluster);
  else
     myColor->SetColorType(c_symmetry);
  linear->UpdateData();
  linear->UpdateIndex();
  linear->SetColors(myColor);
  linear->GenerateArrows();
  linear->GenerateTubes();
  linear->GenerateIndex();
  linear->GenerateVBO();

  linear_contour->UpdateData();
  linear_contour->UpdateIndex();
  linear_contour->SetColors(myColor);
  linear_contour->GenerateArrows();
  linear_contour->GenerateTubes();
  linear_contour->GenerateIndex();
  linear_contour->GenerateVBO();

  if(glyphinfo.color == c_magnitude)
     myColor->SetColorType(c_texture_cluster);
  else if(glyphinfo.color==1)
     myColor->SetColorType(c_texture_cluster);
  else
     myColor->SetColorType(c_symmetry);
  lt->UpdateData();
  lt->UpdateIndex();
  lt->SetColors(myColor);
  lt->GenerateArrows();
  lt->GenerateTubes();
  lt->GenerateIndex();
  lt->GenerateVBO();

  lt_contour->UpdateData();
  lt_contour->UpdateIndex();
  lt_contour->SetColors(myColor);
  lt_contour->GenerateArrows();
  lt_contour->GenerateTubes();
  lt_contour->GenerateIndex();
  lt_contour->GenerateVBO();
}

void UpdateRender()
{
  sliceData->SetImageLabel(imageinfo.imageType);
  mySlice->SetImageType(imageinfo.imageType);//cluster_image);
  mySlice->SetColors(myColor);
  //mySlice->Reshape(g_imageWidth/6.,g_imageHeight,g_imageWidth*5./6.);
  mySlice->GenerateGroupingByColor(imageinfo.percentage);
  mySlice->AddHybridImage(contourData, datainfo.tmpdir+datainfo.name, 10);
  mySlice->GenerateDisplay();

  UpdateGlyphs();

  testcluster= 0;
  meshCluster = clusterData->meshData;
//  meshClusterRender = new svMesh(meshCluster);
//  meshClusterRender->GenerateSurface(myColor, testcluster);
}

void UpdateVisible()
{
  sliceData->GenerateSampling(glyphinfo.sample);
  contourData->GenerateSampling(glyphinfo.sample);
  sliceData->UpdateVisible();
  contourData->UpdateVisible();
  mySlice->GenerateDisplay();
  lc->UpdateIndex();
  lc->GenerateIndex();
  lc->UpdateIndexVBO();
  linear->UpdateIndex();
  linear->GenerateIndex();
  linear->UpdateIndexVBO();
  lt->UpdateIndex();
  lt->GenerateIndex();
  lt->UpdateIndexVBO();
  lc_contour->UpdateIndex();
  lc_contour->GenerateIndex();
  lc_contour->UpdateIndexVBO();
  linear_contour->UpdateIndex();
  linear_contour->GenerateIndex();
  linear_contour->UpdateIndexVBO();
  lt_contour->UpdateIndex();
  lt_contour->GenerateIndex();
  lt_contour->UpdateIndexVBO();
}

void UpdateControl()
{
  GetSliceText();
  GetSymmetryText();
  GetContourText();
  GetClusterText();
  //if(myState->updateSlice)
  //{
  if(myState->updateSlice)
  {
    UpdateSlice();
    mySlice->New(sliceData);
    widget->Init(sliceData);
    widget->UpdateState();
    widget->Reshape(guiinfo.left,g_imageHeight-50,5,10,g_imageWidth, g_imageHeight);
    mySlice->Reshape(guiinfo.left,g_imageHeight,g_imageWidth - guiinfo.left);//*5./6.);
    mySlice->UpdateState();
    myState->updateContour = true;
  }
  if(myState->updateSymmetry)
  {
    SetSymmetryState();
    UpdateSymmetry();
    myState->updateCluster = true;
  }
  bool updatecontourcluster = false;
  if(myState->updateCluster)
  {
    SetClusterState();
    UpdateCluster(); 
    updatecontourcluster = true;
  } 
  if(myState->updateContour)
  {
    SetContourState();
    UpdateContour();
  }
  if(updatecontourcluster || myState->updateContour)
  {
    UpdateContourCluster();
  }
  UpdateRender();
  UpdateVisible();
}
void  getTime(char *buffer)
{
                        time_t rawtime;
                        struct tm * timeinfo;

                        time (&rawtime);
                        timeinfo = localtime (&rawtime);

                        sprintf(buffer,"%s", asctime(timeinfo));

}
void CaptureScreen(char *file, int startw, int starth, int w, int h)
{
  char *image_buf = new char [w*h*3];
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(startw, starth, w, h, GL_RGB, GL_UNSIGNED_BYTE, image_buf);

  FILE* fp;
  if (NULL != (fp = fopen(file, "wb"))){
    fprintf(fp, "P6 %d %d 255\n", w, h);
    for (int i=h-1; i >= 0; --i)
    {
       fwrite(image_buf+3*i*w, sizeof(unsigned char), 3*w, fp);
    }
    fclose(fp);
  }
  delete [] image_buf;
}

void ScreenShot()
{
          int x, y, w, h;
          char *str = new char[200];
          char buffer[80];
          getTime(buffer);
          sprintf(str,"%s/screenshots/%s.ppm", SRC_DIR,buffer);
          GLUI_Master.get_viewport_area(&x, &y, &w, &h);
          CaptureScreen(str, x, y, w,h);
}

void setData(int type, char *filedir, char *tmpdir, char *file,
             svVector3 center, svVector3 dir, svScalar distance){
//cerr<<"Set Data"<<endl;
//  char *str= new char[300];
//  sprintf(str,"%s/%s/",tmpdir,file);

//cout<<"Read Data.."<<endl;
//  rawData->SetData(type, filedir, str, file);

//cout<<"Generate QDOT Surface..."<<endl;
//  meshData = new svMeshData(rawData);
//  meshData->SetData(str);
//  meshRender = new svMesh(meshData);
//  meshRender->GenerateGlyph(viewproperty);
//  meshRender->GenerateSurface();

//cout<<"Generate Slices ..."<<endl;
//  sliceData = new svRawSliceData(rawData);
//  sliceData->Init(center, dir, distance);
//  sliceData->SaveSlicetoVTK(tmpdir, file);
//  sliceData->SaveSliceDensitytoFile(str);
//  sliceData->state = myState;
//  myState->NewROIProperty(1.,0.,180.,180.,180.,0.,0.,0.,sliceData->splitData.size());
//  myState->NewVisible(rawData);

//cout<<"Generate Symmetry ..."<<endl;
//  symmetryData = new svRawSymmetryData(rawData);
//  symmetryData->state = myState;
//  sliceData->SetSymmetry(symmetryData);
//  symmetryData->Init(str);

//  sliceData->GenerateSampling(glyphinfo.sample);
//  sliceData->UpdateVisibleBySplit();
//  myState->UpdateVisible(rawData);

//cout<<"Generate Cluster ..."<<endl;
//  clusterData = new svRawClusterData(rawData);
//  sliceData->SetCluster(clusterData);
//  clusterData->SetSlice(sliceData);
//  clusterData->SetSymmetry(symmetryData);
//  clusterData->state = myState;
//  clusterData->Init(str);
//  if(enableinfo.enableClusterBoundary)clusterData->GenerateSampling(glyphinfo.sample);
//  sliceData->UpdateVisibleBySplit();
//  sliceData->SetImageLabel(imageinfo.imageType);
//  myState->UpdateVisible(rawData);

//  myColor = new svColors();
//  myColor->SetAlpha(1.);
//  myColor->SetColorBlind(false);
  myColor->SetColorType(c_cluster);//magnitude);

  testcluster= 0;
  meshCluster = clusterData->meshData;
  meshClusterRender = new svMesh(meshCluster);
  meshClusterRender->GenerateSurface(myColor, testcluster);
//cout<<"Generate Contour..."<<endl;
//  contourData = new svContourData(rawData, sliceData);
//  contourData->GenerateContourTree(str);
//  contourData->GenerateContoursByContourTree(str, contourstep, contourratio);
//  contourData->GenerateSampling(sample);

//cout<<"Generate Contour Cluster..."<<endl;
//  contourClusterData = new svContourClusterData(rawData, contourData);
//  contourData->SetCluster(contourClusterData);
//  contourClusterData->state = myState;
//  contourClusterData->Init(str);


//  mySlice = new svImageList(sliceData);
//  mySlice->SetImageType(imageinfo.imageType);//cluster_image);
//  mySlice->SetColors(myColor);
//  mySlice->Reshape(g_imageWidth/6.,g_imageHeight,g_imageWidth*5./6.);
//  mySlice->GenerateGroupingByColor(0.8);
//  mySlice->AddHybridImage(contourData, string(str), 10);

cout<<"Generate Glyphs ..."<<endl;
/*  lc = new svLCArrow(sliceData);
  lc->SetRadius(glyphinfo.arrowradius);//0.1
  lc->SetScale(glyphinfo.tuberadius);
  lc->SetTubeRadius(glyphinfo.tuberadius);//0.05
  lc->UpdateData();
  lc->UpdateIndex();
  lc->SetColors(myColor);
  lc->GenerateArrows();
  lc->GenerateTubes();
  lc->GenerateIndex();
  lc->UpdateArrowVBO();
  lc->UpdateTubeVBO();
  lc->UpdateIndexVBO();
*/
//  myColor->SetColorType(c_cluster);
/*  linear = new svLinearArrow(sliceData);
  linear->SetRadius(glyphinfo.arrowradius);
  linear->SetScale(glyphinfo.tuberadius*LINEAR_SCALE);
  linear->SetTubeRadius(glyphinfo.tuberadius);
  linear->UpdateData();
  linear->UpdateIndex();
  linear->SetColors(myColor);
  linear->GenerateArrows();
  linear->GenerateTubes();
  linear->GenerateIndex();
  linear->GenerateVBO();
*/

  myColor->SetColorType(c_texture_cluster);
/*  lt = new svLTArrow(sliceData);
  lt->SetRadius(glyphinfo.arrowradius);
  lt->SetScale(glyphinfo.tuberadius);
  lt->SetTubeRadius(glyphinfo.tuberadius);
  lt->UpdateData();
  lt->UpdateIndex();
  lt->SetColors(myColor);
  lt->GenerateArrows();
  lt->GenerateTubes();
  lt->GenerateIndex();
  lt->GenerateVBO();
*/
//  cerr<<"Generate widgets"<<endl;
//  widget = new svWidget(sliceData);
//  widget->state = myState;
//  widget->Repeat(true);//false);
//  widget->SetIndex(0, 108, 108);

//  slider = new svROISlider(sliceData);
//  slider->state = myState;
//  button = new svSymmetryButtons();
//  button->SetColor(myColor);

/*  mouseGroup = new svMouseGroup();
  mouseGroup->AddWidgetMouse(widget);
  mouseGroup->AddROISliderMouse(slider);
  mouseGroup->AddImageMouse(mySlice, imageinfo.imageMouseType);
  
  switch (guiinfo.guitype)
  {
       case (gui_widget): mouseGroup->currentMouse=mouse_widget; break;
       case (gui_slider): mouseGroup->currentMouse=mouse_slider; break;
       case (gui_slice): mouseGroup->currentMouse=mouse_image;break;
       case (gui_button): mouseGroup->currentMouse=mouse_button;break;
  }

  mouseHandle = new svMouseInteraction();
  mouseHandle->group = mouseGroup;

  cout<<mouseGroup->currentMouse<<endl;
  cerr<<"processing done...."<<endl;
*/
// lt->UpdateArrowVBO();
//  lt->UpdateTubeVBO();
 // lt->UpdateIndexVBO();

//  char *osgfile = new char[100];
//  l->SaveTubetoOSG("/home/henan/Documents/NIST/NIST_SPLIT/SPLIT_VIS2/lctube.osg");
//  lc->SaveArrowtoOSG("/home/henan/Documents/NIST/NIST_SPLIT/SPLIT_VIS2/lcarrow.osg");
}

//--------------------------------------------------------------------------
void InitDualPeelingRenderTargets()
{
	glGenTextures(2, g_dualDepthTexId);
	glGenTextures(2, g_dualFrontBlenderTexId);
	glGenTextures(2, g_dualBackTempTexId);
	glGenFramebuffersEXT(1, &g_dualPeelingSingleFboId);
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//henan
		//glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_FLOAT_RG32_NV, g_imageWidth, g_imageHeight,
		 glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RG32F, g_imageWidth, g_imageHeight,
					 0, GL_RGB, GL_FLOAT, 0);

		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, g_imageWidth, g_imageHeight,
					 0, GL_RGBA, GL_FLOAT, 0);

		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, g_imageWidth, g_imageHeight,
					 0, GL_RGBA, GL_FLOAT, 0);
	}

	glGenTextures(1, &g_dualBackBlenderTexId);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, g_imageWidth, g_imageHeight,
				 0, GL_RGB, GL_FLOAT, 0);

	glGenFramebuffersEXT(1, &g_dualBackBlenderFboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualBackBlenderFboId);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
							  GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId, 0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingSingleFboId);

	int j = 0;
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[j], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[j], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[j], 0);

	j = 1;
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT3_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[j], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT4_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[j], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT5_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[j], 0);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT6_EXT,
							  GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId, 0);

	CHECK_GL_ERRORS;
}

//--------------------------------------------------------------------------
void DeleteDualPeelingRenderTargets()
{
	glDeleteFramebuffersEXT(1, &g_dualBackBlenderFboId);
	glDeleteFramebuffersEXT(1, &g_dualPeelingSingleFboId);
	glDeleteTextures(2, g_dualDepthTexId);
	glDeleteTextures(2, g_dualFrontBlenderTexId);
	glDeleteTextures(2, g_dualBackTempTexId);
	glDeleteTextures(1, &g_dualBackBlenderTexId);
}

//--------------------------------------------------------------------------
void MakeFullScreenQuad()
{
	g_quadDisplayList = glGenLists(1);
	glNewList(g_quadDisplayList, GL_COMPILE);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	{
		glVertex2f(0.0, 0.0);
		glVertex2f(1.0, 0.0);
		glVertex2f(1.0, 1.0);
		glVertex2f(0.0, 1.0);
	}
	glEnd();
	glPopMatrix();

	glEndList();
}
void BuildShaders(string SHADER_PATH)
{
	printf("\nloading shaders...\n");

	g_shaderDualInit.attachVertexShader(SHADER_PATH+"dual_peeling_init_vertex.glsl");
	g_shaderDualInit.attachFragmentShader(SHADER_PATH+"dual_peeling_init_fragment.glsl");
	g_shaderDualInit.link();

	g_shaderDualPeel.attachVertexShader(SHADER_PATH+"shade_vertex.glsl");
	g_shaderDualPeel.attachVertexShader(SHADER_PATH+"dual_peeling_peel_vertex.glsl");
	g_shaderDualPeel.attachFragmentShader(SHADER_PATH+"shade_fragment.glsl");
	g_shaderDualPeel.attachFragmentShader(SHADER_PATH+"dual_peeling_peel_fragment.glsl");
	g_shaderDualPeel.link();

	g_shaderDualBlend.attachVertexShader(SHADER_PATH+"dual_peeling_blend_vertex.glsl");
	g_shaderDualBlend.attachFragmentShader(SHADER_PATH+"dual_peeling_blend_fragment.glsl");
	g_shaderDualBlend.link();

	g_shaderDualFinal.attachVertexShader(SHADER_PATH+"dual_peeling_final_vertex.glsl");
	g_shaderDualFinal.attachFragmentShader(SHADER_PATH+"dual_peeling_final_fragment.glsl");
	g_shaderDualFinal.link();

}

void ArrowLight()
{
  GLfloat mat_ambient[] = {1,1,1,1};
  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, .8};
  GLfloat mat_specular[] = { 0.2,0.2,0.2,0.2 };
  GLfloat mat_shininess[] = { 0. };
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void InitLight()
{
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   glEnable(GL_NORMALIZE);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8,1};
  GLfloat mat_specular[] = { 0.2,0.2,0.2,0.2 };
  GLfloat light_position[] = {viewproperty.eye[0], 
                              viewproperty.eye[1], 
                              viewproperty.eye[2],1};//100,100,200, 0 };
  GLfloat white_light[] = { 0.8, 0.8, 0.8, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

  GLfloat lmodel_ambient[] = {0.2,0.2,0.2,1.0};
  GLfloat local_view[] = {0.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);


  GLfloat light1_position[] = {100,100,200,1};//eye[0], eye[1], eye[2],0};// 0,0,100,0};
  GLfloat light1_ambient[] = {0.2,0.2,0.2,1};
  GLfloat light1_diffuse[] = {0.5,0.5,0.5,1.0};
  GLfloat light1_specular[] = {0.5,0.5,0.5,1.0};
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR,light1_specular);

  ArrowLight();
}
//--------------------------------------------------------------------------
void InitGL(string shader)
{
  InitLight();
  GLfloat mat_diffuse[] = { 0.55, 0.55, 0.55, 0.4};
  GLfloat mat_specular[] = { 0.5,0.5, 0.5, 1 };
  GLfloat mat_shininess[] = { 0.4 };
  GLfloat light_position[] = { 50,50,50, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

	// Allocate render targets first
	InitDualPeelingRenderTargets();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	BuildShaders(shader);
	MakeFullScreenQuad();
	glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
//	glDisable(GL_NORMALIZE);

	glGenQueries(1, &g_queryId);
}
//--------------------------------------------------------------------------

void Render3D()
{
  glPushMatrix();
  glMultMatrixf(viewproperty.tm);

  glEnable(GL_LIGHTING);
  float g_twoSide = 1;
  g_opacity = 0.75;
  g_shaderDualPeel.setUniform("Alpha", (float*)&g_opacity, 1);
  g_shaderDualPeel.setUniform("isTwoSide", (float*)&g_twoSide, 1);
  if(guiinfo.guitype == gui_slice) mySlice->Render3D();
  if(enableQDOT) {
  //  g_shaderDualPeel.setUniform("isTwoSide", (float*)&g_twoSide, 1);
    meshRender->RenderGlyphs();
    g_twoSide = 0;
    g_shaderDualPeel.setUniform("isTwoSide", (float*)&g_twoSide, 1);
    meshRender->RenderSurface();
  }
  g_twoSide = 0;
  g_shaderDualPeel.setUniform("isTwoSide", (float*)&g_twoSide, 1);
  g_opacity = 1;
  g_shaderDualPeel.setUniform("Alpha", (float*)&g_opacity, 1);
  
 if(glyphinfo.glyph)
 {
  if(contourinfo.showcontour)
  {
   if(glyphinfo.type == linearencode){linear_contour->RenderVBO();}
   else
   {
      if(glyphinfo.color == 0) lc_contour->RenderVBO();
      else lt_contour->RenderVBO();
   }
  }
  else
  {
   if(glyphinfo.type == linearencode){linear->RenderVBO();}
   else 
   {
     if(glyphinfo.color == 0) lc->RenderVBO();  
     else lt->RenderVBO();
   }
  }
 }
  //meshClusterRender->RenderSurface();

  svScalar s = sliceside/2.;
  svVector3 pos1 = sliceinfo.pos;
//  if(sliceData->sliceAxis[0][0]>0)
       pos1 = pos1 - s * sliceData->sliceAxis[0]
              - s * sliceData->sliceAxis[1]; 
//  else
//       pos1 = pos1 + s * sliceData->sliceAxis[0]
//              + s * sliceData->sliceAxis[1];
  if(sliceinfo.showslice) slice->RenderSlice(sliceside, 
                     pos1,
                    sliceinfo.dir, sliceData->sliceAxis[0], 
                    sliceData->sliceAxis[1]);
  pos1 = syminfo.pos;
  if(syminfo.x[0] < 0)
  pos1 = pos1 + s * syminfo.x
              + s * syminfo.y;
  else
  pos1 = pos1 - s * syminfo.x
              - s * syminfo.y;
  if(syminfo.showsymmetry) 
      slice->RenderSlice(sliceside, 
                        pos1,
                        syminfo.dir,
                        syminfo.x,
                        syminfo.y);
  glDisable(GL_LIGHTING);
  outline->DrawXYZ(rawData->lbbox, rawData->rbbox, viewproperty);
  outline->DrawAxis(rawData->lbbox, rawData->rbbox);
  outline->DrawAxisSticks(rawData->lbbox, rawData->rbbox, viewproperty);
  glPopMatrix();

  outline->DrawXYZFont(rawData->lbbox, rawData->rbbox, viewproperty);
  outline->DrawAxisFont(rawData->lbbox, rawData->rbbox, viewproperty);
}
void RenderDualPeeling()
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	// ---------------------------------------------------------------------
//	cerr<<" 1. Initialize Min-Max Depth Buffer "<<endl;
	// ---------------------------------------------------------------------
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingSingleFboId);

	// Render targets 1 and 2 store the front and back colors
	// Clear to 0.0 and use MAX blending to filter written color
	// At most one front color and one back color can be written every pass
	glDrawBuffers(2, &g_drawBuffers[1]);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Render target 0 stores (-minDepth, maxDepth, alphaMultiplier)
	glDrawBuffer(g_drawBuffers[0]);
	glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendEquationEXT(GL_MAX_EXT);

	g_shaderDualInit.bind();
  Render3D();
  g_shaderDualInit.unbind();

	CHECK_GL_ERRORS;

	// ---------------------------------------------------------------------
//	cerr<<" 2. Dual Depth Peeling + Blending"<<endl;
	// ---------------------------------------------------------------------

	// Since we cannot blend the back colors in the geometry passes,
	// we use another render target to do the alpha blending
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualBackBlenderFboId);
	glDrawBuffer(g_drawBuffers[6]);
	glClearColor(g_backgroundColor[0], g_backgroundColor[1], g_backgroundColor[2], 0);
	glClear(GL_COLOR_BUFFER_BIT);

	int currId = 0;

	for (int pass = 1; g_useOQ || pass < g_numPasses; pass++) {
		currId = pass % 2;
		int prevId = 1 - currId;
		int bufId = currId * 3;

		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingFrboId[currId]);

		glDrawBuffers(2, &g_drawBuffers[bufId+1]);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawBuffer(g_drawBuffers[bufId+0]);
		glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render target 0: RG32F MAX blending
		// Render target 1: RGBA MAX blending
		// Render target 2: RGBA MAX blending
		glDrawBuffers(3, &g_drawBuffers[bufId+0]);
		glBlendEquationEXT(GL_MAX_EXT);

		g_shaderDualPeel.bind();
		g_shaderDualPeel.bindTextureRECT("DepthBlenderTex", g_dualDepthTexId[prevId], 0);
		g_shaderDualPeel.bindTextureRECT("FrontBlenderTex", g_dualFrontBlenderTexId[prevId], 1);
                Render3D();
		g_shaderDualPeel.unbind();

		CHECK_GL_ERRORS;

		// Full screen pass to alpha-blend the back color
		glDrawBuffer(g_drawBuffers[6]);

		glBlendEquationEXT(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (g_useOQ) {
			glBeginQuery(GL_SAMPLES_PASSED_ARB, g_queryId);
		}

		g_shaderDualBlend.bind();
		g_shaderDualBlend.bindTextureRECT("TempTex", g_dualBackTempTexId[currId], 0);
		glCallList(g_quadDisplayList);
		g_shaderDualBlend.unbind();

		CHECK_GL_ERRORS;

		if (g_useOQ) {
			glEndQuery(GL_SAMPLES_PASSED_ARB);
			GLuint sample_count;
			glGetQueryObjectuiv(g_queryId, GL_QUERY_RESULT_ARB, &sample_count);
			if (sample_count == 0) {
				break;
			}
		}
	}
	glDisable(GL_BLEND);

	// ---------------------------------------------------------------------
	// 3. Final Pass
	// ---------------------------------------------------------------------

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);

	g_shaderDualFinal.bind();
	g_shaderDualFinal.bindTextureRECT("DepthBlenderTex", g_dualDepthTexId[currId], 0);
	g_shaderDualFinal.bindTextureRECT("FrontBlenderTex", g_dualFrontBlenderTexId[currId], 1);
	g_shaderDualFinal.bindTextureRECT("BackBlenderTex", g_dualBackBlenderTexId, 2);
	glCallList(g_quadDisplayList);
	g_shaderDualFinal.unbind();

	CHECK_GL_ERRORS;


}

//--------------------------------------------------------------------------
void display()
{
  glClearColor(0.5,0.5,0.5,1);
  g_numGeoPasses = 0;
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(viewproperty.eye[0], 
            viewproperty.eye[1], 
            viewproperty.eye[2], 
            0,0,0,0,1,0);
  trackball.getMatrix().getValue(viewproperty.tm);
  RenderDualPeeling();

//=======================2D=========================//
   glDisable(GL_LIGHTING);
   glGetDoublev (GL_MODELVIEW_MATRIX, viewproperty.mvmatrix);
   glGetDoublev (GL_PROJECTION_MATRIX, viewproperty.projmatrix);
   glGetIntegerv( GL_VIEWPORT, viewproperty.viewport );

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,(GLfloat) viewproperty.viewport[2], 0.0, (GLfloat) viewproperty.viewport[3]);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glEnable(GL_BLEND);

   //cout<<guiinfo.guitype<<endl;
   switch (guiinfo.guitype)
   {
       case (gui_widget): widget->Render(myColor);break;
       case (gui_slice):  mySlice->Render2D();break;
       case (gui_slider): slider->Render();break;
       case (gui_button): button->Render();break;
   }

   if(imageinfo.imageType == magnitude_image
   ||glyphinfo.color == c_magnitude)
   {
      glPushMatrix();
      glTranslatef(g_imageWidth-100,50,0);
      glScalef(10,20,1);
      myColor->RenderDivergingLegend(sliceData);
      glPopMatrix();
   }

   glMatrixMode(GL_PROJECTION);
   glLoadMatrixd(viewproperty.projmatrix);
   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixd(viewproperty.mvmatrix);

   glutSwapBuffers();
}

//--------------------------------------------------------------------------
void reshape(int w, int h)
{
	if (g_imageWidth != w || g_imageHeight != h)
	{
		g_imageWidth = w;
		g_imageHeight = h;

		DeleteDualPeelingRenderTargets();
		InitDualPeelingRenderTargets();

	}
  guiinfo.left = g_imageWidth/4.;

  button->Reshape(guiinfo.left,g_imageHeight);
  widget->Reshape(guiinfo.left,h-50,5,10, w, h);
  mySlice->Reshape(guiinfo.left,g_imageHeight,g_imageWidth - guiinfo.left);//*5./6.);
  slider->Reshape(guiinfo.left,g_imageHeight, 1920);//g_imageWidth); 

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  float aspect =  float(w)/float(h);
  float GLfloat_fov=60;
   gluPerspective(GLfloat_fov,
                       aspect,
                       0.1,//   view_info.hither,
                       1000);//  view_info.yon);

  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void mouse(int button, int state, int x, int y){
        long s=0x00000000;
    s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
    s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
    s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;


    int key_state = glutGetModifiers();
    s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
//  s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
    s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

    mouseHandle->mouse(button, state, x, g_imageHeight-y);

    if(!mouseHandle->success)
    {
        if (s & Trackball::BUTTON_DOWN){
          trackball.mouseDown(s, x, y);
          g_numPasses=2;
          glEnable(GL_CULL_FACE);
        }

        if (s & Trackball::BUTTON_UP){
          trackball.mouseUp(s, x, y);
          g_numPasses=4;
      //    if(enableinfo.enableMesh)meshRender->GenerateGlyph(viewproperty);
          glDisable(GL_CULL_FACE);
        }
    }
    if(state == GLUT_UP)
    {
/*
       if(myState->updateVisible)
       {
           sliceData->UpdateVisible();
           contourData->UpdateVisible();
           UpdateVisible();
           myState->updateVisible = false;
       }
*/
        myState->PrintROI();
    }
    glutPostRedisplay();
}
void idle()
{
        if (glutGetWindow() != window)
            glutSetWindow(window);

        glutPostRedisplay();
}
void motion(int x, int y){
   mouseHandle->motion(x,g_imageHeight-y);

   trackball.mouseMotion(x, y);

   glutPostRedisplay();
}
void movement(int x, int y)
{
   mouseHandle->movement(x,g_imageHeight-y);
   glutPostRedisplay();
}
void key(unsigned char key, int x, int y){
        switch (key) {
        case '\033':
        case 'q':
        case 'Q':
                exit(0);
                break;
        case 'h':
        case 'H':
               trackball.reset();
               break;
        case 'm':
              enableinfo.enableMesh = 1-enableinfo.enableMesh;
              break;
        case 'a':
	      mouseGroup->SetImageMouseType(one_layer);break;
        case 'c':
              cout<<"input testcluster"<<endl;
              cin>>testcluster;
              //meshClusterRender->GenerateSurface(myColor, testcluster);
              break;
        case 's':{
                      cout<<"input frequency"<<endl;
                      cin>>glyphinfo.sample;
                      myState->NewVisible(rawData);
                      if(enableinfo.enableClusterBoundary)
                        clusterData->GenerateSampling(glyphinfo.sample);
                      else
                        sliceData->GenerateSampling(glyphinfo.sample);
                      myState->UpdateSplitVisible(glyphinfo.zmin, glyphinfo.zmax);
                      sliceData->UpdateVisibleBySplit();
                      myState->UpdateVisible(rawData);
                      linear->UpdateIndex();
                      linear->GenerateIndex();
                      linear->UpdateIndexVBO();
                      break;}
        case 'l':
    		{
    		  cout<<"input the min and max z layer index"<<endl;
    		  cin>>glyphinfo.zmin; cin>>glyphinfo.zmax;
         // if(enableinfo.enableClusterBoundary)
          //  clusterData->GenerateSampling(glyphinfo.sample);
         // else
            sliceData->GenerateSampling(glyphinfo.sample);
          myState->UpdateSplitVisible(glyphinfo.zmin, glyphinfo.zmax);
          sliceData->UpdateVisibleBySplit();
                      //clusterData->GenerateClusterSampling(sample);
                      //myState->UpdateSplitVisible(zmin, zmax);
          myState->UpdateVisible(rawData);
          lt->UpdateIndex();
          lt->GenerateIndex();
          lt->UpdateIndexVBO();
          break;
    		}
      }
      glutPostRedisplay();

    }
//--------------------------------------------------------------------------

unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch);
void SetSliceText()
{
  int precision = 2;
  sliceText.clear();
  sliceText = "position " 
              + float_to_string(sliceinfo.pos[0], precision) + " "
              + float_to_string(sliceinfo.pos[1], precision) + " "
              + float_to_string(sliceinfo.pos[2], precision) + "\n"
              + "direction " 
              + float_to_string(sliceinfo.dir[0], precision) + " "
              + float_to_string(sliceinfo.dir[1], precision) + " "
              + float_to_string(sliceinfo.dir[2], precision) + "\n"
              + "distance "
              + float_to_string(sliceinfo.distance, precision);
}
void SetSymmetryText()
{
  int precision = 2;
  symmetryText.clear();
  symmetryText = "position "
              + float_to_string(syminfo.pos[0], precision) + " "
              + float_to_string(syminfo.pos[1], precision) + " "
              + float_to_string(syminfo.pos[2], precision) + "\n"
              + "x "
              + float_to_string(syminfo.x[0], precision) + " "
              + float_to_string(syminfo.x[1], precision) + " "
              + float_to_string(syminfo.x[2], precision) + "\n"
              + "direction "
              + float_to_string(syminfo.dir[0], precision) + " "
              + float_to_string(syminfo.dir[1], precision) + " "
              + float_to_string(syminfo.dir[2], precision);
}
void SetContourText()
{
  contourText.clear();
  int precision = 2;
  string tmp = (contourinfo.type==1?"true\n":"false\n");
  string tmp2 = contourinfo.type==1?float_to_string(contourinfo.value, precision):e_to_string(contourinfo.value,precision);
  contourText = "ratio " + tmp
              + "value " + tmp2; 
}
void SetClusterText()
{
  clusterText1.clear();
  clusterText1 = "num " + int_to_string(clusterinfo.num) + "\n"
               + "weight(pos,dir,mag) "//(position,direction,magntiude) "
               + float_to_string(clusterinfo.weight[0],2) + " "
               + float_to_string(clusterinfo.weight[3],2) + " "
               + float_to_string(clusterinfo.weight[6],2);
  clusterText2 = "num " + int_to_string(clusterinfo.roinum) + "\n"
               + "weight(pos,dir,mag) "//(position,direction,magntiude) "
               + float_to_string(clusterinfo.roiweight[0],2) + " "
               + float_to_string(clusterinfo.roiweight[3],2) + " "
               + float_to_string(clusterinfo.roiweight[6],2);
}
void GetSliceText() 
{
   sliceText.clear();
   sliceText = string(glui_slice_box -> get_text());
   vector<string> strs;
   int size = split(sliceText, strs,  '\n');
   vector<string> strs2;
   split(strs[0], strs2, ' ' );
   sliceinfo.pos[0] = atof(strs2[1].c_str());
   sliceinfo.pos[1] = atof(strs2[2].c_str());
   sliceinfo.pos[2] = atof(strs2[3].c_str());
   strs2.clear();
   split(strs[1], strs2, ' ' );
   sliceinfo.dir[0] = atof(strs2[1].c_str());
   sliceinfo.dir[1] = atof(strs2[2].c_str());
   sliceinfo.dir[2] = atof(strs2[3].c_str());
   strs2.clear();
   split(strs[2], strs2, ' ' );
   sliceinfo.distance = atof(strs2[1].c_str());
   cout<<sliceinfo.pos[0]<<" "<<sliceinfo.pos[1]<<" "<<sliceinfo.pos[2]<<endl;
   cout<<sliceinfo.dir[0]<<" "<<sliceinfo.dir[1]<<" "<<sliceinfo.dir[2]<<endl;
   cout<<sliceinfo.distance<<endl;
}
void GetContourText()
{
   contourText.clear();
   contourText = string(glui_contour_box -> get_text());
   vector<string> strs;
   int size = split(contourText, strs,  '\n');
   vector<string> strs2;
   split(strs[0], strs2, ' ' );
   cout<<strs2[1].c_str()<<endl;
   contourinfo.type = strcmp((char*)strs2[1].c_str(), "true\n")==0?1:0;
   strs2.clear();
   split(strs[1], strs2, ' ' );
   contourinfo.value = atof(strs2[1].c_str());
   cout<<contourinfo.type<<" "<<contourinfo.value<<endl;
}
void GetSymmetryText()
{
   symmetryText.clear();
   symmetryText = string(glui_symmetry_box -> get_text());
   vector<string> strs;
   int size = split(symmetryText, strs,  '\n');
   vector<string> strs2;
   split(strs[0], strs2, ' ' );
   syminfo.pos[0] = atof(strs2[1].c_str());
   syminfo.pos[1] = atof(strs2[2].c_str());
   syminfo.pos[2] = atof(strs2[3].c_str());
   strs2.clear();
   split(strs[1], strs2, ' ' );
   syminfo.x[0] = atof(strs2[1].c_str());
   syminfo.x[1] = atof(strs2[2].c_str());
   syminfo.x[2] = atof(strs2[3].c_str());
   strs2.clear();
   split(strs[2], strs2, ' ' );
   syminfo.dir[0] = atof(strs2[1].c_str());
   syminfo.dir[1] = atof(strs2[2].c_str());
   syminfo.dir[2] = atof(strs2[3].c_str());
   strs2.clear();
   cout<<syminfo.pos[0]<<" "<<syminfo.pos[1]<<" "<<syminfo.pos[2]<<endl;
   cout<<syminfo.x[0]<<" "<<syminfo.x[1]<<" "<<syminfo.x[2]<<endl;
   cout<<syminfo.dir[0]<<" "<<syminfo.dir[1]<<" "<<syminfo.dir[2]<<endl;
}
void GetClusterText()
{
    clusterText1.clear();
    clusterText1 = string(glui_cluster_box1->get_text());
    vector<string> strs;
    int size = split(clusterText1, strs, '\n');
    vector<string> strs2;
    split(strs[0], strs2, ' ');
    clusterinfo.num = atoi(strs2[1].c_str());
    strs2.clear();
    split(strs[1], strs2, ' ');
    clusterinfo.weight[0]=atof(strs2[1].c_str());
    clusterinfo.weight[1]=atof(strs2[1].c_str());
    clusterinfo.weight[2]=atof(strs2[1].c_str());
    clusterinfo.weight[3]=atof(strs2[2].c_str());
    clusterinfo.weight[4]=atof(strs2[2].c_str());
    clusterinfo.weight[5]=atof(strs2[2].c_str());
    clusterinfo.weight[6]=atof(strs2[3].c_str());
    cout<<clusterinfo.num<<endl;
    cout<<clusterinfo.weight[5]<<endl;

    clusterText2.clear();
    clusterText2 = string(glui_cluster_box2->get_text());
    strs.clear();
    size = split(clusterText2, strs, '\n');
    if(size == 0){clusterinfo.roicluster = 0;return;} 
    strs2.clear();
    split(strs[0], strs2, ' ');
    clusterinfo.roinum = atoi(strs2[1].c_str());
    if(clusterinfo.roinum == 0) {clusterinfo.roicluster = 0; return;} 
    strs2.clear();
    split(strs[1], strs2, ' ');
    clusterinfo.roiweight[0]=atof(strs2[1].c_str());
    clusterinfo.roiweight[1]=atof(strs2[1].c_str());
    clusterinfo.roiweight[2]=atof(strs2[1].c_str());
    clusterinfo.roiweight[3]=atof(strs2[2].c_str());
    clusterinfo.roiweight[4]=atof(strs2[2].c_str());
    clusterinfo.roiweight[5]=atof(strs2[2].c_str());
    clusterinfo.roiweight[6]=atof(strs2[3].c_str());
}

unsigned int split(const std::string &txt, std::vector<std::string> &strs, char ch)

{
    int pos = txt.find( ch );
    int size = txt.size(); 
   int initialPos = 0;
    strs.clear();
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos + 1 ) );
        initialPos = pos + 1;
        pos = txt.find( ch, initialPos );
    }
    strs.push_back( txt.substr( initialPos, min( pos, size ) - initialPos + 1 ) );
    return strs.size();
}

//--------------------------------------------------------------------------

void glui_vis_display()
{
  GLUI_Panel *panel_vis = glui->add_panel("Display");
  panel_vis->set_alignment(GLUI_ALIGN_LEFT);
  //ROI
  GLUI_Panel *panel_roi = glui->add_panel_to_panel(panel_vis,"GUI");
  GLUI_Panel *panel_image = glui->add_panel_to_panel(panel_roi, "2D image");
  panel_image->set_alignment(GLUI_ALIGN_LEFT);
  glui_show_image = glui->add_checkbox_to_panel(panel_image, "Show 2D image", &guiinfo.slice, GLUI_SHOW_IMAGE_ID, control_cb);
  glui_image_type = glui->add_radiogroup_to_panel(panel_image, &imageinfo.imageType, GLUI_IMAGE_TYPE_ID, control_cb);
  glui->add_radiobutton_to_group(glui_image_type,"Magnitude");
  glui->add_radiobutton_to_group(glui_image_type,"Cluster");
  glui_image_percentage_t = glui->add_edittext_to_panel(panel_image,"Percentage",GLUI_EDITTEXT_FLOAT,&imageinfo.percentage, GLUI_IMAGE_PERCENTAGE_T_ID, control_cb);
  glui->add_column_to_panel(panel_image,true);
  new GLUI_StaticText(panel_image,"Interaction");
  glui_image_interaction = glui->add_radiogroup_to_panel(panel_image, &imageinfo.imageMouseType, GLUI_IMAGE_INTERACTION_ID, control_cb);
  glui->add_radiobutton_to_group(glui_image_interaction,"Rearrange");
  glui->add_radiobutton_to_group(glui_image_interaction,"Add to 3D");
//  glui->add_radiobutton_to_group(glui_image_interaction,"Select a region");
  //widget
  GLUI_Panel *panel_widget = glui->add_panel_to_panel(panel_roi, "Slice widget");
  panel_widget->set_alignment(GLUI_ALIGN_LEFT);
  glui_show_widget = glui->add_checkbox_to_panel(panel_widget, "Show widgets", &guiinfo.widget, GLUI_SHOW_WIDGET_ID, control_cb);
  glui->add_column_to_panel(panel_widget,true);
  glui_widget_repeat = glui->add_checkbox_to_panel(panel_widget,"Repeat", &guiinfo.widgetrepeat, GLUI_WIDGET_REPEAT_ID, control_cb);
 //
  glui_show_slider = glui->add_checkbox_to_panel(panel_roi, "Show slider", &guiinfo.slider, GLUI_SHOW_SLIDER_ID, control_cb);
  glui_show_button = glui->add_checkbox_to_panel(panel_roi, "Show button", &guiinfo.button, GLUI_SHOW_BUTTON_ID, control_cb);
  //
  GLUI_Panel *panel_glyph = glui->add_panel_to_panel(panel_vis, "Glyph");
  panel_glyph->set_alignment(GLUI_ALIGN_LEFT);
  glui_show_glyph = glui->add_checkbox_to_panel(panel_glyph, "Show glyph", &glyphinfo.glyph, GLUI_SHOW_GLYPH_ID, control_cb);
  GLUI_Panel *panel_glyph_type = glui->add_panel_to_panel(panel_glyph,"Type");
  glui_glyph_type = glui->add_radiogroup_to_panel(panel_glyph_type, &glyphinfo.type, GLUI_GLYPH_TYPE_ID, control_cb);
  glui->add_radiobutton_to_group(glui_glyph_type,"Linear");
  glui->add_radiobutton_to_group(glui_glyph_type,"SplitVectors");
  GLUI_Panel *panel_glyph_color = glui->add_panel_to_panel(panel_glyph,"Color");
  glui_color_type = glui->add_radiogroup_to_panel(panel_glyph_color, &glyphinfo.color, GLUI_GLYPH_COLOR_ID, control_cb);
  glui->add_radiobutton_to_group(glui_color_type,"Magnitude");
  glui->add_radiobutton_to_group(glui_color_type,"Cluster");
  glui->add_radiobutton_to_group(glui_color_type,"Symmetry");
  glui->add_column_to_panel(panel_glyph,true);
  GLUI_Panel *panel_sample = glui->add_panel_to_panel(panel_glyph,"Sampling");
  panel_sample->set_alignment(GLUI_ALIGN_LEFT);
  glui_sample_box = glui->add_edittext_to_panel(panel_sample, "Sampling", 
  GLUI_EDITTEXT_INT,
  &glyphinfo.sample, GLUI_SAMPLE_ID, control_cb);
//  glui_sample = new GLUI_Scrollbar(panel_sample, "Sampling", GLUI_SCROLL_HORIZONTAL, &glyphinfo.sample, GLUI_SAMPLE_ID, control_cb);
  GLUI_Panel *panel_glyph_length = glui->add_panel_to_panel(panel_glyph,"Length");
  panel_glyph_length->set_alignment(GLUI_ALIGN_LEFT);
  glui_glyph_length = new GLUI_Scrollbar(panel_glyph_length, "Length", GLUI_SCROLL_HORIZONTAL, &glyphinfo.length, GLUI_GLYPH_LENGTH_ID, control_cb);
  GLUI_Panel *panel_glyph_width = glui->add_panel_to_panel(panel_glyph,"Radius");
  panel_glyph_width->set_alignment(GLUI_ALIGN_LEFT);
  glui_glyph_width = new GLUI_Scrollbar(panel_glyph_width, "Radius", GLUI_SCROLL_HORIZONTAL, &glyphinfo.arrowradius, GLUI_GLYPH_WIDTH_ID, control_cb);

  //
  glui_show_qdot = glui->add_checkbox_to_panel(panel_vis, "Show QDOT surface", &enableQDOT, GLUI_SHOW_QDOT_ID, control_cb);
  glui_show_csurface = glui->add_checkbox_to_panel(panel_vis, "Show cluster surface", &enableSurface, GLUI_SHOW_CSURFACE_ID, control_cb);
  //
   glui_vis_update = glui->add_button_to_panel(panel_vis, "Update", GLUI_VIS_UPDATE_ID, control_cb);
  glui_screenshot = glui->add_button_to_panel(panel_vis, "Screenshot", GLUI_SCREENSHOT_ID, control_cb);
}

void control_cb(int control)
{
  glutSetWindow(window);

  switch(control)
  {
     case GLUI_SLICE_TEXT_ID: myState->updateSlice=true;break;
     case GLUI_SHOW_CONTOUR_ID:
     case GLUI_CONTOUR_TEXT_ID: myState->updateContour=true;break;
     case GLUI_SYM_TEXT_ID: myState->updateSymmetry=true;break;
     case  GLUI_ROI_CLUSTER_ID: 
     case  GLUI_CLUSTER_TEXT_ID0:
     case  GLUI_CLUSTER_TEXT_ID1:
     case  GLUI_CLUSTER_SPLIT_ID: myState->updateCluster=true;break; 
     case   GLUI_SHOW_IMAGE_ID:
            {
              if(guiinfo.slice == 0)
                  guiinfo.guitype = gui_none;
              else{
                guiinfo.guitype = gui_slice;
		guiinfo.slice = 1;
		guiinfo.slider=0;
		guiinfo.button=0;
		guiinfo.widget=0;
                glui_show_button->set_int_val(0);
                glui_show_slider->set_int_val(0);
                glui_show_widget->set_int_val(0); 
              }
       //         glui_show_button->set_int_val(0);
		 switch (guiinfo.guitype)
		  {
       case (gui_widget): mouseGroup->currentMouse=mouse_widget; break;
       case (gui_slider): mouseGroup->currentMouse=mouse_slider; break;
       case (gui_slice): mouseGroup->currentMouse=mouse_image;break;
       case (gui_button): mouseGroup->currentMouse=mouse_button;break;
       case (gui_none): mouseGroup->currentMouse=-1;break;
 		 }

		break;
            }
     case GLUI_IMAGE_INTERACTION_ID:
	{
		mouseGroup->SetImageMouseType(imageinfo.imageMouseType);
		break;
	}
     case GLUI_IMAGE_TYPE_ID:
	{
                updateImage = true;
		break;
	}
     case GLUI_IMAGE_PERCENTAGE_T_ID:
	{
                updateImage = true;
                //mySlice->SetImageType(imageinfo.imageType);//cluster_image);
                //mySlice->SetColors(myColor);
		//mySlice->GenerateGroupingByColor(imageinfo.percentage);	
		break;
	} 
     case GLUI_SHOW_WIDGET_ID:
            {
              if(guiinfo.widget == 0)
                   guiinfo.guitype = gui_none;
              else{
                guiinfo.guitype = gui_widget;
                guiinfo.slice = 0;
                guiinfo.slider=0;
                guiinfo.button=0;
                guiinfo.widget=1;
                glui_show_slider->set_int_val(0);
                glui_show_image->set_int_val(0);
                glui_show_button->set_int_val(0); 
              }
        //        glui_show_button->set_int_val(0);
                 switch (guiinfo.guitype)
                  {
       case (gui_widget): mouseGroup->currentMouse=mouse_widget; break;
       case (gui_slider): mouseGroup->currentMouse=mouse_slider; break;
       case (gui_slice): mouseGroup->currentMouse=mouse_image;break;
       case (gui_button): mouseGroup->currentMouse=mouse_button;break;
       case (gui_none): mouseGroup->currentMouse=-1;break;
                 }

                break;
            }
     case GLUI_WIDGET_REPEAT_ID:
	{
		widget->Repeat(guiinfo.widgetrepeat);			
		break;
	}
     case GLUI_GLYPH_WIDTH_ID:
     case GLUI_GLYPH_LENGTH_ID:
        {
           updateGlyph = true;
           break;
        }
     case GLUI_APPLY_ID:
        {
            GetSliceText();
            GetSymmetryText();
            GetContourText();
            GetClusterText();
            break;
        } 
     case GLUI_VIS_UPDATE_ID:
        { 
          if(updateImage) UpdateImage();
          updateImage = false;
          if(updateGlyph) UpdateGlyphs();
          updateGlyph = false;
          if(updateColor) UpdateColor();
          updateColor = false;
          if(myState->updateVisible) 
          {
               sliceData->GenerateSampling(glyphinfo.sample);
               UpdateVisible();
          }
          myState->updateVisible = false; break;
        }
     case GLUI_UPDATE_ID: UpdateControl();break;
     case GLUI_SHOW_BUTTON_ID:
     {
             if(guiinfo.button == 0)
                   guiinfo.guitype = gui_none;
             else
             {
                guiinfo.guitype = gui_button;
                guiinfo.slice = 0;
                guiinfo.slider=0;
                guiinfo.button=1;
                guiinfo.widget=0;
                glui_show_slider->set_int_val(0);
                glui_show_widget->set_int_val(0);
                glui_show_image->set_int_val(0);
             }
                 switch (guiinfo.guitype)
                  {
       case (gui_widget): mouseGroup->currentMouse=mouse_widget; break;
       case (gui_slider): mouseGroup->currentMouse=mouse_slider; break;
       case (gui_slice): mouseGroup->currentMouse=mouse_image;break;
       case (gui_button): mouseGroup->currentMouse=mouse_button;break;
       case (gui_none): mouseGroup->currentMouse=-1;break;
                 }

                break;

     }
     case GLUI_SHOW_SLIDER_ID:
	{
            if(guiinfo.slider == 0)
                 guiinfo.guitype = gui_none;
            else{
                guiinfo.guitype = gui_slider;
                guiinfo.slice = 0;
                guiinfo.slider=1;
                guiinfo.button=0;
                guiinfo.widget=0;
                glui_show_button->set_int_val(0);
                glui_show_widget->set_int_val(0);
                glui_show_image->set_int_val(0);
             }
          //      glui_show_button->set_int_val(0);
                 switch (guiinfo.guitype)
                  {
       case (gui_widget): mouseGroup->currentMouse=mouse_widget; break;
       case (gui_slider): mouseGroup->currentMouse=mouse_slider; break;
       case (gui_slice): mouseGroup->currentMouse=mouse_image;break;
       case (gui_button): mouseGroup->currentMouse=mouse_button;break;
       case (gui_none): mouseGroup->currentMouse=-1;break;
                 }

                break;
	}
     case GLUI_SHOW_GLYPH_ID: break;
     case GLUI_GLYPH_TYPE_ID: break;
     case GLUI_GLYPH_COLOR_ID: updateColor = true;
        //UpdateColor();
/*
	{
		if(glyphinfo.color == 0)//magnitude
		{
			//imageinfo.imageType= magnitude_image;
		        //sliceData->SetImageLabel(imageinfo.imageType);	
			myColor->SetColorType(c_magnitude);
			UpdateRender();
		}	
		else
		{
			//imageinfo.imageType= cluster_image;
                        //sliceData->SetImageLabel(imageinfo.imageType);
                        myColor->SetColorType(c_cluster);
                        UpdateRender();
		}
*/		break;

	
     case GLUI_SHOW_QDOT_ID: break;
     case  GLUI_SHOW_CSURFACE_ID:break;
     case  GLUI_SAMPLE_ID:
           myState->updateVisible = true;break;
     case  GLUI_SCREENSHOT_ID:ScreenShot();break;

  }
  glutPostRedisplay();
}

void glui_display()
{
  int w= 250;
  int h= 60;
//  sliceText = "position 0.00 0.00 0.00\n"
//              "direction 0.00 0.00 1.00\n"
//              "distance 0.25\n";
  SetSliceText();
  glui = GLUI_Master.create_glui_subwindow(window, GLUI_SUBWINDOW_LEFT);
  GLUI_Panel *panel_compute = glui->add_panel("Computation");
  panel_compute->set_alignment(GLUI_ALIGN_LEFT);
  //data panel/
//  GLUI_Panel *panel_data = glui->add_panel_to_panel(panel_compute,"Data");
//  panel_data->set_alignment(GLUI_ALIGN_LEFT);
//  glui_data = glui->add_statictext_to_panel(panel_data, datainfo.name.c_str());
  //slice panel
  GLUI_Panel *panel_slice = glui->add_panel_to_panel(panel_compute,"Slice"); 
  panel_slice->set_alignment(GLUI_ALIGN_LEFT);
  panel_slice->set_w(w);
  glui_show_slice = glui->add_checkbox_to_panel(panel_slice,"Show slice", &sliceinfo.showslice, GLUI_SHOW_SLICE_ID, control_cb);
  glui_slice_box = new GLUI_TextBox(panel_slice, true,GLUI_SLICE_TEXT_ID,control_cb);
  glui_slice_box->set_text(sliceText.c_str());
  glui_slice_box->set_w(w);
  glui_slice_box->set_h(h);
  glui_slice_box->enable();
//  GLUI_Panel *panel_slice_pos = glui->add_panel_to_panel(panel_slice,"Position");
//  glui_slice_pos[0]=glui->add_edittext_to_panel(panel_slice_pos,"x",GLUI_EDITTEXT_FLOAT, &sliceinfo.pos[0], GLUI_SLICE_POS_ID0, control_cb);
//  glui->add_column_to_panel(panel_slice_pos,true);
//  glui_slice_pos[1]=glui->add_edittext_to_panel(panel_slice_pos,"y",GLUI_EDITTEXT_FLOAT, &sliceinfo.pos[1], GLUI_SLICE_POS_ID1, control_cb);
//  glui->add_column_to_panel(panel_slice_pos,true);
//  glui_slice_pos[2]=glui->add_edittext_to_panel(panel_slice_pos,"",GLUI_EDITTEXT_FLOAT,&sliceinfo.pos[2], GLUI_SLICE_POS_ID2, control_cb);
//  for(int i=0;i<3;i++) glui_slice_pos[i]->set_w(w);
//  GLUI_Panel *panel_slice_dir = glui->add_panel_to_panel(panel_slice,"Direction");
//  glui_slice_dir[0]=glui->add_edittext_to_panel(panel_slice_dir,"",GLUI_EDITTEXT_FLOAT,&sliceinfo.dir[0], GLUI_SLICE_DIR_ID0, control_cb);
//  glui->add_column_to_panel(panel_slice_dir,true);
 // glui_slice_dir[1]=glui->add_edittext_to_panel(panel_slice_dir,"",GLUI_EDITTEXT_FLOAT,&sliceinfo.dir[1], GLUI_SLICE_DIR_ID1, control_cb);
//  glui->add_column_to_panel(panel_slice_dir,true);
//  glui_slice_dir[2]=glui->add_edittext_to_panel(panel_slice_dir,"",GLUI_EDITTEXT_FLOAT,&sliceinfo.dir[2], GLUI_SLICE_DIR_ID2, control_cb);
//  for(int i=0;i<3;i++) glui_slice_dir[i]->set_w(w);
//  glui_slice_d = glui->add_edittext_to_panel(panel_slice,"Distance ",GLUI_EDITTEXT_FLOAT,&sliceinfo.distance, GLUI_SLICE_DISTANCE_ID, control_cb);
  //contour panel
//  contourText = "ratio true\n"
//                "value 0.5";
  SetContourText();
  GLUI_Panel *panel_contour = glui->add_panel_to_panel(panel_compute,"Contour");
  panel_contour->set_alignment(GLUI_ALIGN_LEFT);
  glui_show_contour = glui->add_checkbox_to_panel(panel_contour,"Show contours", &contourinfo.showcontour, GLUI_SHOW_CONTOUR_ID, control_cb);
  glui_contour_box = new GLUI_TextBox(panel_contour, true, GLUI_CONTOUR_TEXT_ID, control_cb);
  glui_contour_box->set_text(contourText.c_str());
  glui_contour_box->set_w(w);
  glui_contour_box->set_h(h*2/3);
  glui_contour_box->enable();
/*
  GLUI_Panel *panel_contour = glui->add_panel_to_panel(panel_compute,"Contour");
  panel_contour ->set_alignment(GLUI_ALIGN_LEFT);
  glui_show_contour = glui->add_checkbox_to_panel(panel_contour,"Show contours", &contourinfo.showcontour, GLUI_SHOW_CONTOUR_ID, control_cb);
  glui_contour_value = glui->add_edittext_to_panel(panel_contour,"Value ",GLUI_EDITTEXT_FLOAT, &contourinfo.value, GLUI_CONTOUR_VALUE_ID, control_cb);
  glui->add_column_to_panel(panel_contour,true); 
  glui_contour_type = glui->add_radiogroup_to_panel(panel_contour, &contourinfo.type, GLUI_CONTOUR_TYPE_ID, control_cb);
  glui->add_radiobutton_to_group(glui_contour_type,"Absolute");
  glui->add_radiobutton_to_group(glui_contour_type,"Ratio");
*/
  //symmetry panel
//  symmetryText = "position 0.00 0.00 0.00\n"
//                 "x -1.00 1.00 0.00\n"
//                 "direction 1.00 1.00 0.00";
  SetSymmetryText();
  GLUI_Panel *panel_sym = glui->add_panel_to_panel(panel_compute,"Symmetry");
  panel_sym->set_alignment(GLUI_ALIGN_LEFT);
  glui_show_symmetry = glui->add_checkbox_to_panel(panel_sym,"Show symmetry plane", &syminfo.showsymmetry, GLUI_SHOW_SYMMETRY_ID, control_cb);
  glui_symmetry_box = new GLUI_TextBox(panel_sym, true, GLUI_SYM_TEXT_ID, control_cb);
  glui_symmetry_box->set_text(symmetryText.c_str());
  glui_symmetry_box->set_w(w);
  glui_symmetry_box->set_h(h);
  glui_symmetry_box->enable();
/*
  GLUI_Panel *panel_symmetry_pos = glui->add_panel_to_panel(panel_sym,"Position");
  glui_symmetry_pos[0]=glui->add_edittext_to_panel(panel_symmetry_pos,"x",GLUI_EDITTEXT_FLOAT, &syminfo.pos[0], GLUI_SYM_POS_ID0, control_cb);
  glui->add_column_to_panel(panel_symmetry_pos,true);
  glui_symmetry_pos[1]=glui->add_edittext_to_panel(panel_symmetry_pos,"y",GLUI_EDITTEXT_FLOAT, &syminfo.pos[1], GLUI_SYM_POS_ID1, control_cb);
  glui->add_column_to_panel(panel_symmetry_pos,true);
  glui_symmetry_pos[2]=glui->add_edittext_to_panel(panel_symmetry_pos,"z",GLUI_EDITTEXT_FLOAT, &syminfo.pos[2], GLUI_SYM_POS_ID2, control_cb);
  for(int i=0;i<3;i++) glui_symmetry_pos[i]->set_w(w);
  GLUI_Panel *panel_symmetry_dir = glui->add_panel_to_panel(panel_sym,"Direction");
  glui_symmetry_dir[0]=glui->add_edittext_to_panel(panel_symmetry_dir,"",GLUI_EDITTEXT_FLOAT,&syminfo.dir[0], GLUI_SLICE_DIR_ID0, control_cb);
  glui->add_column_to_panel(panel_symmetry_dir,true);
  glui_symmetry_dir[1]=glui->add_edittext_to_panel(panel_symmetry_dir,"",GLUI_EDITTEXT_FLOAT,&syminfo.dir[1], GLUI_SLICE_DIR_ID1, control_cb);
  glui->add_column_to_panel(panel_symmetry_dir,true);
  glui_symmetry_dir[2]=glui->add_edittext_to_panel(panel_symmetry_dir,"",GLUI_EDITTEXT_FLOAT,&syminfo.dir[2], GLUI_SLICE_DIR_ID2, control_cb);
  for(int i=0;i<3;i++) glui_symmetry_dir[i]->set_w(w);
*/
  //cluster panel
//  clusterText1 = "num 40\n"
//                 "weights(position,direction,magnitude) 1 1 1";
//  clusterText2 = clusterText1;
  SetClusterText();
  GLUI_Panel *panel_cluster= glui->add_panel_to_panel(panel_compute,"Cluster");
  panel_cluster->set_alignment(GLUI_ALIGN_LEFT);
  glui_cluster_split = new GLUI_Checkbox(panel_cluster,"By each slice",&clusterinfo.split, GLUI_CLUSTER_SPLIT_ID, control_cb);
  glui_cluster_box1 = new GLUI_TextBox(panel_cluster, true, GLUI_CLUSTER_TEXT_ID0, control_cb);
  glui_cluster_box1->set_text(clusterText1.c_str());
  glui_cluster_box1->set_w(w);
  glui_cluster_box1->set_h(h*2/3);
  glui_cluster_box1->enable();
  GLUI_Panel *panel_roi_cluster=glui->add_panel_to_panel(panel_cluster,"Cluster in ROI");
  glui_roi_cluster = glui->add_checkbox_to_panel(panel_roi_cluster,"Cluster data in ROI", &clusterinfo.roicluster, GLUI_ROI_CLUSTER_ID, control_cb); 
 glui_cluster_box2 = new GLUI_TextBox(panel_roi_cluster, true, GLUI_CLUSTER_TEXT_ID1, control_cb);
  glui_cluster_box2->set_text(clusterText2.c_str());
  glui_cluster_box2->set_w(w);
  glui_cluster_box2->set_h(h*2/3);
  glui_cluster_box2->enable();
/*
  glui_cluster_num = glui->add_edittext_to_panel(panel_cluster, "Number of clusters ", GLUI_EDITTEXT_INT, &clusterinfo.num, GLUI_CLUSTER_NUM_ID, control_cb);
  GLUI_Panel *panel_cluster_weight=glui->add_panel_to_panel(panel_cluster,"Weight");
  for(int i=0;i<3;i++)
  {
     int id;
     switch (i){
       case 0: id = GLUI_CLUSTER_WEIGHT_ID0;break;
       case 1: id = GLUI_CLUSTER_WEIGHT_ID1;break;
       case 2: id = GLUI_CLUSTER_WEIGHT_ID2;break;
       case 3: id = GLUI_CLUSTER_WEIGHT_ID3;break;
       case 4: id = GLUI_CLUSTER_WEIGHT_ID4;break;
       case 5: id = GLUI_CLUSTER_WEIGHT_ID5;break;
       case 6: id = GLUI_CLUSTER_WEIGHT_ID6;break;
     }
     glui_cluster_weight[i]=glui->add_edittext_to_panel(panel_cluster_weight,"",GLUI_EDITTEXT_FLOAT,&clusterinfo.weight[i*3], id, control_cb);
     if(i<2) glui->add_column_to_panel(panel_cluster_weight,true);
     glui_cluster_weight[i]->set_w(w);
  }
  GLUI_Panel *panel_roi_cluster=glui->add_panel_to_panel(panel_cluster,"Cluster in ROI");
  glui_roi_cluster = glui->add_checkbox_to_panel(panel_roi_cluster,"Cluster data in ROI", &clusterinfo.roicluster, GLUI_ROI_CLUSTER_ID, control_cb);
  glui_roi_cluster_num = glui->add_edittext_to_panel(panel_roi_cluster, "Number of clusters ", GLUI_EDITTEXT_INT, &clusterinfo.roinum, GLUI_ROI_CLUSTER_NUM_ID, control_cb);
  GLUI_Panel *panel_roi_cluster_weight=glui->add_panel_to_panel(panel_roi_cluster,"Weight");
  for(int i=0;i<3;i++)
  {
     int id;
     switch (i){
       case 0: id = GLUI_ROI_CLUSTER_WEIGHT_ID0;break;
       case 1: id = GLUI_ROI_CLUSTER_WEIGHT_ID1;break;
       case 2: id = GLUI_ROI_CLUSTER_WEIGHT_ID2;break;
     }
     glui_roi_cluster_weight[i]=glui->add_edittext_to_panel(panel_roi_cluster_weight,"",GLUI_EDITTEXT_FLOAT,&clusterinfo.roiweight[i*3], id, control_cb);
     if(i<2) glui->add_column_to_panel(panel_roi_cluster_weight,true);
     glui_roi_cluster_weight[i]->set_w(w);
  }
*/
  //Update
//  glui_apply = glui->add_button_to_panel(panel_compute, "Apply", GLUI_APPLY_ID, control_cb);
  glui_update = glui->add_button_to_panel(panel_compute, "Update", GLUI_UPDATE_ID, control_cb);

  glui_vis_display();
}

//--------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  //svVector3 center(0,0,0);
  //svVector3 dir(0,0,1);
  //svScalar distance = 0.25;
  readInput(strdup(argv[1]));//"SplitTool/input2");
/*
  sliceinfo.pos[0]=0;
  sliceinfo.pos[1]=0;
  sliceinfo.pos[2]=0;
  sliceinfo.dir[0]=0;
  sliceinfo.dir[1]=0;
  sliceinfo.dir[2]=1;
  sliceinfo.distance = 0.25;
*/
  viewproperty.eye[0] = 0;
  viewproperty.eye[1] = 0;
  viewproperty.eye[2] = 100;
  trackball.getMatrix().getValue(viewproperty.tm);
//  for(int i=0;i<16;i++)viewproperty.tm[i]=m[i];
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(g_imageWidth, g_imageHeight);
        glutInitWindowPosition(0,0);
	glutInit(&argc, argv);
	window=glutCreateWindow("Desktop Tool");

        glui_display();
	if (glewInit() != GLEW_OK)
	{
		printf("glewInit failed. Exiting...\n");
		exit(1);
	}

    if (!glewIsSupported( "GL_VERSION_2_0 "
                          "GL_ARB_texture_rectangle "
						  "GL_ARB_texture_float "
						  "GL_NV_float_buffer "
                          "GL_NV_depth_buffer_float "))
    {
        printf("Unable to load the necessary extensions\n");
		printf("This sample requires:\n");
		printf("OpenGL version 2.0\n");
		printf("GL_ARB_texture_rectangle\n");
		printf("GL_ARB_texture_float\n");
		printf("GL_NV_float_buffer\n");
		printf("GL_NV_depth_buffer_float\n");
		printf("Exiting...\n");
        exit(1);
    }
  
  //datainfo.dir = string(strdup(argv[2]));
  //datainfo.tmpdir = string(strdup(argv[3]));
  //datainfo.name = string(strdup(argv[4]));

  char *str = new char[400];
  sprintf(str,"%s/libs/dual_depth_peeling/shaders/",SRC_DIR); 
  InitGL(string(str));
  delete [] str;
  GLUI_Master.set_glutReshapeFunc(reshape);
  //setData(atoi(argv[1]), strdup(argv[2]), strdup(argv[3]), strdup(argv[4]),
  //       sliceinfo.pos, sliceinfo.dir, sliceinfo.distance);
  init();
  GLUI_Master.set_glutDisplayFunc(display);
  GLUI_Master.set_glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutPassiveMotionFunc(movement);
  GLUI_Master.set_glutKeyboardFunc(key);

	glutMainLoop();
	return 0;
}

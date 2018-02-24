/*  svQDOTData
 */
#include "svQDOTData.h"
#include <string.h>
using namespace std;

namespace __svl_lib {

//======================qdot data===============================//
void svQDOTData::cleandata(){
  for(int i=0;i<splitData.size();i++) splitData[i].clear();
  splitData.clear();
  //(*clusterList).clear();
  isSelectable.clear();
  isVisible.clear();
  isSelected.clear();
  isHighlighted.clear();
}
void svQDOTData::UpdateVisibleBySplit(){
  for(int i=0;i<splitData.size();i++){
   for(int j=0;j<splitData[i].size();j++){
     if(state->rP->splitVisible[i])
       state->qdotVisible.at(splitData[i][j])= true;
     else
       state->qdotVisible.at(splitData[i][j]) = false;
   }
  }
}
//==============================================================//

//======================mesh data===============================//
void svMeshData::SetData(char *dir){
  int regionsize = myQDOT->regionSize;
  facets.resize(regionsize);
  boundPoints.resize(regionsize);
  glyphs.resize(regionsize);
  for(int i=1;i<regionsize+1;i++){
     char *str = new char[400];
//mesh data
     sprintf(str, "%s/mesh%d.txt", dir, i);
     ifstream infile(str);
     int n;
     infile>>n;
     facets[i-1].resize(n/3);
     for(int j=0;j<n;j+=3){
        for(int v=0;v<3;v++){
            infile>>facets[i-1][j/3].pos[v][0]
                  >>facets[i-1][j/3].pos[v][1]
                  >>facets[i-1][j/3].pos[v][2]
                  >>facets[i-1][j/3].norm[0]
                  >>facets[i-1][j/3].norm[1]
                  >>facets[i-1][j/3].norm[2];
        }
        facets[i-1][j/3].norm = -facets[i-1][j/3].norm;
     }
     infile.close();

//cerr<<"boundary points"<<endl;
     sprintf(str, "%s/point%d.txt", dir, i);
     infile.open(str);
     infile>>n;
     for(int j=0;j<n;j++){
       Spin p;
       infile>>p.pos[0]>>p.pos[1]>>p.pos[2];
       if(p.pos[2] > myQDOT->regionZ[0][i-1]
       && p.pos[2] < myQDOT->regionZ[1][i-1]) continue;
       bool exist = false;
       for(int v=0;v<boundPoints[i-1].size();v++){
           if(fabs(boundPoints[i-1][v].pos[0] - p.pos[0])<1e-8
           && fabs(boundPoints[i-1][v].pos[1] - p.pos[1])<1e-8
           && fabs(boundPoints[i-1][v].pos[2] - p.pos[2])<1e-8){
              exist = true;break;
           }
       }
       if(!exist){boundPoints[i-1].push_back(p);}
     }
     infile.close();
     delete [] str;
  }
//cerr<<"boundary points"<<endl;
  ProcessBoundary();
//cerr<<"boundary points"<<endl;
  GenerateMeshGlyphs(dir);
}

void svMeshData::ProcessBoundary(){
 for(int i=0;i<boundPoints.size();i++){
   svNear *near = new svNear();
   vector<svVector3> inData;
   vector<Spin> inSpin;
   vector<svVector3> bound;
   for(int j=0;j<myQDOT->qdotData.size();j++){
     if(myQDOT->qdotData[j].region == i+1){
        inData.push_back(myQDOT->qdotData[j].pos);
        inSpin.push_back(myQDOT->qdotData[j]);
     }
   }
   for(int j=0;j<boundPoints[i].size();j++){
     bound.push_back(boundPoints[i][j].pos);
   }
   int *index = new int[boundPoints[i].size()];
   near->ComputeNear(inData, bound, index);
   for(int j=0;j<boundPoints[i].size();j++){
     boundPoints[i][j]= inSpin[index[j]];
   }
   delete near;
   delete [] index;
   inData.clear();
   inSpin.clear();
   bound.clear();
 }
}

void svMeshData::GenerateMeshGlyphs(char *dir){
 char *str = new char[400];
//found the nearest triangle plane
 vector<svVector3> outData;
 vector<Spin> outSpin;
 for(int i=0;i<myQDOT->qdotData.size();i++){
    if(myQDOT->qdotData[i].region == 0){
       outData.push_back(myQDOT->qdotData[i].pos);
       outSpin.push_back(myQDOT->qdotData[i]);
    }
 }
 for(int i=0;i<boundPoints.size();i++){
   sprintf(str, "%s/meshglyphs%d.txt", dir, i+1);
   ifstream infile(str);
   if(infile.is_open()){
       infile.close();
       ReadGlyphsFile(i,str);
       continue;
   }
   vector<svVector3> inData;
   vector<svVector3> inNorm;
   for(int j=0;j<boundPoints[i].size();j++){
     svScalar td = 9e+9;
     svScalar dd = 9e+9;
     svVector3 p,pos;
     int findex = -1;
     for(int f=0;f<facets[i].size();f++){
       if(facets[i][f].pos[0][0] > boundPoints[i][j].pos[0] - 2.
       && facets[i][f].pos[0][1] > boundPoints[i][j].pos[1] - 2.
       && facets[i][f].pos[0][2] > boundPoints[i][j].pos[2] - 2.
       && facets[i][f].pos[0][0] < boundPoints[i][j].pos[0] + 2.
       && facets[i][f].pos[0][1] < boundPoints[i][j].pos[1] + 2.
       && facets[i][f].pos[0][2] < boundPoints[i][j].pos[2] + 2.){
         bool intersect = rayTriangleIntersect(boundPoints[i][j].pos,
                            facets[i][f].norm,
                            facets[i][f].pos[0],
                            facets[i][f].pos[1],
                            facets[i][f].pos[2],
                            -facets[i][f].norm,
                            td, p);//cerr<<intersect<<" ";
         if(intersect){if(td<dd){dd=td; findex = f;pos=p;}}
       }
     }
     meshGlyph tmpglyph;
     tmpglyph.pos=pos;tmpglyph.dir=facets[i][findex].norm;
     tmpglyph.inDen = boundPoints[i][j].exp;
     tmpglyph.inDir = boundPoints[i][j].dir;
     if(fabs(tmpglyph.pos[2]-myQDOT->regionZ[0][i])<1e-8){
         tmpglyph.dir[0]=0;tmpglyph.dir[1]=0;tmpglyph.dir[2]=-1;
     }
     else if(fabs(tmpglyph.pos[2]-myQDOT->regionZ[1][i])<1e-8){
         tmpglyph.dir[0]=0;tmpglyph.dir[1]=0;tmpglyph.dir[2]=1;
     }
     glyphs[i].push_back(tmpglyph);
     inData.push_back(pos);
     inNorm.push_back(facets[i][findex].norm);
   }
cerr<<"found the nearest neighbors in the vertical direction"<<endl;
   svNear *near = new svNear();
   int *index = new int[inData.size()];
   near->ComputeNear(outData, inData, inNorm, index);
   for(int j=0;j<inData.size();j++){
      glyphs[i][j].outDen = outSpin[index[j]].exp;
      glyphs[i][j].outDir = outSpin[index[j]].dir;
      glyphs[i][j].denDiff = fabs(glyphs[i][j].outDen-glyphs[i][j].inDen);
      glyphs[i][j].dirDiff = acos(GetDot(glyphs[i][j].outDir,
                                  glyphs[i][j].inDir));
   }
   delete [] index;
   inData.clear();
   inNorm.clear();
   SaveGlyphstoFile(i, str);
 }
 outSpin.clear();
 outData.clear();
 delete[] str;
}

void svMeshData::ReadGlyphsFile(int region, char *file){
  ifstream infile(file);
  int n;infile>>n;
  glyphs[region].resize(n);
  for(int i=0;i<glyphs[region].size();i++){
    infile>>glyphs[region][i].pos[0]
    >>glyphs[region][i].pos[1]
    >>glyphs[region][i].pos[2]
    >>glyphs[region][i].dir[0]
    >>glyphs[region][i].dir[1]
    >>glyphs[region][i].dir[2]
    >>glyphs[region][i].inDen
    >>glyphs[region][i].outDen
    >>glyphs[region][i].inDir[0]
    >>glyphs[region][i].inDir[1]
    >>glyphs[region][i].inDir[2]
    >>glyphs[region][i].outDir[0]
    >>glyphs[region][i].outDir[1]
    >>glyphs[region][i].outDir[2]
    >>glyphs[region][i].denDiff
    >>glyphs[region][i].dirDiff;
  }
  infile.close();
}

void svMeshData::SaveGlyphstoFile(int region, char *file){
  ofstream outfile(file);
  outfile<<glyphs[region].size()<<endl;
  for(int i=0;i<glyphs[region].size();i++){
    outfile<<glyphs[region][i].pos[0]
    <<" "<<glyphs[region][i].pos[1]
    <<" "<<glyphs[region][i].pos[2]
    <<" "<<glyphs[region][i].dir[0]
    <<" "<<glyphs[region][i].dir[1]
    <<" "<<glyphs[region][i].dir[2]
    <<" "<<glyphs[region][i].inDen
    <<" "<<glyphs[region][i].outDen
    <<" "<<glyphs[region][i].inDir[0]
    <<" "<<glyphs[region][i].inDir[1]
    <<" "<<glyphs[region][i].inDir[2]
    <<" "<<glyphs[region][i].outDir[0]
    <<" "<<glyphs[region][i].outDir[1]
    <<" "<<glyphs[region][i].outDir[2]
    <<" "<<glyphs[region][i].denDiff
    <<" "<<glyphs[region][i].dirDiff<<endl;
  }
  outfile.close();
}

//====================Slice Data============================
svRawSliceData::svRawSliceData(svQDOT *data){
  myQDOT = NULL;
  mySymmetry = NULL;
  myCluster = NULL;
  sliceList = new map<Spin *, int>();
  data_structure_type = d_slice;
  New(data);
}
void svRawSliceData::New(svQDOT *data){
  (*sliceList).clear();
  if(splitData.size()>0) {cleandata();cleanup();}
  myQDOT = data;
  maxExp = myQDOT->maxExp;
  minExp = myQDOT->minExp;
}
void svRawSliceData::Init(svVector3 &center, svVector3 &dir, svScalar &distance){
  dir = normalize(dir);
  SplitData(center, dir, distance);
  GenerateAxis(center, dir, distance);
  sliceDir = dir;
  sliceDistance = distance;
  GenerateOrigins();
  GenerateSlice();
  GenerateHistogram();

  int size = splitData.size();
  isSelectable.resize(size);
  std::fill(isSelectable.begin(), isSelectable.end(), true);
  isVisible.resize(size);
  std::fill(isVisible.begin(), isVisible.end(), true);
  isHighlighted.resize(size);
  std::fill(isHighlighted.begin(), isHighlighted.end(), false);
  isSelected.resize(size);
  std::fill(isSelected.begin(), isSelected.end(), false);
  /*for(int i=0;i<myQDOT->qdotData.size();i++){
    visibleLabel.insert(std::pair<Spin *, bool>(&(myQDOT->qdotData[i]), true));
  }*/
  //state->NewVisible(myQDOT);
}
void svRawSliceData::SaveSlicetoVTK(char *dir, char *file){
  char *str = new char[1024];
  for(int i=0;i<splitData.size();i++){
     sprintf(str,"%s/%s/%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f%d.vtk", dir, file,
             sliceMinCenter[0], sliceMinCenter[1], sliceMinCenter[2],
             sliceDir[0], sliceDir[1], sliceDir[2],
             i );
     ifstream infile(str);
     if(infile.is_open()){ infile.close(); continue;}
     cerr<<str<<endl;
     ofstream outfile(str);
     outfile<<"# vtk DataFile Version 2.0"<<endl;
     outfile<<str<<endl;
     outfile<<"ASCII"<<endl;
     outfile<<"DATASET UNSTRUCTURED_GRID"<<endl;

     outfile<<"POINTS "<<splitData[i].size()<<" float"<<endl;
     for(int j=0;j<splitData[i].size();j++){//TODO fix the plane cooridinates
        outfile<<(*splitData[i][j]).pos[0]<<" "
               <<(*splitData[i][j]).pos[1]<<" "
               <<(*splitData[i][j]).pos[2]<<endl;
     }
     outfile<<endl;
     outfile<<"POINT_DATA "<<splitData[i].size()<<endl;
     outfile<<"VECTORS velocity float"<<endl;
     for(int j=0;j<splitData[i].size();j++){
        outfile<<(*splitData[i][j]).dir[0]<<" "
               <<(*splitData[i][j]).dir[1]<<" "
               <<(*splitData[i][j]).dir[2]<<endl;
     }
     outfile<<endl;
     outfile<<"SCALARS density float 1"<<endl;
     outfile<<"LOOKUP_TABLE default"<<endl;
     for(int j=0;j<splitData[i].size();j++){
       outfile<<(*splitData[i][j]).den<<endl;
     }
     outfile<<endl;
     outfile.close();
  }
  delete [] str;
}

void svRawSliceData::SaveSliceDensitytoFile(char *dir){
  char *str = new char[1024];
  for(int i=0;i<imageData.size();i++){
     sprintf(str,"%s/density%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f%d.txt", dir,
             sliceMinCenter[0], sliceMinCenter[1], sliceMinCenter[2],
             sliceDir[0], sliceDir[1], sliceDir[2],
             i);
     ofstream outfile(str);
     //cout<<imageRow<<" "<<imageColumn<<endl;
     outfile<<1<<" "<<imageRow<<" "<<imageColumn<<endl;
     for(int r=imageRow-1;r>=0;r--){
       for(int c=0;c<imageColumn;c++){
          int j = r * imageColumn + c;
          int index = imageData[i][j];
          if(index<0) outfile<<0<<" ";
          else outfile<<(*splitData[i][sliceData[i][index].torindex]).den<<" ";
       }
       outfile<<endl;
     }
     outfile.close();
  }
  delete [] str;
}

void svRawSliceData::SplitData(svVector3 &center, svVector3 &dir, svScalar &distance){
  svVector3 minCenter, maxCenter;
  minCenter = center; maxCenter = center;
  if(dir[2]<0) dir = -dir;
  else if(dir[1]<0) dir = -dir;
  else if(dir[0]<0) dir = -dir;
  while(1){
     minCenter = minCenter - dir * distance;
     if(!planeBox(myQDOT->lbbox, myQDOT->rbbox, minCenter, dir)){
        minCenter = minCenter + dir * distance;break;
     }
  }
  while(1){
     maxCenter = maxCenter + dir * distance;
     if(!planeBox(myQDOT->lbbox, myQDOT->rbbox, maxCenter, dir)){
        maxCenter = maxCenter - dir * distance;break;
     }
  }
  int numSlice = sqrt((maxCenter[0]-minCenter[0])*(maxCenter[0]-minCenter[0])
         + (maxCenter[1]-minCenter[1])*(maxCenter[1]-minCenter[1])
         + (maxCenter[2]-minCenter[2])*(maxCenter[2]-minCenter[2]))/distance+1;
  splitData.resize(numSlice);
  sliceDen.resize(numSlice);
  for(int i=0;i<numSlice;i++)sliceDen[i]=0;
  for(int i=0;i<(myQDOT->qdotData).size();i++){
     Spin tmpspin = (myQDOT->qdotData).at(i);
     svVector3 tmp = tmpspin.pos - minCenter;
     double dot = GetDot(tmp, dir);
     if((fmod(dot, (double)distance)<1e-5)
        || (distance-fmod(dot, (double)distance)<1e-5)){
        int index = (int)(dot/distance);
        splitData[index].push_back(&((myQDOT->qdotData).at(i)));
        (*sliceList).insert(std::pair<Spin*,int>(&((myQDOT->qdotData).at(i)),index));
        if((myQDOT->qdotData).at(i).den > sliceDen[index]){
          sliceDen[index] = (myQDOT->qdotData).at(i).den;
        }
     }
  }
  sliceMinCenter = minCenter;
}

void svRawSliceData::GenerateAxis(svVector3 &center, svVector3 &dir, svScalar &distance){
  dir = normalize(dir);
  svVector3 xcoor, ycoor;
  svVector3 zcoor;
  zcoor[0]=0;zcoor[1]=0;zcoor[2]=1;
  if(fabs(dir[0])<1e-8 && fabs(dir[1])<1e-8 && fabs(dir[2]-1)<1e-8){
     xcoor[0]=1;xcoor[1]=0;xcoor[2]=0;
     ycoor[0]=0;ycoor[1]=1;ycoor[2]=0;
  }
  else{
     xcoor = cross(zcoor, dir);
     ycoor = cross(dir, xcoor);
  }
  sliceAxis[0] = normalize(xcoor);
  sliceAxis[1] = normalize(ycoor);
  sliceAxis[2] = normalize(dir);
  //cerr<<xcoor[0]<<" "<<xcoor[1]<<" "<<xcoor[2]<<endl;
  //cerr<<ycoor[0]<<" "<<ycoor[1]<<" "<<ycoor[2]<<endl;
  //cerr<<dir[0]<<" "<<dir[1]<<" "<<dir[2]<<endl;
}

void svRawSliceData::GenerateOrigins(){
  svVector3 ilbbox, irbbox;
  ilbbox[0]=9e+9;ilbbox[1]=9e+9;ilbbox[2]=9e+9;
  irbbox[0]=-9e+9;irbbox[1]=-9e+9;irbbox[2]=-9e+9;
  svVector3 pos = (*splitData[0][0]).pos;
  for(int i=0;i<splitData.size();i++){
     for(int j=0;j<splitData[i].size();j++){
        svVector3 currentPos = (*splitData[i][j]).pos;
        svVector3 newpos;
        newpos[0] = GetDot(currentPos-pos,sliceAxis[0]);
        newpos[1] = GetDot(currentPos-pos,sliceAxis[1]);
        if(ilbbox[0]>newpos[0]){ilbbox[0]=newpos[0];}
        if(ilbbox[1]>newpos[1]){ilbbox[1]=newpos[1];}
     }
  }

  svVector3 origin = ilbbox[0] * sliceAxis[0]
                   + ilbbox[1] * sliceAxis[1]
                   + pos;
  for(int i=0;i<splitData.size();i++){
     svVector3 pp = sliceMinCenter  + (svScalar)i * sliceDistance * sliceDir;
     double od = pointtoPlane(origin, pp, sliceDir);
     svVector3 op = origin+ (svScalar)od * sliceDir;
     sliceOrigin.push_back(op);
     //cerr<<op[0]<<" "<<op[1]<<" "<<op[2]<<endl;
  }
}

void svRawSliceData::GenerateHistogram()
{
  for(int i=0;i<NUM_TOP_MAG;i++)
  {
    histovalues[i].free();
  }
  delete [] histovalues;
  histovalues = new svScalarArray[NUM_TOP_MAG];

  svScalar max = myQDOT->maxExp;
  svScalar level = max - NUM_TOP_MAG;
  int *count = new int[splitData.size()];
  for(int i=0;i<splitData.size();i++)
  {
    count[i] = 0;
    for(int j=0;j<splitData[i].size();j++)
    {
          count[i]++;
    }
  }
  for(int i=0;i<splitData.size();i++)
  {
    for(int h=0;h<NUM_TOP_MAG;h++)
      histovalues[h].add(0);
     for(int j=0;j<splitData[i].size();j++)
     {
       svScalar exp = (*splitData[i][j]).exp;
       svScalar coe = (*splitData[i][j]).coe;
        if(exp > level&&coe>0)
        {
          for(int h=0;h<NUM_TOP_MAG;h++)
          {
            if(isEqual(exp, level+h+1, false))
              histovalues[h][i] += 1.;
          }
          /* if(fabs(exp[i][j]-level-1)<1e-3)
                prob[3][i] = prob[3][i]+1;
           if(fabs(exp[i][j]-level-2)<1e-3)
                prob[2][i] = prob[2][i]+1;
           if(fabs(exp[i][j]-level-3)<1e-3)
                prob[1][i] = prob[1][i]+1;
           if(fabs(exp[i][j]-level-4)<1e-3)
                prob[0][i] = prob[0][i]+1;*/
        }
     }
     //prob[i] = prob[i]/ (svScalar)count[i];//cerr<<prob[i]<<endl;
  }//cerr<<c<<endl;
}

bool sortSlice(svSliceData data1, svSliceData data2){
  if(fabs(data1.ipos[1]-data2.ipos[1])>1e-8){
      return (data1.ipos[1]<data2.ipos[1]);
  }
  else if(fabs(data1.ipos[0]-data2.ipos[0])>1e-8){
      return (data1.ipos[0]<data2.ipos[0]);
  }
}

void svRawSliceData::GenerateSlice(){
  sliceData.resize(sliceOrigin.size());
  imageData.resize(sliceOrigin.size());
  ilbbox[0]=9e+9;ilbbox[1]=9e+9;ilbbox[2]=9e+9;
  irbbox[0]=-9e+9;irbbox[1]=-9e+9;irbbox[2]=-9e+9;
  for(int i=0;i<sliceData.size();i++){
    svVector3 pp = sliceMinCenter + (svScalar)i * sliceDistance * sliceDir;
    for(int j=0;j<splitData[i].size();j++){
       svVector3 newpos;
       newpos[0] = GetDot((*splitData[i][j]).pos-sliceOrigin[i],sliceAxis[0]);
       newpos[1] = GetDot((*splitData[i][j]).pos-sliceOrigin[i],sliceAxis[1]);
       newpos[2] = GetDot((*splitData[i][j]).pos-sliceOrigin[i],sliceAxis[2]);
       svSliceData tmpslice;
       tmpslice.ipos = newpos;
       tmpslice.torindex = j;
       tmpslice.iheight = 0.;
       tmpslice.qdotPoint = splitData[i][j];
       //tmpslice.idir = (*splitData[i][j]).dir;
       sliceData[i].push_back(tmpslice);
       if(ilbbox[0]>newpos[0]){ilbbox[0]=newpos[0];}
       if(ilbbox[1]>newpos[1]){ilbbox[1]=newpos[1];}
       if(ilbbox[2]>newpos[2]){ilbbox[2]=newpos[2];}
       if(irbbox[0]<newpos[0]){irbbox[0]=newpos[0];}
       if(irbbox[1]<newpos[1]){irbbox[1]=newpos[1];}
       if(irbbox[2]<newpos[2]){irbbox[2]=newpos[2];}
    }
    sort(sliceData[i].begin(), sliceData[i].end(), sortSlice);
  }
  GenerateImage();
}

void svRawSliceData::GenerateImage(){
//grid
   int ii = sliceData.size()/2;
   for(int j=0;j<sliceData[ii].size();j++){
      svScalar dd = sliceData[ii][j].ipos[1] - sliceData[ii][0].ipos[1];
      if(dd>0){
         iminD[1] = dd;break;
      }
   }
   iminD[0]=9e+9;
   for(int j=1;j<sliceData[ii].size();j++){
      svScalar dd = sliceData[ii][j].ipos[0] - sliceData[ii][0].ipos[0];
      if(dd>0 && dd<iminD[0]) iminD[0] =dd;
   }
   iminD[2] = sliceDistance;
   svScalar minD = min(iminD[0], iminD[1]);
//image
   int fill = (int)((sliceData[ii][1].ipos[0]
                   - sliceData[ii][0].ipos[0])/minD);
   int row = (irbbox[1] - ilbbox[1])/(minD)+1;
   int column = (irbbox[0] - ilbbox[0])/(minD)+fill;
   //cerr<<row<<" "<<column<<" "<<iminD[0]<<" "<<iminD[1]<<" "<<iminD[2]<<" "<<fill<<" "<<irbbox[0]<<" "<<ilbbox[0]<<endl;
   imageRow = row; imageColumn = column;
   for(int i=0;i<sliceData.size();i++){
      int indexj = 0;
      int indexr1 = (sliceData[i][0].ipos[1]-ilbbox[1])/minD;
      int indexr2 = (sliceData[i][sliceData[i].size()-1].ipos[1]-ilbbox[1])/minD;
      //cerr<<indexr1<<" "<<indexr2<<endl;
      for(int j=0; j<row; j++){//1cerr<<i<<" "<<j<<endl;
        if(j<indexr1 || j>indexr2){//cout<<"out"<<endl;
         for(int t=0;t<column;t++){
           imageData[i].push_back(-1);
         }
        }
        else{
         //cerr<<"column"<<" "<<column<<" "<<indexj<<" "<<splitData[i].size()<<endl;
         for(int t=0;t<column;){
           if(indexj >= sliceData[i].size()){ //fill in the pixel after the data
               int tsize = t;
               for(int d=tsize;d<tsize+fill-1;d++){
                   imageData[i].push_back(indexj-1);t++;
               }
               tsize = t;
               for(int d=tsize;d<column;d++){
                imageData[i].push_back(-1);t++;
               } break;
           }
           int index = (sliceData[i][indexj].ipos[0]-ilbbox[0])/minD;
//           cerr<<t<<" "<<sliceData[i][indexj].ipos[0]<<" "<<indexj<<" "<<index<<endl;
           //TODO: DOUBLE CHECK
           if(t>index){//t>0&&sliceData[i][indexj].ipos[1]>sliceData[i][indexj-1].ipos[1]){
               //cerr<<t<<endl;
               int tsize= t;
               for(int d=tsize;d<tsize+fill-1;d++){
                   imageData[i].push_back(indexj-1);t++;
               }
               tsize = t;
               for(int d=tsize;d<column;d++){
                imageData[i].push_back(-1);t++;
               } break;
            }
            if(index>t){
             if(t==0){ //fill in the pixel before the data
              for(int d=0;d<index;d++){
                imageData[i].push_back(-1);t++;
              }
             }
             else{ //fill in the pixel between two data
              int indexd = index - t;
              int mid = indexd/2;
              for(int d=0;d<=mid;d++){
                imageData[i].push_back(indexj-1); t++;
              }
              for(int d=mid+1;d<indexd;d++){
               imageData[i].push_back(indexj);t++;
              }
              imageData[i].push_back(indexj);
              t++; indexj++;
             }
            }
            else if(index==t){ //fill in the pixel of a data
              imageData[i].push_back(indexj);
              t++; indexj++;
            }
          }
       }
    }
  }
}
void svRawSliceData::GenerateSampling(int frequency){
  state->sampleVisible.clear();
  for(int i=0;i<splitData.size();i++){
     for(int j=0;j<splitData[i].size();j++){
        vector<int> value(3);
        value = myQDOT->qdotFormat.at(splitData[i][j]);
        if(value[0]%frequency==0
          && value[1]%frequency==0
          && value[2]%frequency==0){
          state->sampleVisible.insert(std::pair<Spin *, bool>(splitData[i][j], true));
        }
        else
          state->sampleVisible.insert(std::pair<Spin *, bool>(splitData[i][j], false));
     }
  }
  if(mySymmetry == NULL) return;
  for(int i=0;i<SYMMETRY_TYPE;i++){
     if(mySymmetry->splitData[i].size()<=0) continue;
     for(int j=0;j<myQDOT->qdotData.size();j++){
        if((*(mySymmetry->symmetryList))[i].count(&myQDOT->qdotData[j])<=0)
             continue;
        vector<Spin *> value = (*(mySymmetry->symmetryList))[i].at(&myQDOT->qdotData[j]);
        if(value.size()>0){
            if(state->sampleVisible.at(&myQDOT->qdotData[j])==true){
              for(int v=0;v<value.size();v++){
                 state->sampleVisible.at(value[v]) = true;
              }
            }
        }
     }
  }
}
void svRawSliceData::cleanup(){
  sliceDen.clear();
  sliceOrigin.clear();
  for(int i=0;i<imageData.size();i++){
     imageData[i].clear();
     sliceData[i].clear();
  }
  imageData.clear();
  sliceData.clear();
}
//======================================================================

//============================Symmetry Data=========================//
svRawSymmetryData::svRawSymmetryData(svQDOT *qdot){
  mySlice = NULL;
  myCluster = NULL;
  symmetryList = new vector< map<Spin *, vector<Spin *> > >();
  (*symmetryList).resize(SYMMETRY_TYPE);
  New(qdot);
  data_structure_type = d_symmetry;
}
void svRawSymmetryData::New(svQDOT *qdot){
  if(splitData.size()>0) {cleandata();}
  myQDOT = qdot;
  for(int i=0;i<SYMMETRY_TYPE;i++)(*symmetryList)[i].clear();
  maxExp = myQDOT->maxExp;
  minExp = myQDOT->minExp;
}
void svRawSymmetryData::Init(char *dir){
  splitData.resize(SYMMETRY_TYPE+1);
  GenerateSymmetry(dir);
  isSelectable.resize(SYMMETRY_TYPE+1);
  std::fill(isSelectable.begin(), isSelectable.end(), true);
  isVisible.resize(SYMMETRY_TYPE+1);
  std::fill(isVisible.begin(), isVisible.end(), true);
  isHighlighted.resize(SYMMETRY_TYPE+1);
  std::fill(isHighlighted.begin(), isHighlighted.end(), false);
  isSelected.resize(SYMMETRY_TYPE+1);
  std::fill(isSelected.begin(), isSelected.end(), false);
}
void svRawSymmetryData::GenerateSymmetry(char *dir){
  svSymmetry *symmetry = new svSymmetry();
  svVector3 cutPos(0,0,myQDOT->lbbox[2]);
  svVector3 cutDir(0,0,1);
  symmetry->ComputeSymmetry(*state->symP, myQDOT, cutPos, cutDir, dir);
  char *str = new char[400];
  cutPos = (state->symP)->pos;
  cutDir = (state->symP)->dir;
  for(int i=0;i<SYMMETRY_TYPE;i++){
    switch (i){
      case 0: sprintf(str, "%s/xsyszs%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f.txt",
                           dir, cutPos[0], cutPos[1], cutPos[2],
                           cutDir[0], cutDir[1], cutDir[2]); break;
      case 1: sprintf(str, "%s/xsysza%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f.txt",
                           dir, cutPos[0], cutPos[1], cutPos[2],
                           cutDir[0], cutDir[1], cutDir[2]); break;
      case 2: sprintf(str, "%s/xsyazs%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f.txt",
                           dir, cutPos[0], cutPos[1], cutPos[2],
                           cutDir[0], cutDir[1], cutDir[2]); break;
      case 3: sprintf(str, "%s/xsyaza%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f.txt",
                           dir, cutPos[0], cutPos[1], cutPos[2],
                           cutDir[0], cutDir[1], cutDir[2]); break;
      case 4: sprintf(str, "%s/xayszs%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f.txt",
                           dir, cutPos[0], cutPos[1], cutPos[2],
                           cutDir[0], cutDir[1], cutDir[2]); break;
      case 5: sprintf(str, "%s/xaysza%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f.txt",
                           dir, cutPos[0], cutPos[1], cutPos[2],
                           cutDir[0], cutDir[1], cutDir[2]); break;
      case 6: sprintf(str, "%s/xayazs%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f.txt",
                           dir, cutPos[0], cutPos[1], cutPos[2],
                           cutDir[0], cutDir[1], cutDir[2]); break;
      case 7: sprintf(str, "%s/xayaza%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f.txt",
                           dir, cutPos[0], cutPos[1], cutPos[2],
                           cutDir[0], cutDir[1], cutDir[2]); break;
    }
    ifstream infile(str);
    int n;
    infile>>n;
    for(int j=0;j<myQDOT->qdotData.size();j++){
        int index; infile>>index;
        vector<Spin *> value;
        for(int s=0;s<index;s++){
           int in;infile>>in;
           //the first element is the  current spin, no need to store it
           if(s>0) value.push_back(&(myQDOT->qdotData[in]));
        }
        if(value.size()>0)
        (*symmetryList)[i].insert(std::pair<Spin *, vector<Spin *> >
                                (&(myQDOT->qdotData[j]), value));
        value.clear();
    }
    infile.close();
  }
  delete symmetry;

  SplitData();
}

void svRawSymmetryData::SplitData(){
  int *exist = new int[myQDOT->qdotData.size()]();
  for(int i=0;i<(*symmetryList).size();i++){
   if(!(*symmetryList)[i].empty()){
    for(int j=0;j<myQDOT->qdotData.size();j++){
      if((*symmetryList)[i].count(&(myQDOT->qdotData[j]))>0){
         splitData[i].push_back(&(myQDOT->qdotData[j]));
         exist[j] = 1;
      }
    }
   }
  }
  for(int i=0;i<myQDOT->qdotData.size();i++){
     if(exist[i]==0) splitData[SYMMETRY_TYPE].push_back(&(myQDOT->qdotData[i]));
  }
  delete [] exist;
}

//======================================================================

//==================svRawClusterData====================================//
svRawClusterData::svRawClusterData(svQDOT *qdot){
  mySlice = NULL;
  mySymmetry = NULL;
  clusterList = new map<Spin *, int>();
  New(qdot);
  data_structure_type = d_cluster;
}
void svRawClusterData::New(svQDOT *qdot){
  myQDOT = qdot;
  (*clusterList).clear();
  if(splitData.size()>0){cleandata();cleanup();}
  for(int i=0;i<myQDOT->qdotData.size();i++){
     sampleLabel.insert(std::pair<Spin *, int>(&(myQDOT->qdotData[i]), 0.));
     (*clusterList).insert(std::pair<Spin *, int>(&(myQDOT->qdotData[i]), -1));
  }
  maxExp = myQDOT->maxExp;
  minExp = myQDOT->minExp;
}
void svRawClusterData::ResetCluster(){
  for(int i=0;i<myQDOT->qdotData.size();i++){
     (*clusterList).at(&(myQDOT->qdotData[i])) = -1;
     sampleLabel.at(&(myQDOT->qdotData[i])) = 0.;
  }
  for(int i=0;i<splitData.size();i++){
      splitData[i].clear();
  }
  for(list< vector<int> >::iterator it=boundaryIndex.begin();it!=boundaryIndex.end();++it)
  {
     (*it).clear();
  }
  boundaryIndex.clear();
  splitData.clear();
}
void svRawClusterData::Init(char *folder){//, KmeansProperty & property){
  GenerateClustersBySymmetry(folder);//, *(state->kP));
  GenerateSampleLabel(folder);
  int size = splitData.size();
  isSelectable.resize(size);
  std::fill(isSelectable.begin(), isSelectable.end(), true);
  isVisible.resize(size);
  std::fill(isVisible.begin(), isVisible.end(), true);
  isHighlighted.resize(size);
  std::fill(isHighlighted.begin(), isHighlighted.end(), false);
  isSelected.resize(size);
  std::fill(isSelected.begin(), isSelected.end(), false);
//  for(int i=0;i<myQDOT->qdotData.size();i++){
//    visibleLabel.insert(std::pair<Spin *, bool>(&(myQDOT->qdotData[i]), true));
//  }
}
void svRawClusterData::GenerateClustersBySymmetry(char *folder){
 //                                          KmeansProperty & property){
  vector< vector<Spin*> > tmpdata;
  tmpdata.resize(mySlice->splitData.size());
  svVector3 pos = state->symP->pos;//mySymmetry->property->pos;
  svVector3 dir = state->symP->dir;//mySymmetry->property->dir;
  for(int i=0;i<SYMMETRY_TYPE;i++){
    if(mySymmetry->splitData[i].size()>0){
      //if the data is one the same side of the normal direction of symmetry plane, push it .
      for(int j=0;j<mySymmetry->splitData[i].size();j++){
         svVector3 p = (*mySymmetry->splitData[i][j]).pos - pos;
         svScalar d = dot(p, dir);
         int sliceindex = (*mySlice->sliceList).at(mySymmetry->splitData[i][j]);
         if(d > 0) tmpdata[sliceindex].push_back(mySymmetry->splitData[i][j]);
      }
    }
  }
 //cerr<<"case: add the no symmetry data "<<endl;
  for(int j=0;j<mySymmetry->splitData[SYMMETRY_TYPE].size();j++){
    int sliceindex = (*mySlice->sliceList).at(mySymmetry->splitData[SYMMETRY_TYPE][j]);
    tmpdata[sliceindex].push_back((mySymmetry->splitData[SYMMETRY_TYPE][j]));
  }
  char *clusterfname = new char[400];
  GenerateCluster(tmpdata,  folder, clusterfname);
  for(int i=0;i<tmpdata.size();i++)tmpdata[i].clear();
  tmpdata.clear();
  delete [] clusterfname;
}
void svRawClusterData::GenerateCluster(vector< vector<Spin*> > tmpdata,
                                       char *dir, char *clusterfname){
  char *ifname = new char[400];
  char *ofname = new char[400];
  sprintf(ifname,"%s/tmpdata.txt", dir);
  ofstream outfile(ifname);
  outfile<<tmpdata.size()<<endl;
  int maxsize = -1;
  for(int i=0;i<tmpdata.size();i++){
   outfile<<tmpdata[i].size()<<endl;
   int size = tmpdata[i].size();
   if(size>maxsize){
	 maxsize = tmpdata[i].size();
   }
   for(int j=0;j<size;j++){
    outfile<<(*tmpdata[i][j]).pos[0]<<" "
           <<(*tmpdata[i][j]).pos[1]<<" "
           <<(*tmpdata[i][j]).pos[2]<<" "
           <<(*tmpdata[i][j]).dir[0]<<" "
           <<(*tmpdata[i][j]).dir[1]<<" "
           <<(*tmpdata[i][j]).dir[2]<<" "
           <<(*tmpdata[i][j]).den<<endl;
    }
  }
  outfile.close();
  sprintf(ofname,"%s/tmpdatanormalize.txt",dir);
  sprintf(clusterfname,"%s/cluster.txt",dir);

  svKmeans *cluster = new svKmeans();
  int **tmplist = new int*[mySlice->splitData.size()];
  //cerr<<mySlice->splitData.size()<<" "<<maxsize<<endl;
  for(int i=0;i<mySlice->splitData.size();i++) tmplist[i]=new int[maxsize];
  for(int i=0;i<tmpdata.size();i++)for(int j=0;j<maxsize;j++)tmplist[i][j]=-1;
  int maxCluster = cluster->ComputeClusters(ifname, ofname, clusterfname, *state->kP, tmplist);
  cerr<<"maxCluster "<<maxCluster<<endl;
  splitData.resize(maxCluster);//-1, 0, 1, 2, ...
  boundaryIndex.resize(maxCluster);
  SetClusterLabel(tmpdata, tmplist);
  delete cluster;
  char *exe = new char[1024];
  sprintf(exe, "rm %s", ofname); system(exe);
  sprintf(exe, "rm %s", ifname); system(exe);
  sprintf(exe, "rm %s", clusterfname); system(exe);
  delete [] exe; delete [] ifname; delete [] ofname; //delete [] clusterfName;
}
/*
void svRawClusterData::SetClusterLabel(vector<Spin*> tmpdata, char *file){
  vector<int> cluster;
  ifstream infile(file);
  int c;
  while(infile>>c){cluster.push_back(c);}
  infile.close();
  for(int i=0;i<SYMMETRY_TYPE;i++){
    if(mySymmetry->splitData[i].size()>0){
       for(int j=0;j<tmpdata.size();j++){
         (*clusterList).at(tmpdata[j]) = cluster[j];
         (*clusterList).at((mySymmetry->symmetryList[i].at(tmpdata[j]))[0])=cluster[j];
       }
    }
  }
  for(int i=0;i<myQDOT->qdotData.size();i++){
    int index = (*clusterList).at(&myQDOT->qdotData[i]);
    splitData[index].push_back(&myQDOT->qdotData[i]);
  }
  cluster.clear();
}
*/
void svRawClusterData::SetClusterLabel(vector< vector<Spin*> > tmpdata, int **tmplist){
  for(int j=0;j<tmpdata.size();j++){
     for(int l=0;l<tmpdata[j].size();l++){
         (*clusterList).at(tmpdata[j][l]) = tmplist[j][l];
     }
  }
/*assign cluster labels to the symmetry data*/
  for(int i=0;i<SYMMETRY_TYPE;i++){
    if(mySymmetry->splitData[i].size()>0){
       for(int j=0;j<tmpdata.size();j++){
        for(int l=0;l<tmpdata[j].size();l++){
         if((*(mySymmetry->symmetryList))[i].count(tmpdata[j][l])>0)
         (*clusterList).at(((*(mySymmetry->symmetryList))[i].at(tmpdata[j][l]))[0])=tmplist[j][l];
        }
       }
    }
  }
/*split data*/
/*index = -1*/
  for(int i=0;i<myQDOT->qdotData.size();i++){
    int index = (*clusterList).at(&myQDOT->qdotData[i]);
    splitData[index+1].push_back(&myQDOT->qdotData[i]);
  }
}

//void svRawClusterData::GenerateSampling(char *dir, int frequency){
//  connectIndex.resize(splitData.size());
//  list< vector<int> >::iterator it=connectIndex.begin();
//  numOfComponents.resize(splitData.size());
//  #pragma omp parallel for
//  for(int i=0;i<splitData.size();i++){
//   cerr<<"========="<<i<<"=========="<<endl;
/*   svNear *near = new svNear();
   svScalar minDistance = sqrt(myQDOT->minXDistance*myQDOT->minXDistance
                        + myQDOT->minYDistance*myQDOT->minYDistance
                        + myQDOT->minZDistance*myQDOT->minZDistance);
   near->ComputeComponents(splitData[i], numOfComponents[i], *it, minDistance);
   cerr<<numOfComponents[i]<<endl;
   ++it;
   delete near;
*/
//----------------find the boundary points in each cluster------//
//   GenerateBoundary(i, dir);
//  }
//----------------boundary points end---------------------------//
//  GenerateSampling(frequency);
//}
//TODO:Integrate Alphashape in quanvislib
//TODO:OpenMP
void svRawClusterData::GenerateBoundary(int splitlevel, char *dir){
   int i = splitlevel;
   char *ofname = new char[400];
   sprintf(ofname, "%s/clustertmp%d.txt", dir,i);
   //cerr<<ofname<<endl;
   ofstream outfile(ofname);
   outfile<<splitData[i].size()<<endl;
   for(int j=0;j<splitData[i].size();j++){
     outfile<<(*splitData[i][j]).pos[0]<<" "
            <<(*splitData[i][j]).pos[1]<<" "
            <<(*splitData[i][j]).pos[2]<<endl;
   }
   outfile.close();
   char *pointfname = new char[400];
   sprintf(pointfname, "%s/clusterpoints%d.txt", dir,i);
   svMeshGenerator *mesh = new svMeshGenerator();
   mesh->GenerateBoundaryPoints(ofname, pointfname);
   ifstream infile(pointfname);
   int n; infile>>n;
   vector<svVector3> tmpbound(n);
   for(int j=0;j<n;j++){
      infile>>tmpbound[j][0]>>tmpbound[j][1]>>tmpbound[j][2];
   }
   infile.close();
   //cerr<<pointfname<<endl;
   for(int j=0;j<splitData[i].size();j++){
      int index = -1;
      for(int t=0;t<tmpbound.size();t++){
        if(isEqual((*splitData[i][j]).pos[0], tmpbound[t][0], false)
        && isEqual((*splitData[i][j]).pos[1], tmpbound[t][1], false)
        && isEqual((*splitData[i][j]).pos[2], tmpbound[t][2], false)){
           index = t;
           break;
        }
      }
      if(index>0){
         std::list< vector<int> >::iterator it = boundaryIndex.begin();
         for(int t=0;t<i;t++) ++it;
         (*it).push_back(j);
         //sampleLabel.at(splitData[i][j])= true;
      }
      else{
         //sampleLabel.insert(splitData[i][j])= false;
      }
   }
   delete [] ofname;
   delete [] pointfname;
}
//sampling based on x, y and z
void svRawClusterData::GenerateSampling(int frequency){
  state->sampleVisible.clear();
  for(int i=0;i<splitData.size();i++){
     for(int j=0;j<splitData[i].size();j++){
        vector<int> value(3);
        value = myQDOT->qdotFormat.at(splitData[i][j]);
        if(value[0]%frequency==0
          && value[1]%frequency==0
          && value[2]%frequency==0){
          state->sampleVisible.insert(std::pair<Spin *, bool>(splitData[i][j], true));
        }
        else
          state->sampleVisible.insert(std::pair<Spin *, bool>(splitData[i][j], false));
     }
  }
  if(mySymmetry == NULL) return;
  for(int i=0;i<SYMMETRY_TYPE;i++){
     if(mySymmetry->splitData[i].size()<=0) continue;
     for(int j=0;j<myQDOT->qdotData.size();j++){
        if((*(mySymmetry->symmetryList))[i].count(&myQDOT->qdotData[j])<=0)
             continue;
        vector<Spin *> value = (*(mySymmetry->symmetryList))[i].at(&myQDOT->qdotData[j]);
        if(value.size()>0){
            if(state->sampleVisible.at(&myQDOT->qdotData[j])==true){
              for(int v=0;v<value.size();v++){
                 state->sampleVisible.at(value[v]) = true;
              }
            }
        }
     }
  }
}

//TODO:Sampling each cluster
//DBSCAN instead of alpha shape
void svRawClusterData::GenerateClusterSampling(int percentage){
  state->sampleVisible.clear();
  for(int i=0;i<myQDOT->qdotData.size();i++){
     if(sampleLabel.at(&myQDOT->qdotData[i])<=percentage){
        state->sampleVisible.insert(std::pair<Spin*, int>(&myQDOT->qdotData[i],true));
     }
     else
        state->sampleVisible.insert(std::pair<Spin*, int>(&myQDOT->qdotData[i],false));

  }
  int i = 0;
  for(std::list< vector<int> >::iterator it=boundaryIndex.begin();
        it!=boundaryIndex.end();++it){
    for(int j=0;j<(*it).size();j++){
        state->sampleVisible.at(splitData[i][(*it)[j]]) = true;
    }
    i++;
  }
}
void svRawClusterData::GenerateSampleLabel(char *folder){
  string ifname(folder); ifname = ifname + "tmpdata.txt";
  string ofname(folder); ofname = ofname + "tmpdatanormalize.txt";
  string clustername(folder); clustername = clustername + "cluster.txt";
  cerr<<ifname<<" "<<ofname<<" "<<clustername<<endl;
//=======================Boundary points of each cluster===========//
  #pragma omp parallel for
  for(int i=0;i<splitData.size();i++){
    GenerateBoundary(i, folder);
  }
cerr<<"//========================sample label============================//"<<endl;
  for(int i=0;i<splitData.size();i++){
    if(splitData[i].size()==0) continue;
    vector<Spin *> tmpdata;
    svVector3 pos = state->symP->pos;
    svVector3 dir = state->symP->dir;
cerr<<"//====================Symmetry=========================================//"<<endl;
    for(int j=0;j<splitData[i].size();j++){
      bool exist = false;
      for(int ii=0;ii<SYMMETRY_TYPE;ii++){
        if(mySymmetry->splitData[ii].size()==0) continue;
        if((*mySymmetry->symmetryList)[ii].count(splitData[i][j])>0){
          exist = true;
          svVector3 p = (*splitData[i][j]).pos-pos;
          svScalar d = dot(p, dir);
          if(d>0) tmpdata.push_back(splitData[i][j]);
        }
      }
      if(!exist){
         tmpdata.push_back(splitData[i][j]);
      }
    }
cerr<<"//================Clustering===========================================//"<<endl;
    ofstream outfile(ifname.c_str());
    outfile<<1<<endl;
    outfile<<tmpdata.size()<<endl;
    for(int j=0;j<tmpdata.size();j++){
       outfile<<0<<" "<<0<<" "<<0<<" "
           <<(*tmpdata[j]).dir[0]<<" "
           <<(*tmpdata[j]).dir[1]<<" "
           <<(*tmpdata[j]).dir[2]<<" "
           <<(*tmpdata[j]).den<<endl;
    }
    outfile.close();
    svKmeans *cluster = new svKmeans();
    int **tmplist = new int*[1];
    KmeansProperty *cP = new KmeansProperty();
    cP->isNormalize.push_back(true);
    cP->dimension.push_back(7);
    cP->isSplit.push_back(false);
    cP->numCluster.push_back(10);
    cP->isUpdate.push_back(true);
    cP->weights.resize(1);
    for(int j=0;j<7;j++) cP->weights[0].add(1);
    cP->roiP.push_back(NULL);
    for(int j=0;j<tmpdata.size();j++)tmplist[0]=new int[tmpdata.size()];
    for(int j=0;j<tmpdata.size();j++)tmplist[0][j] = -1;
    cerr<<tmpdata.size()<<endl;
    int maxCluster=cluster->ComputeClusters((char*)ifname.c_str(),
                                           (char*)ofname.c_str(),
                                           (char*)clustername.c_str(),
                                           *cP,
                                           tmplist);
cerr<<"//===================Sampling===========================================//"<<endl;
    int count = 0;
    int *splitCount=new int[10];
    for(int j=0;j<splitData[i].size();j++){
       bool exist = false;
       for(int ii=0;ii<SYMMETRY_TYPE;ii++){
        if(mySymmetry->splitData[ii].size()==0) continue;
        if((*mySymmetry->symmetryList)[ii].count(splitData[i][j])>0){
          exist = true;
          svVector3 p = (*splitData[i][j]).pos-pos;
          svScalar d = dot(p, dir);
          if(d>0){
            int sampleid = splitCount[tmplist[0][count]]%10+1;
            sampleLabel.at(splitData[i][j])=sampleid;
            splitCount[tmplist[0][count]]++;count++;
            sampleLabel.at(((*(mySymmetry->symmetryList))[ii].at(splitData[i][j]))[0])=sampleid;
          }
        }
      }
      //cerr<<j<<" "<<count<<" "<<tmplist[0][count]<<" "<<splitCount[tmplist[0][count]]<<endl;
      if(!exit) {
         int sampleid = splitCount[tmplist[0][count]]%10+1;
         sampleLabel.at(splitData[i][j])=sampleid;
         splitCount[tmplist[0][count]]++;count++;
      }
  }
cerr<<"//==========================Release========================"<<endl;
  delete [] tmplist[0];
  delete [] tmplist;
  tmpdata.clear();
 }
cerr<<"done"<<endl;
}
/*void svRawClusterData::GenerateSampling(int splitlevel, char *dir, int frequency){

  char *ifname = new char[400];
  char *ofname = new char[400];
  sprintf(ifname,"%s/tmpdata.txt", dir);
  ofstream outfile(ifname);
  outfile<<splitData[splitlevel].size()<<endl;
  int maxsize = -1;
  for(int i=0;i<tmpdata.size();i++){
   outfile<<tmpdata[i].size()<<endl;
   int size = tmpdata[i].size();
   if(size>maxsize){
         maxsize = tmpdata[i].size();
   }
   for(int j=0;j<size;j++){
    outfile<<(*tmpdata[i][j]).pos[0]<<" "
           <<(*tmpdata[i][j]).pos[1]<<" "
           <<(*tmpdata[i][j]).pos[2]<<" "
           <<(*tmpdata[i][j]).dir[0]<<" "
           <<(*tmpdata[i][j]).dir[1]<<" "
           <<(*tmpdata[i][j]).dir[2]<<" "
           <<(*tmpdata[i][j]).den<<endl;
    }
  }
  outfile.close();
  sprintf(ofname,"%s/tmpdatanormalize.txt",dir);
  sprintf(clusterfname,"%s/cluster.txt",dir);

  svKmeans *cluster = new svKmeans();
  int **tmplist = new int*[mySlice->splitData.size()];
  //cerr<<mySlice->splitData.size()<<" "<<maxsize<<endl;
  //  for(int i=0;i<mySlice->splitData.size();i++) tmplist[i]=new int[maxsize];
  //    for(int i=0;i<tmpdata.size();i++)for(int j=0;j<maxsize;j++)tmplist[i][j]=-1;
  //      int maxCluster = cluster->ComputeClusters(ifname, ofname, clusterfname, *state->kP, tmplist);
  //        cerr<<"maxCluster "<<maxCluster<<endl;
  //          splitData.resize(maxCluster);//-1, 0, 1, 2, ...
  //            boundaryIndex.resize(maxCluster);
  //              SetClusterLabel(tmpdata, tmplist);
  //
}
*/
//========================================================================
svContourData::svContourData(svQDOT *qdot, svRawSliceData *data){
  myCluster = NULL;
  New(qdot, data);
  data_structure_type = d_slice;
//  contourcompute = new ContourTree();
}
void svContourData::New(svQDOT *qdot, svRawSliceData *data){
  myQDOT = qdot;
  myData = data;
  if(splitData.size()>0){cleandata();cleanup();}
  splitData.resize((myData->splitData).size());
  contourLines.resize((myData->splitData).size());
  contourTreeData.resize((myData->splitData).size());
  contourcompute.resize((myData->splitData).size());
  for(int i=0;i<myData->splitData.size();i++){
       isUpdate.push_back(true);
       contourcompute[i] = new ContourTree();
  }
  maxExp = -9e+9;
  minExp = 9e+9;
}
void svContourData::Init(){
  int size = splitData.size();
  isSelectable.resize(size);
  std::fill(isSelectable.begin(), isSelectable.end(), true);
  isVisible.resize(size);
  std::fill(isVisible.begin(), isVisible.end(), true);
  isHighlighted.resize(size);
  std::fill(isHighlighted.begin(), isHighlighted.end(), false);
  isSelected.resize(size);
  std::fill(isSelected.begin(), isSelected.end(), false);
/*  for(int i=0;i<myQDOT->qdotData.size();i++){
    state->sampleVisible.insert(std::pair<Spin *, bool>(&(myQDOT->qdotData[i]), true));
  }
*/
  //state->NewVisible(myQDOT);
}
void svContourData::GenerateContours(//ContourProperty &property,
                                     char *dir){
  for(int j=0;j<myData->splitData.size();j++){
      for(int i=0;i<state->cP->values[j].size();i++) {
        if(state->cP->isUpdate[i]){
           char *contourfile = new char[400];
           sprintf(contourfile,"%s/contour%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f%d%6.2e.txt",
                                dir,
                                myData->sliceMinCenter[0],
                                myData->sliceMinCenter[1],
                                myData->sliceMinCenter[2],
                                myData->sliceDir[0],
                                myData->sliceDir[1],
                                myData->sliceDir[2],
                                j, state->cP->values[j][i]);
           char *planename = new  char[200];
           sprintf(planename, "%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f",
                                myData->sliceMinCenter[0],
                                myData->sliceMinCenter[1],
                                myData->sliceMinCenter[2],
                                myData->sliceDir[0],
                                myData->sliceDir[1],
                                myData->sliceDir[2]);
           GenerateContour(contourfile, dir, planename,j,
                                state->cP->values[j][i]);
           SetContours(contourfile);
           sprintf(contourfile,"%s/contour2d%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f%d%6.2e.svl",
                                dir,
                                myData->sliceMinCenter[0],
                                myData->sliceMinCenter[1],
                                myData->sliceMinCenter[2],
                                myData->sliceDir[0],
                                myData->sliceDir[1],
                                myData->sliceDir[2],
                                j, state->cP->values[j][i]);
           GenerateContourLine(contourfile, dir, planename,j,
                                state->cP->values[j][i]);
           SetContourLines(contourfile, j);
           delete [] contourfile;
          }
       }
  }
  SplitData();
}
void svContourData::GenerateContourTree(char *dir){
  svContourTree *contourtree;
//  ContourTreeProperty property;
   int seed_num = (myData->splitData).size();
//  property.dir = new char[400];
//  property.planename = new char[100];
  svVector3 minCenter = myData->sliceMinCenter;
  svVector3 sliceDir  = myData->sliceDir;
//  sprintf(property.dir, "%s", dir);
  char *planename = new char[100];
  sprintf( planename, "%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f",
           minCenter[0], minCenter[1], minCenter[2],
           sliceDir[0], sliceDir[1], sliceDir[2]);
  contourtree = new svContourTree();
  contourtree->ComputeContourTree(dir, planename, (myData->splitData).size());//property);
  for(int i=0;i<seed_num;i++){
      char *str = new char[800];
      sprintf(str,"%s/contourtree%s%d.txt", dir, planename,i);
      contourcompute[i]->Construct(str);
      contourcompute[i]->PruneVolume();
      contourcompute[i]->GenerateIsoValues();
  }
  delete [] planename;
//  delete [] property.dir;
  GenerateContourTreeContours(dir);
}
void svContourData::GenerateContourTreeContours(char *dir){
  svVector3 minCenter = myData->sliceMinCenter;
  svVector3 sliceDir  = myData->sliceDir;
  for(int i=0;i<myData->splitData.size();i++){
      char *str = new char[400];
      int size = contourcompute[i]->GetNodeSize();
      vector<svScalar> values = contourcompute[i]->GetIsoValues();
      size = values.size();
      svScalar maxDen = myData->sliceDen[i];
      if(size==1) {values.clear(); values.push_back(0.5 * maxDen);}
      for(int j=0;j<size;j++){
           char *contourfile = new char[400];
           sprintf(contourfile, "%s/contour2d%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f%d%6.2e.txt",
                   dir,
                   minCenter[0], minCenter[1], minCenter[2],
                   sliceDir[0], sliceDir[1], sliceDir[2],
                   i, values[j]);
           char *planename = new char[100];
           sprintf(planename, "%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f",
              minCenter[0], minCenter[1], minCenter[2],
              sliceDir[0], sliceDir[1], sliceDir[2]);
//           cerr<<contourfile<<endl;
           GenerateContourLine(contourfile, dir, planename, i, values[j]);
           SetContourTreeData(contourfile,i);
           delete [] contourfile;
           delete [] planename;
      }
      delete [] str;
      values.clear();
  }
}
void svContourData::GenerateContoursByContourTree(char *dir,
                                     int step,
                                     bool ratio){
  /*svContourTree *contourtree;
  ContourTreeProperty property;
  property.seed_num = (myData->splitData).size();
  property.dir = new char[200];
  strcpy(property.dir, dir);
  property.planename = new char[100];
  svVector3 minCenter = myData->sliceMinCenter;
  svVector3 sliceDir  = myData->sliceDir;
  sprintf( property.planename, "%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f",
           minCenter[0], minCenter[1], minCenter[2],
           sliceDir[0], sliceDir[1], sliceDir[2]);
  contourtree = new svContourTree();
  contourtree->ComputeContourTree(property);
  */
  svVector3 minCenter = myData->sliceMinCenter;
  svVector3 sliceDir  = myData->sliceDir;
  char *planename = new char[100];
  sprintf(planename, "%0.2f%0.2f%0.2f%0.2f%0.2f%0.2f",
              minCenter[0], minCenter[1], minCenter[2],
              sliceDir[0], sliceDir[1], sliceDir[2]);
  for(int i=0;i<myData->splitData.size();i++){
      char *str = new char[400];
//      ContourTree *contourcompute = new ContourTree();
//      sprintf(str,"%s/contourtree%s%d.txt", property.dir, property.planename,i);
//      contourcompute->Construct(str);
//      contourcompute->PruneVolume();
//      contourcompute->GenerateIsoValues();
      int size = contourcompute[i]->GetNodeSize();
      vector<svScalar> values = contourcompute[i]->GetIsoValues();
      svScalar maxDen = myData->sliceDen[i];
      if(values.size()==1){
          values.clear();
          if(step>0){
          svScalar degree = 1./(float)(step);
          for(int t=1;t<=step;t++){
             svScalar v= degree * (float)t;
             v = v * maxDen; values.push_back(v);
          }
          values.push_back(0.1 * maxDen);
          values.push_back(0.9 * maxDen);
          }
          else
             values.push_back(0.5 * maxDen);
      }
      else{
        for(int j=0;j<size;j++){
          svScalar degree = (values[j+1]/maxDen-values[j]/maxDen)/(step+1);
          for(int t=1;t<step+1;t++){
            svScalar v= values[j]/maxDen + (svScalar)t * degree;
            v = v * maxDen;
            values.push_back(v);
          }
        }
      }
      size = values.size();
      for(int j=0;j<size;j++){
        if(values[j]>0){
             svScalar v = values[j];
             char *contourfile = new char[400];
             sprintf(contourfile, "%s/contour%s%d%6.2e.txt",
                   dir, planename, i, v);
//             cerr<<contourfile<<endl;
             GenerateContour(contourfile, dir, planename, i, v);
             SetContours(contourfile);
             sprintf(contourfile, "%s/contour2d%s%d%6.2e.txt",
                   dir, planename, i, v);
             GenerateContourLine(contourfile, dir, planename, i, v);
             SetContourLines(contourfile,i);
             delete [] contourfile;
        }
      }
      sort(values.begin(), values.end());
      /*size = values.size();
      for(int j=0;j<size-1;j++){
        svScalar degree = (values[j+1]/maxDen-values[j]/maxDen)/(step+1);
        for(int t=0;t<=(step+1);t++){
           svScalar v= values[j]/maxDen + (svScalar)t * degree;
           v = v * maxDen;
           if(v>0){
             char *contourfile = new char[400];
             sprintf(contourfile, "%s/contour2d%s%d%6.2e.txt",
                   dir, planename, i, v);
             GenerateContour(contourfile, dir, planename, i, v);
             SetContours(contourfile);
             GenerateContourLine(contourfile, dir, planename, i, v);
             SetContourLines(contourfile,i);
             delete [] contourfile;
           }
        }
      }*/
      values.clear();
      delete [] str;
  }
 // delete contourtree;
 // delete field;
  SplitData();
  //cerr<<maxExp<<" "<<minExp<<endl;
}
void svContourData::GenerateContour(char *contourfile, char *vtkdir, char *planename, int layer, float contourValue){
       ifstream infile(contourfile);
       if(infile.is_open()){infile.close();return;}
       char *vtkName = new char[200];
       sprintf(vtkName, "%s/%s%d.vtk", vtkdir, planename, layer);
       svContour *contourField = new svContour();
       contourField->ComputeContours(vtkName, contourfile, contourValue);
       delete contourField;
       delete [] vtkName;
}
void svContourData::GenerateContourLine(char *contourfile, char *vtkdir, char *planename, int layer, float contourValue){
    ifstream infile(contourfile);
    if(infile.is_open()){infile.close();return;}
    char *vtkName = new char[200];
    sprintf(vtkName, "%s/%s%d.vtk", vtkdir, planename, layer);
    svContour *contourField = new svContour();
    contourField->ComputeContourLines(vtkName, contourfile, contourValue);
    delete contourField;
    delete [] vtkName;
}
void svContourData::SetContours(char *contourfile){
  ifstream infile(contourfile);
  int num;
  infile>>num;
  for(int i=0;i<num;i++){
    Spin tmpspin;
    infile>>tmpspin.pos[0]>>tmpspin.pos[1]>>tmpspin.pos[2]
          >>tmpspin.dir[0]>>tmpspin.dir[1]>>tmpspin.dir[2]
          >>tmpspin.den;
    if(tmpspin.den>1e-20){
     tmpspin.exp = getNumOfIntegerDigits(tmpspin.den);
     tmpspin.coe = tmpspin.den/pow(10., tmpspin.exp);
     if(tmpspin.exp>maxExp) maxExp = tmpspin.exp;
     if(tmpspin.exp<minExp) minExp = tmpspin.exp;
    }
    else{
     tmpspin.exp = 0;
    tmpspin.coe = 0;}// tmpspin.den/pow(10., tmpspin.exp);
    tmpspin.dir.normalize();
    tmpspin.xangle = acos(tmpspin.dir[0])*180./PI;
    tmpspin.yangle = acos(tmpspin.dir[1])*180./PI;
    tmpspin.zangle = acos(tmpspin.dir[2])*180./PI;
    contourData.push_back(tmpspin);
    //splitData[layer].push_back(&(contourData.at(contourData.size()-1)));
  }
  infile.close();
}

void svContourData::SetContourLines(char *svlfile, int layer){
    ifstream infile(svlfile);
    int n;
    infile>>n;
    for(int i=0;i<n;i++){
         int m;
         infile>>m;
         for(int j=0;j<m;j++){
            Spin tmpspin;
            infile>>tmpspin.pos[0]>>tmpspin.pos[1]>>tmpspin.pos[2];
            svVector3 pos = tmpspin.pos;
            tmpspin.pos[0] = GetDot(pos  - myData->sliceOrigin[layer], myData->sliceAxis[0]);
            tmpspin.pos[1] = GetDot(pos - myData->sliceOrigin[layer], myData->sliceAxis[1]);
            tmpspin.pos[2] = GetDot(pos - myData->sliceOrigin[layer], myData->sliceAxis[2]);
            //     >>tmpspin.dir[0]>>tmpspin.dir[1]>>tmpspin.dir[2]
            //     >>tmpspin.den;
           //if(tmpspin.den>1e-20){
           //tmpspin.exp = getNumOfIntegerDigits(tmpspin.den);
           //tmpspin.coe = tmpspin.den/pow(10., tmpspin.exp);}
           //else{
           //  tmpspin.exp = 0;
           //  tmpspin.coe = 0;
           //}// tmpspin.den/pow(10., tmpspin.exp);
           contourLines[layer].push_back(tmpspin);
         }
    }
    isUpdate[layer] = false;
    infile.close();
}
void svContourData::SetContourTreeData(char *svlfile, int layer){
    ifstream infile(svlfile);
    int n;
    infile>>n;
    for(int i=0;i<n;i++){
         int m;
         infile>>m;
         for(int j=0;j<m;j++){
            Spin tmpspin;
            infile>>tmpspin.pos[0]>>tmpspin.pos[1]>>tmpspin.pos[2];
                 //>>tmpspin.dir[0]>>tmpspin.dir[1]>>tmpspin.dir[2]
                 //>>tmpspin.den;
           svVector3 pos = tmpspin.pos;
           tmpspin.pos[0] = GetDot(pos  - myData->sliceOrigin[layer], myData->sliceAxis[0]);
           tmpspin.pos[1] = GetDot(pos - myData->sliceOrigin[layer], myData->sliceAxis[1]);
           tmpspin.pos[2] = GetDot(pos - myData->sliceOrigin[layer], myData->sliceAxis[2]);
           if(tmpspin.den>1e-20){
           tmpspin.exp = getNumOfIntegerDigits(tmpspin.den);
           tmpspin.coe = tmpspin.den/pow(10., tmpspin.exp);}
           else{
             tmpspin.exp = 0;
             tmpspin.coe = 0;
           }// tmpspin.den/pow(10., tmpspin.exp);
           contourTreeData[layer].push_back(tmpspin);
         }
    }
    infile.close();
}
void svContourData::SplitData(){
  int count = 0;
  for(int i=0;i<contourData.size();i++){
     Spin tmpspin = contourData.at(i);
     svVector3 tmp = tmpspin.pos - myData->sliceMinCenter;
     double dot = GetDot(tmp, myData->sliceDir);
//     if((fmod(dot, (double)myData->sliceDistance)<1e-5)
//        || (myData->sliceDistance-fmod(dot, (double)myData->sliceDistance)<1e-5)){
        int index = (int)(dot/myData->sliceDistance);
        //cerr<<i<<" "<<index<<" "<<dot<<" "<<myData->sliceDistance<<" "<<tmp[0]<<" "<<tmp[1]<<" "<<tmp[2]<<endl;
        splitData[index].push_back(&(contourData.at(i)));
        count++;
 //    }
  }
//  cerr<<splitData.size()<<endl;
  //cout<<contourData.size()<<" "<<count<<endl;
}
//TODO:NEED TO BE FIXED
void svContourData::GenerateSampling(int frequency){
  state->qdotVisible.clear();

  for(int i=0;i<splitData.size();i++){
     for(int j=0;j<splitData[i].size();j++){
        if(j%frequency==0){
          state->qdotVisible.insert(std::pair<Spin *, bool>(splitData[i][j], true));
//at(splitData[i][j]) = true;
        }
        else state->qdotVisible.insert(std::pair<Spin *, bool>(splitData[i][j], false));
//at(splitData[i][j]) = false;
     }
  }
//  if(myCluster!=NULL) myCluster->visibleLabel = visibleLabel;
}

void svContourData::cleanup(){
 isUpdate.clear();
 contourData.clear();
 for(int i=0;i<contourLines.size();i++){
    contourLines[i].clear();
    contourTreeData[i].clear();
 }
 contourLines.clear();
 contourTreeData.clear();
}
//=============================================================//
//
//
//====================contour cluster============================//
svContourClusterData::svContourClusterData(svQDOT *qdot, svContourData *data){
  clusterList = new map<Spin *, int>();
  New(qdot, data);
  data_structure_type = d_cluster;
}
void svContourClusterData::New(svQDOT *qdot, svContourData *data){
  myQDOT = qdot;
  myData = data;
  (*clusterList).clear();
  if(splitData.size()>0){cleandata();}
  for(int i=0;i<myData->contourData.size();i++)
     (*clusterList).insert(std::pair<Spin *, int>(&(myData->contourData[i]), -1));
}
void svContourClusterData::ResetCluster(){
  for(int i=0;i<myData->contourData.size();i++)
     (*clusterList).at(&(myData->contourData[i])) = -1;
  for(int i=0;i<splitData.size();i++) splitData[i].clear(); splitData.clear();
}
void svContourClusterData::Init(char *folder){//,
//                                            KmeansProperty & property){
  GenerateClusters(folder);//, property);
  int size = splitData.size();
  isSelectable.resize(size);
  std::fill(isSelectable.begin(), isSelectable.end(), true);
  isVisible.resize(size);
  std::fill(isVisible.begin(), isVisible.end(), true);
  isHighlighted.resize(size);
  std::fill(isHighlighted.begin(), isHighlighted.end(), false);
  isSelected.resize(size);
  std::fill(isSelected.begin(), isSelected.end(), false);
//  state->NewVisible(myQDOT);
}

void svContourClusterData::GenerateClusters(char *folder){
                             //               KmeansProperty & property){
  char *clusterfname = new char[400];;
  GenerateCluster(folder, clusterfname);//property
  //SetClusterLabel(clusterfname);
  delete [] clusterfname;
}
void svContourClusterData::GenerateCluster(//KmeansProperty & property,
                                       char *dir, char *clusterfname){
//  splitData.resize(state->kP->numCluster[0]);//TODO FIX
  char *ifname = new char[400];
  char *ofname = new char[400];
  sprintf(ifname,"%s/tmpdata.txt", dir);
  ofstream outfile(ifname);
  outfile<<myData->splitData.size()<<endl;
  int max = 0;
  for(int i=0;i<myData->splitData.size();i++){
     outfile<<myData->splitData[i].size()<<endl;
     if(myData->splitData[i].size()>max) max = myData->splitData[i].size();
     for(int j=0;j<myData->splitData[i].size();j++){
       outfile<<(*myData->splitData[i][j]).pos[0]<<" "
              <<(*myData->splitData[i][j]).pos[1]<<" "
              <<(*myData->splitData[i][j]).pos[2]<<" "
              <<(*myData->splitData[i][j]).dir[0]<<" "
              <<(*myData->splitData[i][j]).dir[1]<<" "
              <<(*myData->splitData[i][j]).dir[2]<<" "
              <<(*myData->splitData[i][j]).den<<endl;
     }
  }
/*
  for(int i=0;i<myData->contourData.size();i++){
    outfile<<(myData->contourData[i]).pos[0]<<" "
           <<(myData->contourData[i]).pos[1]<<" "
           <<(myData->contourData[i]).pos[2]<<" "
           <<(myData->contourData[i]).dir[0]<<" "
           <<(myData->contourData[i]).dir[1]<<" "
           <<(myData->contourData[i]).dir[2]<<" "
           <<(myData->contourData[i]).den<<endl;
  }
*/
  outfile.close();
  int **tmplist = new int*[myData->splitData.size()];
  for(int i=0;i<myData->splitData.size();i++) tmplist[i] = new int[max];
  for(int i=0;i<myData->splitData.size();i++)
     for(int j=0;j<max;j++)tmplist[i][j] = -1;
  sprintf(ofname,"%s/tmpdatanormalize.txt",dir);
  sprintf(clusterfname,"%s/cluster.txt",dir);
  svKmeans *cluster = new svKmeans;
  //cluster->Normalization(ifname, ofname, property.dimension,
  //                       myData->contourData.size(), property.clusterWeight);
  //cluster->ComputeClusters(ofname, clusterfname, property.dimension,
  //                       myData->contourData.size(), property.numCluster);
  int maxCluster=cluster->ComputeClusters(ifname, ofname, clusterfname, *state->kP, tmplist);
  splitData.resize(maxCluster);//-1, 0, 1, 2, ...
  SetClusterLabel(tmplist);
  delete cluster;
  char *exe = new char[1024];
  sprintf(exe, "rm %s", ofname); system(exe);
  sprintf(exe, "rm %s", ifname); system(exe);
  sprintf(exe, "rm %s", clusterfname); system(exe);
  delete [] exe;
}
void svContourClusterData::SetClusterLabel(int **tmplist){
  int count = 0;
  for(int i=0;i<myData->splitData.size();i++){
    int tmpcount = 0;
    for(int j=0;j<myData->splitData[i].size();j++){
        (*clusterList).at(myData->splitData[i][j]) = tmplist[i][j];
        if(tmplist[i][j]==-1) splitData[0].push_back(myData->splitData[i][j]);
        else {
          if(state->kP->isSplit[0]){
            splitData[tmplist[i][j]+count+1].push_back(myData->splitData[i][j]);
          }
          else{
            splitData[tmplist[i][j]+1].push_back(myData->splitData[i][j]);
          }
          if(tmplist[i][j] > tmpcount) tmpcount = tmplist[i][j];
        }
     }
     count = tmpcount + 1;
  }
/*
  for(int i=0;i<myData->contourData.size();i++){
    int index = clusterList.at(&myData->contourData[i]);
    //cerr<<i<<" "<<index<<endl;
    splitData[index+1].push_back(&myData->contourData[i]);
  }*/
}
/*
void svContourClusterData::SetClusterLabel(char *file){
  vector<int> cluster;
  ifstream infile(file);
  int c;
  while(infile>>c){cluster.push_back(c);}
  infile.close();
  for(int j=0;j<myData->contourData.size();j++){
     clusterList.at(&myData->contourData[j]) = cluster[j];
  }
  for(int i=0;i<myData->contourData.size();i++){
    int index = clusterList.at(&myData->contourData[i]);
    splitData[index].push_back(&myData->contourData[i]);
  }
  cluster.clear();
}
void svContourClusterData::cleanup(){}//clusterList.clear();}
*/
//===============================================================//
}

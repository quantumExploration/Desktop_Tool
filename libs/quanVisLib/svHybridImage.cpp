#include "svHybridImage.h"
#include <fstream>
#include <string.h>
#include <sstream>

using namespace std;

namespace __svl_lib {

svHybridImage::svHybridImage():svImage(){

}

void svHybridImage::GenerateHybridImage(string ifname1, string ifname2,
                                        float cutoff, string ofname){
//                                        int sliceIndex){
  //cout<<ifname1<<endl;
  //cout<<ifname2<<endl;
  //cout<<ofname<<endl;
  //cerr<<sliceIndex<<endl;
  SaveImage(ifname1);//, sliceIndex);
  SaveContours(ifname2);//, sliceIndex);
  string bin(BIN_DIR);
  stringstream stream;
  stream<<cutoff;

  string exe = bin+ "/hybridImage" + " " + ifname1
               + " " + ifname2 + " " + stream.str() + " " + ofname;
  system(exe.c_str());
  //cerr<<exe<<endl;
  ReadColors(ofname);//, sliceIndex);
/*
  ifstream infile(offile);
  int s1, s2;
  infile>>s1>>s2;
  int dataCount = 0;
  for(int j=0;j<s1;j++){
    for(int t=0;t<s2;t++){
      infile>>color[dataCount][0]
           >> color[dataCount][1]
           >> color[dataCount][2];
      dataCount++;
    }
  }
  infile.close();*/

}

void svHybridImage::SaveImage(string ofname){//, int sliceIndex){
  ofstream outfile(ofname.c_str());
  outfile<<1<<endl;
  outfile<<row<<" "<<column<<endl;
  int i = sliceIndex;
  for(int r=0;r<myData->imageRow;r++){
      for(int c=0;c<myData->imageColumn;c++){
         int j = r * myData->imageColumn + c;
         j = myData->imageData[i][j];
         if(j<0)
          outfile<<255<<" "<<255<<" "<<255<<endl;
         else
          outfile<<(int)(myData->sliceData[i][j].icolor[0]*255)<<" "
            <<(int)(myData->sliceData[i][j].icolor[1]*255)<<" "
            <<(int)(myData->sliceData[i][j].icolor[2]*255)<<endl;
      }
  }
  outfile.close();
}

void svHybridImage::SaveContours(string ofname){//, int sliceIndex){
  ofstream outfile(ofname.c_str());
  svVector3Array tmpColor;
  outfile<<1<<endl;
  svVector3 white;
  svVector3 black;
  svVector3 red;
  red[0]=255;red[1]=0;red[2]=0;
  black[0]=0;black[1]=0;black[2]=0;
  white[0]=255;white[1]=255;white[2]=255;
  row = myData->imageRow;
  column = myData->imageColumn;
  outfile<<row<<" "<<column<<endl;
  for(int j=0;j<row;j++){
      for(int t=0;t<column;t++){
          tmpColor.add(white);
      }
  }
  int i = sliceIndex;
//  cerr<<myContour->contourTreeData[sliceIndex].size()<<" "<<myContour->contourLines[sliceIndex].size()<<endl;
  //svScalar minD = min(myData->iminD[0], myData->iminD[1]);
  for(int j=0;j<myContour->contourTreeData[sliceIndex].size();j+=2){
      svVector3 p1 = myContour->contourTreeData[sliceIndex][j].pos;
      int x = (p1[0] - myData->ilbbox[0])/iminD[0];
      int y = (p1[1] - myData->ilbbox[1])/iminD[1];
      //if(sliceIndex==40)cerr<<myContour->contourTreeData[sliceIndex].size()<<" "<<p1[0]<<" "<<p1[1]<<endl;
      if(x>=column) x = column - 1;
      if(y>=row) y =row-1;
      svVector3 p2 = myContour->contourTreeData[sliceIndex][j+1].pos;
      svVector3 p = (p1+p2)/(svScalar)2.;
      x = (p[0] - myData->ilbbox[0])/iminD[0];
      y = (p[1] - myData->ilbbox[1])/iminD[1];
      if(x>=column) x = column - 1;
      if(y>=row) y =row-1;
      tmpColor[x + y * column ]=black;
      x = (p2[0] - myData->ilbbox[0])/iminD[0];
      y = (p2[1] - myData->ilbbox[1])/iminD[1];
      if(x>=column) x = column - 1;
      if(y>=row) y =row-1;
      tmpColor[x + y * column ]=black;
  }
  for(int j=0;j<myContour->contourLines[sliceIndex].size();j+=2){
    svVector3 p1 = myContour->contourLines[sliceIndex][j].pos;
    int x = (p1[0] - myData->ilbbox[0])/iminD[0];
    int y = (p1[1] - myData->ilbbox[1])/iminD[1];
    if(x>=column) x = column - 1;
    if(y>=row) y =row-1;
    tmpColor[x + y * column ]=red;
    svVector3 p2 = myContour->contourLines[sliceIndex][j+1].pos;
    svVector3 p = (p1+p2)/(svScalar)2.;
    x = (p[0] - myData->ilbbox[0])/iminD[0];
    y = (p[1] - myData->ilbbox[1])/iminD[1];
    if(x>=column) x = column - 1;
    if(y>=row) y =row-1;
    tmpColor[x + y * column ]=red;
    x = (p2[0] - myData->ilbbox[0])/iminD[0];
    y = (p2[1] - myData->ilbbox[1])/iminD[1];
    if(x>=column) x = column - 1;
    if(y>=row) y =row-1;
    tmpColor[x + y * column ]=red;
  }
  for(int j=0;j<tmpColor.size();j++){
       outfile<<tmpColor[j][0]<<" "<<tmpColor[j][1]<<" "<<tmpColor[j][2]<<endl;
  }
 outfile.close();
 tmpColor.free();
}

void svHybridImage::ReadColors(string ofname){//, int sliceIndex){
  ifstream infile(ofname.c_str());
  int n; infile>>n;
  int rr,cc;
  infile>>rr>>cc;
  int i = sliceIndex;
  for(int r=0;r<myData->imageRow;r++){
      for(int c=0;c<myData->imageColumn;c++){
         int j = r * myData->imageColumn + c;
         j = myData->imageData[i][j];
         if(j>=0){
          infile>>myData->sliceData[i][j].icolor[0]
                >>myData->sliceData[i][j].icolor[1]
                >>myData->sliceData[i][j].icolor[2];
          myData->sliceData[i][j].icolor[0]/=255.;
          myData->sliceData[i][j].icolor[1]/=255.;
          myData->sliceData[i][j].icolor[2]/=255.;
        }
        else{
          svVector3 tmpc;
          infile>>tmpc[0]>>tmpc[1]>>tmpc[2];
        }
      }
  }
  infile.close();
}
/*
void svHybridImage::Render2D(){
  glPushMatrix();
  glTranslatef(tranx, trany, 0);
  if(isRotatable)glMultMatrixf(rotateM);
  glScalef(scale, scale, scale);
  int index = 0;
  glBegin(GL_QUADS);
  for(int i=0;i<row;i++){
    for(int j=0;j<column;j++){
       glColor3f(image[index].icolor[0],image[index].icolor[1],image[index].icolor[2]);
       glVertex2f(image[index].ipos[0], image[index].ipos[1]);
       glVertex2f(image[index].ipos[0]+iminD[0], image[index].ipos[1]);
       glVertex2f(image[index].ipos[0]+iminD[0], image[index].ipos[1]+iminD[1]);
       glVertex2f(image[index].ipos[0], image[index].ipos[1]+iminD[1]);
       index++;
    }
  }
  glEnd();
  if(isHighlighted){
    glColor3f(253./255., 174./255., 97./255.);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0,0);
    glVertex2f(sidex, 0);
    glVertex2f(sidex, sidey);
    glVertex2f(0,sidey);
    glEnd();
  }
  glPopMatrix();
}

void svHybridImage::Render3D(){
  for(){

  }
}
*/
}

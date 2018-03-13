#include "svImage.h"
#include <fstream>
#include <string.h>

using namespace std;

namespace __svl_lib {

svImage::svImage(){
  isSelectable = true;
  isSelected = false;//true;//false;
  isHighlighted = false;
  is3D  = false;
  isRotatable = false;
}

void svImage::New(){
  row = myData->imageRow;
  column = myData->imageColumn;
}

void svImage::GenerateDisplay(int list2D, int list3D)
{
  if(glIsList(list2D))
     glDeleteLists(list2D, 1);
//  cout<<list2D<<" "<<list3D<<endl;
  displaylist2D = list2D;
  glNewList(displaylist2D, GL_COMPILE);
  RenderPixel();
  glEndList();

  if(glIsList(list3D))
     glDeleteLists(list3D, 1);
  displaylist3D = list3D;
  glNewList(displaylist3D, GL_COMPILE);
  Render3DList();
  glEndList();
}

void svImage::Render2D(){//int sliceIndex){
//  if(isRotatable) RenderBar();
//  else RenderPixel();//sliceIndex);
//  cerr<<displaylist2D<<endl;
  glCallList(displaylist2D);
}

void svImage::Render3D()
{
  glCallList(displaylist3D);
}

void svImage::Render3DList(){//int sliceIndex){
  if(myData->sliceData[sliceIndex].size()<=0) return;
  svVector3 dir = myData->sliceDir;
//  int jj = myData->imageData[sliceIndex][0];
//  svVector3 pos = (*(myData->splitData[sliceIndex][myData->sliceData[sliceIndex][jj].torindex])).pos;
  //rotate from (0,0,1) to dir
//  double angle_x, angle_z;
//  GetRotateAngle(dir, angle_x, angle_z);
  //render
//  glPushMatrix();
//  glTranslatef(pos[0], pos[1], pos[2]);
//  glRotatef(angle_z,0,0,1);
//  glRotatef(angle_x,1,0,0);
//  cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<" ";
  int i = sliceIndex;
  float x = 0;
  float y = 0;
//  svScalar minD = min(myData->iminD[0], myData->iminD[1]);
//  cout<<minD<<endl;
  glBegin(GL_QUADS);
  for(int r=0;r<myData->imageRow;r++){
      for(int c=0;c<myData->imageColumn;c++){
         int j = r * myData->imageColumn + c;
         j = myData->imageData[i][j];
         if(j<0)
          glColor3f(1,1,1);
         else
          glColor3f(myData->sliceData[i][j].icolor[0],
                  myData->sliceData[i][j].icolor[1],
                  myData->sliceData[i][j].icolor[2]);
         glNormal3f(dir[0],dir[1],dir[2]);
         //glBegin(GL_QUADS);
         glVertex3f(x,y,0);
         glVertex3f(x+myData->iminD[0],y,0);
         glVertex3f(x+myData->iminD[0],y+myData->iminD[1],0);
         glVertex3f(x,y+myData->iminD[1],0);
         //glEnd();
         x = x+myData->iminD[0];
      }
      x = 0;
      y = y+myData->iminD[1];
  }
  glEnd();
/*
  if(isSelected){
    glDisable(GL_LIGHTING);
    glColor3f(253./255., 174./255., 97./255.);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0);
    glVertex3f(sidex, 0);
    glVertex3f(sidex, sidey);
    glVertex3f(0,sidey);
    glEnd();
    glEnable(GL_LIGHTING);
  }
*/
//  glPopMatrix();
}

void svImage::RenderPixel(){///int sliceIndex){
  if(myData->sliceData[sliceIndex].size()<=0) return;

  //glPushMatrix();
  //glTranslatef(tranx, trany, 0);
  //glScalef(scale, scale, 1);
  //cerr<<tranx<<" "<<trany<<" "<<scale<<" "<<endl;
  //svScalar minD = min(myData->iminD[0], myData->iminD[1]);
  svVector3 iminD= myData->iminD;
  glBegin(GL_QUADS);
  float x =0;
  float y =0;
  int i = sliceIndex;
  for(int r=0;r<myData->imageRow;r++){
      for(int c=0;c<myData->imageColumn;c++){
         int j = r * myData->imageColumn + c;
         j = myData->imageData[i][j];
         if(j<0)
          glColor4f(1,1,1,0.25);//alpha);
         else
         {
         bool visible = (*myData->qdotVisible).at(myData->splitData[i][myData->sliceData[i][j].torindex]);
         float alpha = visible ? 1 : 0.25;
          glColor4f(myData->sliceData[i][j].icolor[0],
                  myData->sliceData[i][j].icolor[1],
                  myData->sliceData[i][j].icolor[2], alpha);
         }
         glVertex2f(x,y);
         glVertex2f(x+iminD[0],y);
         glVertex2f(x+iminD[0],y+iminD[1]);
         glVertex2f(x,y+iminD[1]);
         //cerr<<x<<" "<<y<<" "<<minD<<endl;
         x += iminD[0];
      }
      x = 0;
      y += iminD[1];
  }
  glEnd();
/*  glPopMatrix();

  glPushMatrix();
  glTranslatef(tranx, trany, 0);
  if(isHighlighted || isSelected){
    glLineWidth(4.);
    glColor3f(253./255., 174./255., 97./255.);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0,0);
    glVertex2f(sidex, 0);
    glVertex2f(sidex, sidey);
    glVertex2f(0,sidey);
    glEnd();
    glLineWidth(1.);
  }
  glPopMatrix();
*/
}

void svImage::ReadColors(string dir, string file){//, int sliceIndex){
  string ofname = dir + file;
  ifstream infile(ofname.c_str());
  infile>>row>>column;
  int i = sliceIndex;
  for(int r=0;r<myData->imageRow;r++){
      for(int c=0;c<myData->imageColumn;c++){
         int j = r * myData->imageColumn + c;
         j = myData->imageData[i][j];
         if(j>0)
          infile>>myData->sliceData[i][j].icolor[0]
                >>myData->sliceData[i][j].icolor[1]
                >>myData->sliceData[i][j].icolor[2];
      }
  }
  infile.close();
}

void svImage::SaveImage(string dir, string file){//, int sliceIndex){
  string ofname = dir + file;
  ofstream outfile(ofname.c_str());
  outfile<<row<<" "<<column<<endl;
  int i = sliceIndex;
  for(int r=0;r<myData->imageRow;r++){
      for(int c=0;c<myData->imageColumn;c++){
         int j = r * myData->imageColumn + c;
         j = myData->imageData[i][j];
         if(j<0)
          outfile<<1<<" "<<1<<" "<<1<<endl;
         else
          outfile<<myData->sliceData[i][j].icolor[0]<<" "
            <<myData->sliceData[i][j].icolor[1]<<" "
            <<myData->sliceData[i][j].icolor[2]<<endl;
      }
  }
  outfile.close();
}

void svImage::cleanup(){
  //image.clear();
}

}

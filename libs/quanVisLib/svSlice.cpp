#include "svSlice.h"
#include <fstream>
#include <string.h>

using namespace std;

namespace __svl_lib {
/*
svVector3 sliceDirection[][]={
{0,0,1},
{0,1,0},
{1,3,0},
{1,1,0},
{3,1,0},
{1,0,0},
{3,-1,0},
{1,-1,0},
{1,-3,0}};

svSlice::svSlice(svRawQDOTData *d){
  seed_num = 0;
  New(d);
}
svSlice::~svSlice(){
  cleanup();
}

*/
void svSlice::RenderSlice(svScalar side, svVector3 origin,
                          svVector3 dir, svVector3 x, svVector3 y)
{
   glDisable(GL_LIGHTING);
   glDisable(GL_LIGHT0);
   glDisable(GL_LIGHT1);
   glColor3f(253./255., 174./255., 97./255.);
   glPushMatrix();
   glTranslatef(origin[0], origin[1], origin[2]);
   double rotate_x, rotate_z;
   GetRotateAngle(dir, rotate_x, rotate_z); 
   glRotatef(rotate_z/PI*180.,0,0,1);
   glRotatef(rotate_x/PI*180.,1,0,0);
//   cout<<side<<" "<<rotate_z/PI*180.<<" "<<rotate_x/PI*180.<<" "<<origin[0]<<" "<<origin[1]<<" "<<origin[2]<<endl;
   glBegin(GL_LINE_LOOP);
   glVertex3f(0,0,0);
   glVertex3f(side,0,0);
   glVertex3f(side,side,0);
   glVertex3f(0,side,0);
   glEnd();
   glPopMatrix();

   glPushMatrix();
   //glTranslatef(origin[0], origin[1], origin[2]);
   glColor3f(1,0,0);
   glLineWidth(4.);
   glBegin(GL_LINES);
   glVertex3f(0,0,0);
   glVertex3f(x[0]*side, x[1]*side, x[2]*side);
   glEnd();
   glColor3f(0,1,0);
   glBegin(GL_LINES);
   glVertex3f(0,0,0);
   glVertex3f(y[0]*side, y[1]*side, y[2]*side);
   glEnd();
   glColor3f(0,0,1);
   glBegin(GL_LINES);
   glVertex3f(0,0,0);
   glVertex3f(dir[0]*side, dir[1]*side, dir[2]*side);
   glEnd();
   glLineWidth(1.);
   glPopMatrix();
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
}
                           
/*
void svSlice::RenderSlice(){
  svVector3 c(0,0,0);
  for(int i=0;i<planePos.size();i++){
     c = c + planePos[i];
  } 
  glColor3f(0.5,0.5,0.5);
  glDisable(GL_LIGHTING);
  glBegin(GL_LINE_LOOP);
  for(int i=0;i<planePos.size();i++){
    glVertex3f(planePos[i][0], planePos[i][1], planePos[i][2]);
  }
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(c[0],c[1],c[2]);
  glVertex3f(widgetPos[0], widgetPos[1], widgetPos[2]);
  glEnd();
  glEnable(GL_LIGHTING);
  if(selectPlane){
    glBegin(GL_POLYGON);
    glNormal3f(direction[0], direction[1], direction[2]);
    for(int i=0;i<planePos.size();i++){
      glVertex3f(planePos[i][0], planePos[i][1], planePos[i][2]);
    }
    glEnd();
    if(selectWidget) glColor3f(253./255., 174./255., 97./255.,0.5);
    glPushMatrix();
    glTranslatef(widgetPos[0], widgetPos[1], widgetPos[2]);
    glutSolidSphere(1,6,6);
    glEnd();
  }
}
void svSlice::New(svRawQDOTData *d){
   if(seed_num>0) clean();
   this->myData = d;
   seed_num = myData->GetSeed();
   elementColors = new svVector4Array[seed_num];
   for(int i=0;i<seed_num;i++){
      validSlice.add(i);
   }
   myData->GetPhysicalDimension(&lbbox, &rbbox);
   currentSlice = -1;
   currentElement[0]=-1; currentElement[1]=-1;
   minCenter = myData->GetMinPlane();//myData->GetCenter();
   center[0]=0;center[1]=0;center[2]=0;//hard code
   direction = myData->GetPlaneDir();//sliceDirection[0];
   distance = myData->GetPlaneDistance();

   SetPosition();
}
void svSlice::SetPosition(){//hard code!!!
   svVector3 tmp1(lbbox[0], lbbox[1], 0);
   svVector3 tmp2(rbbox[0], lbbox[1], 0);
   svVector3 tmp3(rbbox[0], rbbox[1], 0);
   svVector3 tmp4(lbbox[0], rbbox[1], 0);
   planePos.add(tmp1);
   planePos.add(tmp2);
   planePos.add(tmp3);
   planePos.add(tmp4);
}
void svSlice::SetColors(svColors *c){
   myColor = c;
   elementColors = c->GetColors();
}

void svSlice::cleanup(){
   validSlice.free();
   for(int i=0;i<seed_num;i++){
      elementColors[i].free();
   }
   delete [] elementColors;
   planePos.free();
}
*/
}


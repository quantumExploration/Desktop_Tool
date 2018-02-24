#include "svLineImage.h"
#include <fstream>
#include <string.h>

using namespace std;

namespace __svl_lib {

svLineImage::svLineImage(svRawQDOTData *d, svContourQDOTData *c){
  seed_num = 0;
  this->New(d,c);
}

void svLineImage::New(svRawQDOTData *d, svContourQDOTData *c){
  if(seed_num > 0) cleanup();
  svImage::New(d);
  contourData = c;
  lines = new svVector3ArrayPtr[seed_num];
}

void svLineImage::GenerateLines(){
  svVector3ArrayPtr *contour = contourData->GetContours();
  for(int i=0;i<seed_num;i++){
    lines[i] = new svVector3Array[contourData->GetContourCount(i)];
    for(int j=0;j<contourData->GetContourCount(i);j++){
      // svVector3Array tmppos;
       for(int t=0;t<contour[i][j].size();t++){
          svVector3 pos = contour[i][j][t];
          svScalar x = dot(pos-myOrigin[i], myCoor[0]);
          svScalar y = dot(pos-myOrigin[i], myCoor[1]);
          svScalar z = dot(pos-myOrigin[i], myCoor[2]); 
          svVector3 line;
          svScalar s = (irbbox[0]-ilbbox[0])/(irbbox[0]-ilbbox[0]+minD[0]);
          line[0]=x/s;
          s = (irbbox[1]-ilbbox[1])/(irbbox[1]-ilbbox[1]+minD[1]);
          line[1]=y/s;line[2]=z;
          line[0]=line[0]+minD[0]/2.;
          line[1]=line[1]+minD[1]/2.;
          lines[i][j].add(line);
        //  tmppos.add(line);
       }
      // lines[i].add(tmppos);
      // tmppos.free();
    }
  }
}

void svLineImage::RenderLines(){
  glColor3f(0,0,0);
  glBegin(GL_LINES);
  for(int i=0;i<seed_num;i++){
     glPushMatrix();
     glTranslatef(image_pos[i][0], image_pos[i][1], image_pos[i][2]);
     glScalef(scale, scale, scale);
     for(int j=0;j<contourData->GetContourCount(i);j++){
        for(int t=0;t<lines[i][j].size()-2;t+=2){
           glVertex2f(lines[i][j][t][0], lines[i][j][t][1]);
           glVertex2f(lines[i][j][t+1][0], lines[i][j][t+1][1]);
        }
     }
     glPopMatrix();
  }
  glEnd();
}

void svLineImage::Render3D(){
}

void svLineImage::cleanup(){
  for(int i=0;i<seed_num;i++){
    for(int j=0;j<contourData->GetContourCount(i);j++) 
       lines[i][j].free();
    delete [] lines[i];
  }
  delete [] lines;
}

svLineImage::~svLineImage(){
  cleanup();
}

}


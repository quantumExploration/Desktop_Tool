
#include "svDistanceImage.h"

#define DISTANCE_DEGREE 1 

namespace __svl_lib {


svDistanceImage::svDistanceImage(){
  dLength = NULL;
}

svDistanceImage::~svDistanceImage(){
  cleandistance();
}

virtual void svDistanceImage::ResetData(int seed):Reset(seed){//edit distance.h
  if(dLength!=NULL){
    cleandistance();
  }
  dLength = new svScalarArray[seed_num];
}

virtual void svDistanceImage::generateDistance(DistanceProperty &property){
  svDistance *distance = new svDistance();

  distance->computeDistance(property,//edit distance.h
                           data, mag, vel,
                           dLength);

  delete distance;
  distance = NULL;
}

virtual void svDistanceImage::setColorsbyDistsance(){
   for(int i=0;i<seed_num;i++){
     for(int j=0;j<pixelColors[i].size();j++){
       float light = 1-dLength[i][j]/180.;
       pixelColors[i][j][0] = (int)light;
       pixelColors[i][j][1] = (int)light;
       pixelColors[i][j][2] = (int)light;
       if(roiLabel[i][j]){
         pixelColors[i][j][3] = 1;
       }
       else {
         pixelColors[i][j][3] = alpha;//edit image.h
       }
     }
   } 
}

virtual void svDistanceImage::RenderLegend(){
   int x =0,y=0;
   char str[5];
   glColor3f(font_color[0],font_color[1],font_color[2]);//edit image.h
   sprintf(str,"0");
   glRasterPos2f(x, y-2);
   for(int j=0;j<strlen(str);j++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
   sprintf(str,"90");
   glRasterPos2f(x+90, y-2);
   for(int j=0;j<strlen(str);j++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
   sprintf(str,"180");
   glRasterPos2f(x+180, y-2);
   for(int j=0;j<strlen(str);j++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

   for(int i=0;i<=180;i++){
     float c = (float)c/180.;
     glColor3f(c,c,c);
     glBegin(GL_QUADS);
     glVertex2f(x,y);
     glVertex2f(x+1, y);
     glVertex2f(x+1, y+1); 
     glVertex2f(x,y+1);
     glEnd();
     x = x + 1;
   }
}

virtual void svDistanceImage::cleandistance(){
  for(int i=0;i<seed_num;i++){
    dLength[i].free();
  }
  delete [] dLength;
  dLength = NULL;
}

}


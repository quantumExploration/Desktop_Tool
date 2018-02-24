#include "svEntropyImage.h"
//entropy field
//360 bins 
namespace __svl_lib {

svEntropyImage::svEntropyImage(){
  entropy = NULL;
}

svEntropyImage::~svEntropyImage(){
  cleanentropy();
}

void svEntropyImage::resetData(int seed):reset(seed){
  if(entropy!=NULL){
    cleanentropy();
  }
  entropy = new svScalarArray[seed_num]; 
}

//input: patch size (neighborhood size)
void svEntropyImage::generateEntropy(svScalar width,
                               svScalar height){
  int x, y;
  maxEntropy = -9e+9;
  minEntropy = 9e+9;
  for(int i=0;i<seed_num;i++){
    int size = (rbbox[1]-lbbox[1])/height;
    for(int j=0;j<pixel[i].size();j++){
      svVector3Array v;
      for(int t=0;t<pixel[i].size();t++){
         if(pixel[i][j][0]>pixel[i][t][0]-width/2.
         && pixel[i][j][0]<pixel[i][t][0]+width/2.
         && pixel[i][j][1]>pixel[i][t][1]-height/2.
         && pixel[i][j][1]<pixel[i][t][1]+height/2.){
           v.add(vel[i][t]);
         }
      }
      svScalar en = GetEntropy(v);
      v.clear();
      entropy[i].add(en);         
      if(en>maxEntropy) maxEntropy = en;
      if(en<minEntropy) minEntropy = en;
    }
  }
}

void svEntropyImage::setColorsbyEntropy(){
   for(int i=0;i<seed_num;i++){
     for(int j=0;j<pixelColors[i].size();j++){
       float light = (entropy[i][j]-minEntropy)/(maxEntropy-minEntropy);
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

void svEntropyImage::renderLegend(){
   int x =0,y=0;
   int length = (maxEntropy - minEntropy)/0.1;

   char str[5];
   glColor3f(font_color[0],font_color[1],font_color[2]);//edit image.h
   sprintf(str,"%0.1f",minEntropy);
   glRasterPos2f(x, y-2);
   for(int j=0;j<strlen(str);j++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
   sprintf(str,"%0.1f",maxEntropy);
   glRasterPos2f(x+length, y-2);
   for(int j=0;j<strlen(str);j++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

   float degree = 1./(float)length;
   for(int i=0;i-1<length;i++){
     float c = (float)i*degree;
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

void svEntropyImage::cleanentropy(){
  for(int i=0;i<seed_num;i++){
     entropy[i].free();
  }
  delete [] entropy;
  maxEntropy = -9e+9;
  minEntropy = 9e+9;
}


}


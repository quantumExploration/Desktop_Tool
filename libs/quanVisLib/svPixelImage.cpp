#include "svPixelImage.h"
#include <fstream>
#include <string.h>

using namespace std;

namespace __svl_lib {

svPixelImage::svPixelImage(svRawQDOTData *d){
  seed_num = 0;
  this->New(d);
}

void svPixelImage::New(svRawQDOTData *d){
  if(seed_num > 0) cleanup();
  svImage::New(d);
  lowImagePlane = new imagePlane[seed_num];
}

void svPixelImage::LowPass(float cutoff, char *inputfile, char *output){
  char *exe = new char[2048];
  sprintf(exe, "%s/lowpass %s %f %s", BIN_DIR, inputfile, cutoff, output);
  system(exe);
  delete [] exe;

  ifstream infile(output);
  int n ;
  infile>>n;
  for(int i=0;i<n;i++){
     int r, c;
     infile>>r>>c;
     lowImagePlane[i].row=r;
     lowImagePlane[i].column=c;
     for(int j=0;j<r;j++){
         for(int t=0;t<c;t++){
             svVector4 rgb;
             infile>>rgb[0]>>rgb[1]>>rgb[2];
             lowImagePlane[i].rgb.add(rgb);
         }
     }
  }
  infile.close();
}

void svPixelImage::RenderPixel(){
  for(int i=0;i<seed_num;i++){
    glPushMatrix();
    int count = 0;
    glTranslatef(image_pos[i][0], image_pos[i][1]-sidey, image_pos[i][2]);
    glScalef(scale, scale, scale);
    for(int j=0;j<lowImagePlane[i].row;j++){
      for(int t=0;t<lowImagePlane[i].column;t++){
        glColor3f(lowImagePlane[i].rgb[count][0],
                 lowImagePlane[i].rgb[count][1],
                 lowImagePlane[i].rgb[count][2]);
        glBegin(GL_QUADS);
        glVertex2f(t,j);
        glVertex2f(t+1,j);
        glVertex2f(t+1,j+1);
        glVertex2f(t,j+1);
        glEnd();
        count++;
      }
    }
    glPopMatrix();
  }
}

void svPixelImage::cleanup(){
  for(int i=0;i<seed_num;i++){
    lowImagePlane[i].rgb.free();
  }
  delete [] lowImagePlane;
}
svPixelImage::~svPixelImage(){
  cleanup();
}



}


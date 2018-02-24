/*
 *base function of image
*children: in 2D and 3D
 * */
#ifndef __SV_PIXEL_IMAGE_H
#define __SV_PIXEL_IMAGE_H

#include "svImage.h"

namespace __svl_lib {

class svImage : public svSlice
{
 public:
  svImage(svRawQDOTData *d);
  virtual ~svImage();

  virtual void New(svRawQDOTData *d);  
  virtual void SetData(char *infName, int seed);
  virtual void GeneratePixels();  

  void Reshape(float x, int rw, int rh, int rx, int ry);

  virtual void RenderColors();
  virtual void RenderColors(svVector4Array *color);
  virtual void RenderBoundary();
  virtual void RenderPixel();
  virtual void Generate(){};
  virtual void Render(){};

  virtual void SetMinD(svVector3 d){minD = d;}
  virtual void SetSideX(float x);//{sidex = x;sidey=(rbbox[1]-lbbox[1])/(rbbox[0]-lbbox[0])*x;}
  virtual void SetPosition();
 
  virtual void SetMinD(); 
  svVector3 GetMinD(){return minD;}
  svScalar GetSideX(){return sidex;}
  svScalar GetSideY(){return sidey;}

  virtual void SetSide(svScalar x, svScalar y){sidex=x;sidey=y;}

  virtual void GenerateColors(char *inputfile1, char *inputfile2, float cutoff, char *output);
  virtual void SetColors(char *file);
  virtual void SaveColortoFile(char *file, svVector4Array *color);

  virtual void getImageSize(int &lbwidth, int &lbheight, int &rbwidth, int &rbheight); 
 
  //virtual void Move(int mouse, int mousy);
  //virtual void Mouse(int mousex, int mousey, int &layer, int &index, svScalar &den); 
 private:

 protected:
  void cleanup();

  svScalar sidex;
  svScalar sidey;
  svScalar tranx;
  svScalar trany;
  svScalar largeratio;
  int dataSize;
  int segment;

  svVector3 ilbbox;
  svVector3 irbbox;

  svVector3Array image_pos;
  svVector3Array *pixel;
  svVector3Array *pixelColor;
  svVector3 minD; 
  svVector3 pixelMinD;
};

}

#endif // __SV_IMAGE_H

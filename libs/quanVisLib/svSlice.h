#ifndef __SV_SLICE_H
#define __SV_SLICE_H

/*
 * cutting plane
 * base class
 * slice: center, direction, and distance
 * composition
 *
 * child class:
 * image (2D) + plane (3D)
 * */

//#include "svRawQDOTData.h"
#include "svColors.h"

namespace __svl_lib {

class svSlice 
{
 public:
  svSlice(){}

  virtual void RenderSlice(svScalar side, svVector3 origin,
                          svVector3 dir, svVector3 x, svVector3 y);

//  svSlice(svRawQDOTData *d);
  virtual ~svSlice(){}
/*
  virtual void New(svRawQDOTData *d);  
  virtual void SetColors(svColors *c);
  virtual void SetSlice(){}; 
  virtual void SetColors(char *file); 
 //reset slice based on whether there exist data on it
  
  virtual void Reshape(int w, int h){image_height= h; image_width =w;}
  virtual void RenderSlice();

  virtual void RenderSlice(svVector3 lbbox, svVector3 rbbox);

  virtual bool Mouse(int button, int state, int x, int y){};
  virtual void Move(int x, int y){}

  virtual svVector3 GetCenter(){return center;}
  virtual svVector3 GetDirection(){return direction;}
  //
  //virtual void SetMinD(); 
  //virtual void Move(int mouse, int mousy);
  //virtual void Mouse(int mousex, int mousey, int &layer, int &index, svScalar &den); 
 private:

 protected:
  void cleanup();
  void SetPosition();

  int image_width;
  int image_height;

  svRawQDOTData *myData;
  int seed_num;
  svColors *myColor;
  svVector4Array *elementColors;
  
  svIntArray validSlice;
  svInt currentSlice;
  svInt currentElement[2]; 

  svVector3Array planePos;
  svVector3 widgetPos;

  bool selectPlane;
  bool selectWidget;

//plane parameters
  svVector3 center;
  svVector3 minCenter;
  svVector3 direction;
  svScalar distance; 

  svVector3 lbbox;
  svVector3 rbbox;
*/
};

}

#endif // __SV_SLICE_H

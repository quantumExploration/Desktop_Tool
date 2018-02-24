#ifndef __SV_IMAGE_MOUSE_H
#define __SV_IMAGE_MOUSE_H

#include "svImageList.h"
#include "svMouseEvent.h"

namespace __svl_lib{

enum struct ImageSelection{
  one_layer,
  one_3d,
  //multiple_layer,
  one_pixel,
  one_label //cluster
};

class svImageSelect : svMouseSelect{
public:
  virtual void Select();
  virtual void Reset();

  svImageList *imageList;
  int selecttype;

  int selectpixel[2];
  int selectlabel;
  vector<int> selectedlabel;
  vector<int> selectlayer;
  vector<int> selectedlayer;
  //State *myState;//TODO: visibility + contour values
  //bool isMove;
protected:
  virtual bool SelectLayer(bool addselected);//svImage *image);
  virtual bool SelectPixel();//svImage *image);
  virtual bool SelectLabel();//svImage *image);
  //virtual void UpdateState();
};

class svImageMove : svMouseMove{
public:
  virtual void Move();

  svImageSelect *selectEvent;
  svImageList *imageList;
  svVector3 bounary2D[2];
protected:
  virtual void MoveOneLayer();
  virtual void MoveMultipleLayers();
  virtual void SetLocations();
  virtual void Drag2DinGrid();
  virtual void Dragto3D();
};

class svImageRelease : svMouseRelease{
public:
  virtual void Release();
  svImageSelect *selectEvent;
  svImageList *imageList;
  State *state;

protected:
  virtual void UpdateState();
};

class svImageMotion : svMouseMotion{
  virtual void Motion();

  int motionlayer;
  int motionpixel[2];
  int motionlabel;
  svImageList *imageList;
  svImageSelect *selectEvent;

protected:
  virtual void MotionLayer();
  virtual void MotionPixel();
  virtual void MotionLabel();
};

}
#endif

#ifndef __SV_IMAGE_MOUSE_H
#define __SV_IMAGE_MOUSE_H

#include "svImageList.h"
#include "svMouseEvent.h"
namespace __svl_lib{

enum ImageSelection{
  one_layer,
  one_3d,
  one_label, //cluster
  //multiple_layer,
  one_pixel
};

class svImageSelect : public svMouseSelect{
public:
  virtual void Select();
  virtual void Reset();

  svImageList *imageList;
  int selecttype;

  svVector3 boundary2D[2];
  int selectpixel[3];
  int selectlabel;
  svVector4 selectcolor;
  vector<svVector4> selectedcolor;
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

class svImageMotion : public svMouseMotion{
public:
  virtual void Motion();
  svImageSelect *selectEvent;
  svVector3 boundary2D[2];
protected:
  virtual void MoveOneLayer();
  virtual void MoveMultipleLayers();
  virtual void SetLocations(int column, int row);
  virtual void Drag2DinGrid();
  virtual void Dragto3D();
};

class svImageRelease : public svMouseRelease{
public:
  virtual void Release();
  svImageSelect *selectEvent;
  State *state;

protected:
  virtual void UpdateState();
};

class svImageMove : public svMouseMove{
public:
  virtual void Move();

  int movelayer;
  int movepixel[2];
  int movelabel;
  svVector4 movecolor;
  svImageSelect *selectEvent;

protected:
  virtual void MoveLayer();
  virtual void MovePixel();
  virtual void MoveLabel();
};

}
#endif

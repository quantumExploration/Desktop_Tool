#ifndef __SV_QDOT_SLIDER_H
#define __SV_QDOT_SLIDER_H

#include <vector>
#include <string.h>

#include "svSlider.h"
#include "svUtil.h"

using namespace std;

namespace __svl_lib {

class svQDOTSlider
{
 public:
  svQDOTSlider();  
  ~svQDOTSlider(){recP.clear();}

  virtual void setMag(svScalar minM, svScalar maxM);
  virtual void setBoundary(svVector3 lb, svVector3 rb);
  virtual void setWidgetMag();
  virtual void setWidgetVel();
  virtual void setWidgetPos();
  virtual void initRec(int h, int w);
  virtual void setRec(svVector3 pos);
  virtual void RenderMag();
  virtual void RenderVel();
  virtual void RenderPos();
  virtual void RenderRec();
  virtual void RenderText();
  virtual void Render();
  virtual void Mouse(int x, int y);
  virtual void widgetMouse(int x, int y);
  virtual void widgetMotion(int x, int y);
  virtual void widgetMovement(int x, int y);

  virtual void setImage(int w, int h, int offsetw, int offseth)
  {
     image_height = h;
     image_width = w;
     image_pos_x = offsetw;
     image_pos_y = offseth;   
  }
 protected:

 private:
  svSlider *magSlider;
  svSlider *vxSlider;
  svSlider *vySlider;
  svSlider *vzSlider;
  svSlider *pxSlider;
  svSlider *pySlider;
  svSlider *pzSlider; 

  svVector3 recPos[4];
  
  svScalar rangeMag[2];
  svScalar widgetMag[2];
  svScalar degreeMag;

  svScalar rangeVX[2];
  svScalar degreeVX;
  svScalar widgetVX[2];
  svScalar rangeVY[2];
  svScalar degreeVY;
  svScalar widgetVY[2];
  svScalar rangeVZ[2];
  svScalar degreeVZ;
  svScalar widgetVZ[2];

  svScalar rangePX[2];
  svScalar degreePX;
  svScalar widgetPX[2];
  svScalar rangePY[2];
  svScalar degreePY;
  svScalar widgetPY[2];
  svScalar rangePZ[2];
  svScalar degreePZ;
  svScalar widgetPZ[2];

  bool selectMag;
  bool selectVel;
  bool selectPos;
 
  int recH;
  int recW;
  int textW;
  svVector3 recShift;
  vector<svVector3> recP;

  int image_pos_x;
  int image_pos_y;
  int image_height;
  int image_width;

  svVector3 shiftMag;
  svVector3 shiftVX;
  svVector3 shiftVY;
  svVector3 shiftVZ;
  svVector3 shiftPX;
  svVector3 shiftPY;
  svVector3 shiftPZ;
  svScalar scaleSize;

  svVector3 font_color;
  
 protected:
 // void cleanup();
};

}
#endif // __SV_QDOT_SLIDER_H

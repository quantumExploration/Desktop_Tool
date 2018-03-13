#ifndef SV_ROI_SLIDER_H
#define SV_ROI_SLIDER_H

#include "svSlider.h"
#include "svQDOTData.h"

#define ROI_SLIDER_SIZE 4 
#define ROI_SLIDER_RATIO 0.5
#define SLIDER_MAG_DEGREE 0.1
#define SLIDER_DIR_DEGREE 5

namespace __svl_lib{

class svROISlider{
public:
  svROISlider(svQDOTData *data);
  ~svROISlider(){roundPos.clear();}

  void Init(svQDOTData *data);

  void setWidgetVel();
  void setWidgetPos();
  void setWidgetMag();

  void Reshape(int left, int top, int width);

  void Render();
  void UpdateState();//update visiblity

  int left;
  int bottom;
  int width;

  //define degree of movement in svROISliderMouse

  State *state;
  svQDOTData *myData;
  svSlider * slider[7]; // pos_x, pos_y, pos_z, dir_x, dir_y, dir_z, mag

//=================Interaction=========================//
  svVector3 recPos[2];
  svVector3 magRec[2];
  svVector3 dirRec[2];
  svVector3 posRec[2];
  bool selectRec[3];//mag, vel and pos

protected:
  void setTransformation(svScalar r, int recH, int recW);
  void setRec(svScalar r, int recH, int recW);

  void RenderMag();
  void RenderDir();
  void RenderPos();
  void RenderRec();
  void RenderText();

private:
  vector<svVector3> roundPos;

  
  svVector3 font_color;

  svVector3 mag_text;
  svVector3 dir_text;
  svVector3 pos_text;

  svScalar mag_degree;
  svScalar dir_degree[3];
  svScalar pos_degree[3];

  svScalar mag_range[2];
  svScalar dir_range[3][2];
  svScalar pos_range[3][2];

  svScalar widgetMag[2];
  svScalar widgetDir[3][2];
  svScalar widgetPos[3][2];
};


}
#endif

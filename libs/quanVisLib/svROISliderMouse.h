#ifndef __SV_ROI_SLIDER_MOUSE_H
#define __SV_ROI_SLIDER_MOUSE_H

#include "svMouseEvent.h"
#include "svROISlider.h"

namespace __svl_lib{


class svROISliderSelect : public svMouseSelect{
public:
  virtual void Select();
  virtual void Reset();

  svROISlider *slider;
};

class svROISliderMotion : public svMouseMotion{ //drage
public:
  virtual void Motion();
  //svROISlider *slider;
  svROISliderSelect *selectEvent;
};

class svROISliderRelease : public svMouseRelease{
public:
  virtual void Release();
  svROISliderSelect *selectEvent;

//  svImageSelect *selectEvent;
};

class svROISliderMove : public svMouseMove{
public:
  virtual void Move();
  svROISliderSelect *selectEvent;

  //svImageSelect *selectEvent;
};

}
#endif

#ifndef __SV_ROI_SLIDER_MOUSE_H
#define __SV_ROI_SLIDER_MOUSE_H

#include "svROISlider.h"

namespace __svl_lib{


class svROISliderSelect : svMouseSelect{
public:
  virtual void Select();
  virtual void Reset();

  svROISlider *slider;
};

class svROISliderMotion : svMouseMotion{ //drage
  virtual void Motion();
  //svROISlider *slider;
  svROISliderSelect *selectEvent;
};

class svROISliderRelease : svMouseRelease{
public:
  virtual void Release();
  svROISliderSelect *selectEvent;

//  svImageSelect *selectEvent;
};

class svROISliderMove : svMouseMove{
public:
  virtual void Move();
  svROISliderSelect *selectEvent;

  //svImageSelect *selectEvent;
};

}
#endif

#ifndef SV_MOUSE_GROUP
#define SV_MOUSE_GROUP

#include "svImageMouse.h"
#include "svWidgetMouse.h"
#include "svROISliderMouse.h"

#include <queue>

namespace __svl_lib{

enum struct selectableType{
  simage,
  swidget, //ROI
  slayerwidget,
  sbutton
};

struct ImageProperty{
  svImageSelect *select;
  svImageMove *move;
  svImageMotion *motion;
  svImageRelease *release;
};

struct WidgetProperty{
  svWidgetSelect *select;
  svWidgetRelease *release;
  svWidgetMotion *motion;
  svWidgetMove *move;
};

struct ROISliderProperty{
  svROISliderSelect *select;
  svROISliderMove * move;
  svROISliderRelease * release;
  svROISliderMotion * motion;
};

typedef struct MouseEventProperty{
  ImageProperty *image;
  WidgetProperty *widget;
  ButtonProperty *button;
  ROISliderProperty *slider;
  //SliderProperty *slider;//single slider to control weight; double slide to control visible
};

class svMouseGroup{
  svMouseGroup(){};
  ~svMouseGroup(){mouseGruops.clear();}
  void AddImageMouse(svImageList *imageList);
  void AddWidgetMouse(svWidget *widget);
  void AddROISliderMouse(svROISlider *slider);

  queue<MouseEventProperty *> mouseGruops;
};

}

#endif

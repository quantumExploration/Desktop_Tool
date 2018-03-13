#ifndef SV_MOUSE_GROUP
#define SV_MOUSE_GROUP

#include "svImageMouse.h"
#include "svWidgetMouse.h"
#include "svROISliderMouse.h"
#include "svButtonMouse.h"

#include <vector>

using namespace std;

namespace __svl_lib{

enum selectableType{
  mouse_widget=0, //ROI
  mouse_slider=1,
  mouse_image=2,
  mouse_button=3
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

struct ButtonProperty{
  svButtonSelect *select;
  svButtonMove *move; 
  svButtonRelease * release;  
};

typedef struct MouseEventProperty{
  ImageProperty *image;
  WidgetProperty *widget;
  ButtonProperty *button;
  ROISliderProperty *slider;
  //SliderProperty *slider;//single slider to control weight; double slide to control visible
};

class svMouseGroup{
public:
  svMouseGroup(){};
  ~svMouseGroup(){};//???mouseGruops.clear();}
  void AddImageMouse(svImageList *imageList, int type);
  void SetImageMouseType(int type);
  void ResetImageLabel();
  void AddWidgetMouse(svWidget *widget);
  void AddROISliderMouse(svROISlider *slider);
  void AddButtonMouse(svSymmetryButtons *button);
  vector<MouseEventProperty *> mouseGroups;
  int currentMouse;
};

}

#endif

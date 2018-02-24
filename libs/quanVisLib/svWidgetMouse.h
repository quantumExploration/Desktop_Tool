#ifndef __SV_WIDGET_MOUSE_H
#define __SV_WIDGET_MOUSE_H

#include "svWidget.h"

namespace __svl_lib{


class svWidgetSelect : svMouseSelect{
public:
  virtual void Select();
  virtual void Reset();

  svWidget * widget;
};

class svWidgetMotion : svMouseMotion{ //drage
  virtual void Motion();
  svWidgetSelect *selectEvent;
//  svWidget * widget;
};

class svWidgetRelease : svMouseRelease{
public:
  virtual void Release();
  //svWidget * widget;
  svWidgetSelect *selectEvent;

//  svImageSelect *selectEvent;
};



class svWidgetMove : svMouseMove{
public:
  virtual void Move();
  //svWidget * widget;
  svWidgetSelect *selectEvent;

  //svImageSelect *selectEvent;
};

}
#endif

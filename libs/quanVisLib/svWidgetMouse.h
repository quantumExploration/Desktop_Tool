#ifndef __SV_WIDGET_MOUSE_H
#define __SV_WIDGET_MOUSE_H

#include "svWidget.h"
#include "svMouseEvent.h"

namespace __svl_lib{

class svWidgetSelect : public svMouseSelect{
public:
  virtual void Select();
  virtual void Reset();

  svWidget * widget;
};

class svWidgetMotion : public svMouseMotion{ //drage
public:
  virtual void Motion();
  svWidgetSelect *selectEvent;
//  svWidget * widget;
};

class svWidgetRelease : public svMouseRelease{
public:
  virtual void Release();
  //svWidget * widget;
  svWidgetSelect *selectEvent;

//  svImageSelect *selectEvent;
};



class svWidgetMove : public svMouseMove{
public:
  virtual void Move();
  //svWidget * widget;
  svWidgetSelect *selectEvent;

  //svImageSelect *selectEvent;
};

}
#endif

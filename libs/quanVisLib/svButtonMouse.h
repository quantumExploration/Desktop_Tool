#ifndef __SV_BUTTON_MOUSE_H
#define __SV_BUTTON_MOUSE_H

#include "svSymmetryButtons.h"
#include "svMouseEvent.h"

namespace __svl_lib{

class svButtonSelect : public svMouseSelect{
public:
  virtual void Select();
  virtual void Reset();

  int selectstate[9];
  svSymmetryButtons *buttons;
};


class svButtonRelease : public svMouseRelease{
public:
  virtual void Release();
  svButtonSelect *selectEvent;
};



class svButtonMove : public svMouseMove{
public:
  virtual void Move();
  svButtonSelect *selectEvent;
};

}
#endif

#ifndef SV_MOUSE_EVENT
#define SV_MOUSE_EVENT

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

namespace __svl_lib{

enum  Button {
  button_left,
  button_right,
  button_middle
};

enum ButtonState{
  button_up,
  button_down
};

class svMouseSelect{
public:
  svMouseSelect(){};
  ~svMouseSelect(){};

  virtual void Select(){};

  bool success;
  int mousex;
  int mousey;
  int button;
  int mouse_state;
  bool isMove;
};

class svMouseMove{
public:
  svMouseMove(){};
  ~svMouseMove(){};

  virtual void Move(){}

  int mousex;
  int mousey;
  //int button;
  //int state;
};

class svMouseMotion{
public:
  svMouseMotion(){};
  ~svMouseMotion(){};

  virtual void Motion(){}

  int mousex;
  int mousey;
};

class svMouseRelease{
public:
  svMouseRelease(){}
  ~svMouseRelease(){}
};

}

#endif

#ifndef __SV_SLIDER_H
#define __SV_SLIDER_H

#include <vector>
#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include "svType.h"
#include "svVectorMatrix.h"
#include "svArray.h"


using namespace std;

namespace __svl_lib {

class svSlider
{
 public:
  svSlider();
  virtual ~svSlider(){}

//  virtual void ResetBool();

  virtual void SetFontColor(svVector4 c){font_color = c;}
  virtual void SetWidgetColor();

  //virtual void Mouse(int x, int y);
  //virtual void Motion(int y);
  //virtual void Movement(int x, int y);

  virtual svScalar getTop();
  virtual svScalar getDown();

  //virtual bool getTopSelect(){return widgetSelect[1];}
  //virtual bool getDownSelect(){return widgetSelect[0];}

  virtual void Render(string bottomText, string topText,
                         string widgetText1, string widgetText2,
                         string subject);

  virtual void RenderDoubleSlider(string bottomText, string topText,
                    string widgetText1, string widgetText2,
                    string subject);
  virtual void RenderSingleSlider(string bottomText, string topText,
                    string widgetText,
                    string subject);

  //---------------------Interaction-----------------------------//
  bool widgetSelect[2];
  svVector3 boundWidgetPos[2];
  svVector3 widgetPos[2];
  svScalar widgetW;
  svScalar widgetH;
  float tranx;
  float trany;
  float scalex;
  float scaley;
  bool isDouble;

protected:
  void ResetColor();
 private:

  svVector4 font_color;
  svVector4 widgetColor[2];


};

}
#endif // __SV_SLIDE_H

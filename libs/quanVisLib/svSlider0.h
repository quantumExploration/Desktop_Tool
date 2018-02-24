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

  virtual void ResetBool();

  virtual void SetFontColor(svVector4 c){font_color = c;}


  virtual void Mouse(int x, int y);
  virtual void Motion(int y);
  virtual void Movement(int x, int y);

  virtual svScalar getTop();
  virtual svScalar getDown();

  virtual bool getTopSelect(){return widgetSelect[1];}
  virtual bool getDownSelect(){return widgetSelect[0];}
 
  virtual void Render(char *bottomText, char *topText,
                    char *widgetText1, char *widgetText2,
                    char *subject);
protected:
  void ResetColor();
 private:
  
  svVector4 font_color;
  
  svVector3 boundWidgetPos[2];
  svVector3 widgetPos[2];

  svVector4 widgetColor[2]; 
 
  bool widgetSelect[2];
  
};

}
#endif // __SV_SLIDE_H

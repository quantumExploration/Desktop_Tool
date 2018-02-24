#include "svWidget.h"
#include "svQDOT.h"

using namespace std;
using namespace __svl_lib;

enum widgetsType{
   WIDGETS_STEP = 0;
   WIDGETS_DRAG = 1;
   WIDGETS_LEFT = 2;
   WIDGETS_RIGHT = 3;
};

class interfaceApi(){
public: 
  interfaceApi();
  ~interfaceApi();

  void initWidgets(svScalar tx, svScalar ty,
                               svScalar sx, svScalar sy,
                               int seed,
                               int zmin, int zmax, int nonz);
  void setWidgets(svScalarArray *histo);
  void setWidgetsRepeat(bool repeat);
  void setWidgetsValues(svVector3 *v);
  void showWidgets();
  void mouseWidgets(int status, int direction,
                               int x, int y);
  void motionWidgets(int x, int y);

  svInt *getWidgetsIndex(); //need to be implemented

private:
  svWidget *widget;
 
  bool showWidgets;
 
  svScalar widget_tranx;
  svScalar widget_trany;
  svScalar widget_scalex;
  svScalar widget_scaley;
};

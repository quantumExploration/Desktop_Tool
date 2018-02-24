#include "interfaceApi.h"

interfaceApi::interfaceApi(){
   widget = new svWidget();
}

~interfaceApi::interfaceApi(){
   delete widget;
}

void interfaceApi::initWidgets(svScalar tx, svScalar ty,
                               svScalar sx, svScalar sy,
                               int seed,  
                               int zmin, int zmax, int nonz){
   widget_tranx = tx;
   widget_trany = ty;
   widget_scalex = sx;
   widget_scaley = sy;
  
   widget->SetLevel(seed);
   widget->Init();
   widget->SetIndex(zmin, zmax, nonz);
}

void interfaceApi::setWidgets(svScalarArray *histo){
   widget->SetHistoValues(histo);
}

void interfaceApi::setWidgetsRepeat(bool repeat){
   widget->Repeat(repeat);
}

void interfaceApi::setWidgetsValues(svVector3 *v){
   widget->SetValues(v);
}

void interfaceApi::showWidgets(){
    glPushMatrix();
    glTranslatef(widget_tranx, widget_trany,0);
    glScalef(widget_scalex,widget_scaley,1);
    widget->Render(topmaglevel);
    glPopMatrix();
}

void interfaceApi::mouseWidgets(int status, int direction,
                               int x, int y){
  if(status == WIDGETS_STEP){
    if(direction == WIDGETS_LEFT){
       widget->MoveLeft();
    }
    else if(direction == WIDGETS_RIGHT){
       widget->MoveRight();
    }
  }
  else if(status == WIDGETS_DRAG){
    widget->Mouse(widget_tranx, widget_trany,
                  widget_scalex, widget_scaley,
                  x,y);
  }
}

void interfaceApi::motionWidgets(int x, int y){
    int x1 = (x-widget_tranx)/widget_scalex;
    int y1 = (y-widget_trany)/widget_scaley;
    widget->Move(x1, y1);
}


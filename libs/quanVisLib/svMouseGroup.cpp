#include "svMouseGroup.h"

using namespace std;
namespace __svl_lib{

void svMouseGroup::AddImageMouse(svImageList *imageList, int type)
{
  svImageSelect *select = new svImageSelect();
  svImageMove * move = new svImageMove();
  svImageMotion * motion = new svImageMotion();
  svImageRelease * release = new svImageRelease();

  select->imageList = imageList;
  select->selecttype = type;
  motion->selectEvent = select;
  select->boundary2D[0][0]=9e+9;
  select->boundary2D[0][1]=9e+9;
  select->boundary2D[1][0]=-9e+9;
  select->boundary2D[1][1]=-9e+9;
  for(int i=0;i<imageList->imageList.size();i++)
  {
    float tranx = imageList->imageList[i]->tranx;
    float sidex = imageList->imageList[i]->sidex;
    float trany = imageList->imageList[i]->trany;
    float sidey = imageList->imageList[i]->sidey;
    if(select->boundary2D[0][0]>tranx)
        select->boundary2D[0][0] = tranx;
    if(select->boundary2D[1][0]<sidex + tranx)
        select->boundary2D[1][0] = sidex + tranx;
    if(select->boundary2D[0][1]>trany)
        select->boundary2D[0][1] = trany;
    if(select->boundary2D[1][1]<sidey + trany)
        select->boundary2D[1][1] = sidey + trany;
  }
  move->selectEvent = select;
  release->selectEvent = select;

  ImageProperty *property = new ImageProperty();
  property->select = select;
  property->move = move;
  property->motion = motion;
  property->release = release;

  MouseEventProperty *mproperty = new MouseEventProperty();
  mproperty->image = property;
  mproperty->widget = NULL;
  mproperty->button = NULL;
  mproperty->slider = NULL;

  mouseGroups.push_back(mproperty);
}

void svMouseGroup::SetImageMouseType(int type)
{
  for(int i=0;i<mouseGroups.size();i++)
  {
       if(mouseGroups[i]->image!=NULL)
       {
           mouseGroups[i]->image->select->selecttype = type;
           break;
       }
  }
}
void svMouseGroup::ResetImageLabel()
{
  for(int i=0;i<mouseGroups.size();i++)
  {
       if(mouseGroups[i]->image!=NULL)
       {
           mouseGroups[i]->image->select->selectedlabel.clear();//Reset();
           break;
       }
  }
}


void svMouseGroup::AddWidgetMouse(svWidget *widget)
{
  svWidgetSelect *select = new svWidgetSelect();
  svWidgetMove *move = new svWidgetMove();
  svWidgetMotion *motion = new svWidgetMotion();
  svWidgetRelease *release = new svWidgetRelease();

  select->widget = widget;
  move->selectEvent = select;
  motion->selectEvent = select;
  release->selectEvent = select;

  WidgetProperty *property = new WidgetProperty();
  property->select = select;
  property->move = move;
  property->motion = motion;
  property->release = release;

  MouseEventProperty *mproperty = new MouseEventProperty();
  mproperty->image = NULL;
  mproperty->widget = property;
  mproperty->button = NULL;
  mproperty->slider = NULL;
  mouseGroups.push_back(mproperty);
}

void svMouseGroup::AddButtonMouse(svSymmetryButtons *button)
{
  svButtonSelect *select = new svButtonSelect();
  svButtonMove *move = new svButtonMove();
  svButtonRelease *release = new svButtonRelease();

  select->buttons = button;
  select->Reset();
  move->selectEvent = select;
  release->selectEvent = select;

  ButtonProperty *property = new ButtonProperty();
  property->select = select;
  property->move = move;
  property->release = release;

  MouseEventProperty *mproperty = new MouseEventProperty();
  mproperty->image = NULL;
  mproperty->widget = NULL;
  mproperty->button = property; 
  mproperty->slider = NULL;
  mouseGroups.push_back(mproperty);
}

void svMouseGroup::AddROISliderMouse(svROISlider *slider)
{
  svROISliderSelect *select = new svROISliderSelect();
  svROISliderMove *move = new svROISliderMove();
  svROISliderMotion *motion = new svROISliderMotion();
  svROISliderRelease *release = new svROISliderRelease();

  select->slider = slider;
  move->selectEvent = select;
  motion->selectEvent = select;
  release->selectEvent = select;

  ROISliderProperty *property = new ROISliderProperty();
  property->select = select;
  property->move = move;
  property->motion = motion;
  property->release = release;

  MouseEventProperty *mproperty = new MouseEventProperty();
  mproperty->image = NULL;
  mproperty->widget = NULL;
  mproperty->button = NULL;
  mproperty->slider = property;
  mouseGroups.push_back(mproperty);
}

}

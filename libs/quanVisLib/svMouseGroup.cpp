#include "svMouseGroup.h"

using namespace std;
namespace __svl_lib{

void svMouseGroup::AddImageMouse(svImageList *imageList)
{
  svImageSelect *select = new svImageSelect();
  svImageMove * move = new svImageMove();
  svImageMotion * motion = new svImageMotion();
  svImageRelease * release = new svImageRelease();
  select->imageList = imageList;
  move->imageList = imageList;
  motion->imageList = imageList;
  release->imageList = imageList;
  motion->selectEvent = select;
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
  queue.push(mproperty);
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
  queue.push(mproperty);
}

void svMouseGroup::AddROISliderMouse(svROISlider *slider)
{
  svROISliderSelect *select = new svROISliderSelect();
  svROISliderMove *move = new svROISliderMove();
  svROISliderMotion *motion = new svROISliderMotion();
  svROISliderRelease *release = new svROISliderRelease();
  select->sldier = slider;
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
  queue.push(mproperty);
}

}

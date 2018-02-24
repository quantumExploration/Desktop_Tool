#include "svROISlider.h"

namespace __svl_lib{

svROISlider::svROISlider(svQDOTData *data)
{
  for(int i=0;i<7;i++)
  {
    slider[i] = new svSlider();
    slider[i]->isDouble = true;
  }
  Init(data);
}

void svROISlider::Init(svQDOTData *data)
{
  myData = data;

  mag_degree = SLIDER_MAG_DEGREE;
  dir_degree[0] = SLIDER_DIR_DEGREE;
  dir_degree[1] = SLIDER_DIR_DEGREE;
  dir_degree[2] = SLIDER_DIR_DEGREE;
  pos_degree[0] = myData->myQDOT->minXDistance;
  pos_degree[1] = myData->myQDOT->minYDistance;
  pos_degree[2] = myData->myQDOT->minZDistance;
  font_color[0] = 0;
  font_color[1] = 0;
  font_color[2] = 0;
  font_color[3] = 0.5;
  mag_range[0] = myData->minMag;
  mag_range[1] = myData->maxMag;
  dir_range[0][0] = 0; dir_range[0][1] = 180;
  dir_range[1][0] = 0; dir_range[1][1] = 180;
  dir_range[2][0] = 0; dir_range[2][1] = 180;
  pos_range[0][0] = myData->myQDOT->lbbox[0];
  pos_range[1][0] = myData->myQDOT->lbbox[1];
  pos_range[2][0] = myData->myQDOT->lbbox[2];
  pos_range[0][1] = myData->myQDOT->rbbox[0];
  pos_range[1][1] = myData->myQDOT->rbbox[1];
  pos_range[2][1] = myData->myQDOT->rbbox[2];

  setWidgetVel();
  setWidgetPos();
  setWidgetMag();
}

void svROISlider::setWidgetVel()
{
  int sliderindex[3] = {3,4,5};
  for(int i=0;i<3;i++)
  {
    for(int j=0;j<2;j++)
    {
      svScalar distance = fabs(slider[sliderindex[i]]->widgetPos[j][2]
                              -slider[sliderindex[i]]->boundWidgetPos[0][2]);
      widgetDir[i][j] = distance/(dir_range[i][1]-dir_range[i][0]);
      int num = (int)(widgetDir[i][j] / dir_degree[i]);
      widgetDir[i][j] = (svScalar)num * dir_degree[i] + dir_range[i][0];
    }
  }
}

void svROISlider::setWidgetPos()
{
  int sliderindex[3] = {0,1,2};
  for(int i=0;i<3;i++)
  {
    for(int j=0;j<2;j++)
    {
      svScalar distance = fabs(slider[sliderindex[i]]->widgetPos[j][2]
                              -slider[sliderindex[i]]->boundWidgetPos[0][2]);
      widgetPos[i][j] = distance/(pos_range[i][1]-pos_range[i][0]);
      int num = (int)(widgetPos[i][j] / pos_degree[i]);
      widgetPos[i][j] = (svScalar)num * pos_degree[i] + pos_range[i][0];
    }
  }
}

void svROISlider::setWidgetMag()//logMag
{
  svScalar D = fabs(slider[sliderindex->boundWidgetPos[1][2]
                            -slider[sliderindex->boundWidgetPos[0][2]);
  int sliderindex = 6;
  for(int j=0;j<2;j++)
  {
    svScalar distance = fabs(slider[sliderindex->widgetPos[j][2]
                              -slider[sliderindex->boundWidgetPos[0][2]);
    if(isEqual(distance, 0, false))
    {
      widgetMag[j] = mag_range[0];
    }
    else if(isEqual(distance, D, false))
    {
      widgetMag[j] = mag_range[1];
    }
    else
    {
      distance = log10(distance);
      if(mag_range[0] > 0)
      {
        widgetMag[j] = distance/(log10(mag_range[1])-log10(mag_range[0]));
      }
      else
      {
        widgetMag[j] = distance/(log10(mag_range[1]));
      }
      int num = (int)(widgetMag[j] / mag_degree);
      widgetMag[j] = (svScalar)num * mag_degree + mag_range[0];
      widgetMag[j] = pow(10., widgetMag[j]);
    }

  }
}

void svROISlider::Reshape(int left, int bottom, int width)
{
  this->left = left;
  this->bottom = bottom;
  this->width = width;
  roundPos.clear();

  int recW = width / ROI_SLIDER_SIZE;
  int recH = recW  * ROI_SLIDER_RATIO;
  double r = (double)recH/10.;
  int h = recH;
  int w = recW;

  svVector3 p;
  p[0] = r;
  p[1] = r;
  for(int i=0;i<=5;i++)
  {
    svVector3 tmp;
    tmp[0] = p[0] + r * cos(M_PI/10. * (double)i + M_PI);
    tmp[1] = p[1] + r * sin(M_PI/10. * (double)i + M_PI);
    tmp[2] = 0;
    roundPos.push_back(tmp);
  }

  p[0] = w - r;
  p[1] = r;
  for(int i=0;i<=5;i++)
  {
    svVector3 tmp;
    tmp[0] = p[0] + r * cos(M_PI/10. * (double)i + M_PI*1.5);
    tmp[1] = p[1] + r * sin(M_PI/10. * (double)i + M_PI*1.5);
    tmp[2] = 0;
    roundPos.push_back(tmp);
  }

  p[0] = w - r;
  p[1] = h - r;
  for(int i=0;i<=5;i++)
  {
    svVector3 tmp;
    tmp[0] = p[0] + r * cos(M_PI/10. * (double)i);
    tmp[1] = p[1] + r * sin(M_PI/10. * (double)i);
    tmp[2] = 0;
    roundPos.push_back(tmp);
  }

  p[0] = r;
  p[1] = h-r;
  for(int i=0;i<=5;i++)
  {
    svVector3 tmp;
    tmp[0] = p[0] + r * cos(M_PI/10. * (double)i + M_PI/2.);
    tmp[1] = p[1] + r * sin(M_PI/10. * (double)i + M_PI/2.);
    tmp[2] = 0;
    roundPos.push_back(tmp);
  }
  setRec(r, recH, recW);
  setTransformation(r, recH, recW);
}

void svROISlider::setTransformation(svScalar r, int recH, int recW)
{
  svScalar textW = magRec[1][0] - magRec[0][0];
  //textW = recW/5.;//+r;
  svScalar x = recW - textW;//(recW+r*2 - textW);
  //textW =  + recW/5.; //-r

  mag_text[0] = 10.;//-r+
  mag_text[1] = (float)recH*2./3.+(float)recH/8.;
  dir_text[0] = 10.;//-r+
  dir_text[1] = (float)recH/3.+(float)recH/8.);
  pos_text[0] = 10.;//-r+
  pos_text[1] = (float)recH/8.;

  slider[6]->tranx =  textW + (svScalar)x/2.;
  slider[6]->trany = r;//(svScalar)recH/10.;
  slider[3]->tranx = textW + (svScalar)x/6.;
  slider[3]->trany = slider[6]->trany;
  slider[4]->tranx = textW + (svScalar)x/2.-r/2.;
  slider[4]->trany = slider[6]->trany;
  slider[5]->tranx = textW + (svScalar)x*5./6.-r;
  slider[5]->trany = slider[6]->trany;
  slider[0]->tranx = slider[3]->tranx;
  slider[0]->trany = slider[3]->trany;
  slider[1]->tranx = slider[4]->tranx;
  slider[1]->trany = slider[4]->trany;
  slider[2]->tranx = slider[5]->tranx;
  slider[2]->trany = slider[5]->trany;
  for(int i=0;i<6;i++)
  {
    slider[i]->scalex = ((svScalar)recH - (svScalar)recH/6.)/100.;
    slider[i]->scaley = slider[i]->scalex;
  }
}

void svROISlider::setRec(float r, int recH, int recW)
{
  recPos[0][0] = left;
  recPos[1][0] = recPos[0][0] + width / ROI_SLIDER_SIZE;
  recPos[0][1] = bottom;
  recPos[1][1] = bottom + width / ROI_SLIDER_SIZE * ROI_SLIDER_RATIO;

  magRec[0][0] = 0;
  magRec[0][1] = (float)recH*2./3.;
  magRec[1][0] = (float)recW/5.;
  magRec[1][1] = (float)recH;

  dirRec[0][0] = 0;
  dirRec[0][1] = (float)recH/3.;
  dirRec[1][0] = (float)recW/5.;
  dirRec[1][1] = (float)recH*2./3.;

  posRec[0][0] = 0;
  posRec[0][1] = 0;
  posRec[1][0] = (float)recW/5.;
  posRec[1][1] = (float)recH;
}

void svROISlider::Render()
{
  glPushMatrix();
  glTranslatef(recPos[0][0], recPos[0][1],0);
  RenderRec();
  RenerTex();
  if(selectRec[0])
    RenderMag();
  else if(selectRec[1])
    RenderDir();
  else if(selectRec[2])
    RenderPos();
  glPopMatrix();
}

void svROISlider::RenderMag()
{
  char bottomText[20];
  char topText[20];
  char widgetText1[20];
  char widgetText2[20];

  sprintf(bottomText, "%6.2e", mag_range[0]);
  sprintf(topText, "%6.2e", mag_range[1]);
  sprintf(widgetText1, "%6.2e", widgetMag[0]);
  sprintf(widgetText2, "%6.2e", widgetMag[1]);

  slider[6]->Render(string(bottomText),
                    string(topText),
                    string(widgetText1),
                    string(widgetText2),
                    "");
}

void svROISlider::RenderDir()
{
  char bottomText[20];
  char topText[20];
  char widgetText1[20];
  char widgetText2[20];

  int sliderindex[3] = {3,4,5};
  for(int i=0;i<3;i++)
  {
    sprintf(bottomText, "%0.1f", dir_range[i][0]);
    sprintf(topText, "%0.1f", dir_range[i][1]);
    sprintf(widgetText1, "%0.1f", widgetDir[i][0]);
    sprintf(widgetText2, "%0.1f", widgetDir[i][1]);
    slider[sliderindex[i]]->Render(string(bottomText),
                    string(topText),
                    string(widgetText1),
                    string(widgetText2),
                    "");
  }
}

void svROISlider::RenderPos()
{
  char bottomText[20];
  char topText[20];
  char widgetText1[20];
  char widgetText2[20];

  int sliderindex[3] = {0,1,2};
  for(int i=0;i<3;i++)
  {
    sprintf(bottomText, "%0.2f", pos_range[i][0]);
    sprintf(topText, "%0.2f", pos_range[i][1]);
    sprintf(widgetText1, "%0.2f", widgetPos[i][0]);
    sprintf(widgetText2, "%0.2f", widgetPos[i][1]);
    slider[sliderindex[i]]->Render(string(bottomText),
                    string(topText),
                    string(widgetText1),
                    string(widgetText2),
                    "");
  }
}

void svROISlider::RenderRec()
{
  glLineWidth(5.);
  glColor4f(font_color[0],
            font_color[1],
            font_color[2],
            0.5);
  glBegin(GL_LINE_LOOP);
  for(int i=0;i<recP.size();i++)
  {
      glVertex2f(recP[i][0], recP[i][1]);
  }
  glEnd();
  glLineWidth(1.);

  glBegin(GL_LINES);
  glVertex2f((float)recW/5., 0);
  glVertex2f((float)recW/5., recH);
  glEnd();

  glBegin(GL_LINES);
  glVertex2f((float)recW/5., (float)recH/3.);
  glVertex2f(0-recH/10., (float)recH/3.);
  glEnd();

  glBegin(GL_LINES);
  glVertex2f((float)recW/5., (float)recH*2./3.);
  glVertex2f(0-recH/10., (float)recH*2./3.);
  glEnd();

  glColor4f(0.5,0.5,0.5,0.5);
  glBegin(GL_POLYGON);
  for(int i=0;i<recP.size();i++)
  {
      glVertex2f(recP[i][0], recP[i][1]);
  }
  glEnd();
}

void svROISlider::RenderText()
{
   char str[20];
   sprintf(str, "Magnitude");
   if(selectMag)
       glColor3f(254./255.,178./255.,76./255.);
   else
       glColor3f(font_color[0], font_color[1], font_color[2]);
   glRasterPos2f(mag_text[0], mag_text[1]);
   for(int j=0;j<strlen(str);j++)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

   sprintf(str, "Orientation");
   if(selectVel)
       glColor3f(254./255.,178./255.,76./255.);
   else
       glColor3f(font_color[0], font_color[1], font_color[2]);
   glRasterPos2f(dir_text[0], dir_text[1]);
   for(int j=0;j<strlen(str);j++)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

   sprintf(str, "Space");
   if(selectPos)
       glColor3f(254./255.,178./255.,76./255.);
   else
       glColor3f(font_color[0], font_color[1], font_color[2]);
   glRasterPos2f(pos_text[0], pos_text[1]);
   for(int j=0;j<strlen(str);j++)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
}

void svROISlider::UpdateState()
{
    state->UpdateMag(widgetMag[1], widgetMag[0]);
    state->UpdateAngle(widgetDir[0][1], widgetDir[1][1], widgetDir[2][1],
                        widgetDir[0][0], widgetDir[1][0], widgetDir[2][0]);
    state->UpdatePos(widgetPos[0][1], widgetPos[1][1], widgetPos[2][1],
                        widgetPos[0][0], widgetPos[1][0], widgetPos[2][0]);
}

}

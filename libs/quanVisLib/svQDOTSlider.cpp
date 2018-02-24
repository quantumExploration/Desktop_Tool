#include "svQDOTSlider.h"

namespace __svl_lib {
  
  
svQDOTSlider::svQDOTSlider()
{
  magSlider = new svSlider();
  vxSlider = new svSlider();
  vySlider = new svSlider();
  vzSlider = new svSlider();
  pxSlider = new svSlider();
  pySlider = new svSlider();
  pzSlider = new svSlider();

  degreeMag = 0.1;
  
  rangeVX[0]=0; rangeVX[1]=180;
  rangeVY[0]=0; rangeVY[1]=180;
  rangeVZ[0]=0; rangeVZ[1]=180;
 
  degreeVX = 5;
  degreeVY = 5;
  degreeVZ = 5;
 
  degreePX = 2;
  degreePY = 2;
  degreePZ = 2;

  selectMag = false;
  selectVel = false;
  selectPos = true; 

	font_color[0] = 0;
	font_color[1] = 0;
	font_color[2] = 0;
        font_color[3] = 0.5;
   setWidgetVel();
   setWidgetPos();
}

void svQDOTSlider::setMag(svScalar minM, svScalar maxM)
{
   rangeMag[0] = minM;
   rangeMag[1] = maxM;

   setWidgetMag();
}

void svQDOTSlider::setBoundary(svVector3 lb, svVector3 rb)
{
   rangePX[0] = lb[0]; rangePX[1] = rb[0];
   rangePY[0] = lb[1]; rangePY[1] = rb[1];
   rangePZ[0] = lb[2]; rangePZ[1] = rb[2];
   setWidgetPos();
}

void svQDOTSlider::setWidgetMag()
{
   svScalar top = magSlider->getTop();
   svScalar bottom = magSlider->getDown();

   svScalar logMag[2];
   if(rangeMag[0]>0)
   {
      logMag[0]=log10(rangeMag[0]);
   }
   else
   {
      logMag[0] = 0;
   }
   if(rangeMag[1]>0)
   {
     logMag[1] = log10(rangeMag[1]);
   }
   else
   {
     logMag[1] = 0;
   }
   svScalar logValueMag[2];
   logValueMag[0] = logMag[0] + degreeMag *
         (svScalar)((int)(bottom * (logMag[1] - logMag[0])/degreeMag));
   logValueMag[1] = logMag[0] + degreeMag *
         (svScalar)((int)(top * (logMag[1] - logMag[0])/degreeMag));

   if(fabs(bottom-0.)<1e-18)
   {
     logValueMag[0] = logMag[0];
   }
   if(fabs(top-1.)<1e-18)
   {
     logValueMag[1] = logMag[1];
   }
   
   widgetMag[0] = pow(10., logValueMag[0]);
   widgetMag[1] = pow(10., logValueMag[1]);
}

void svQDOTSlider::setWidgetVel()
{
   svScalar topX = vxSlider->getTop();
   svScalar bottomX = vxSlider->getDown();
   svScalar topY = vySlider->getTop();
   svScalar bottomY = vySlider->getDown();
   svScalar topZ = vzSlider->getTop();
   svScalar bottomZ = vzSlider->getDown();

   svScalar dx = rangeVX[1] - rangeVX[0];
   svScalar dy = rangeVY[1] - rangeVY[0];
   svScalar dz = rangeVZ[1] - rangeVZ[0];

   widgetVX[0] = (int)(bottomX * dx);
   widgetVX[1] = (int)(topX * dx);
   widgetVY[0] = (int)(bottomY * dy);
   widgetVY[1] = (int)(topY * dy);
   widgetVZ[0] = (int)(bottomZ * dz);
   widgetVZ[1] = (int)(topZ * dz);
}

void svQDOTSlider::setWidgetPos()
{
   svScalar topX = pxSlider->getTop();
   svScalar bottomX = pxSlider->getDown();
   svScalar topY = pySlider->getTop();
   svScalar bottomY = pySlider->getDown();
   svScalar topZ = pzSlider->getTop();
   svScalar bottomZ = pzSlider->getDown();

   svScalar dx = rangePX[1] - rangePX[0];
   svScalar dy = rangePY[1] - rangePY[0];
   svScalar dz = rangePZ[1] - rangePZ[0];

   widgetPX[0] = rangePX[0] + (bottomX * dx);
   widgetPX[1] = rangePX[0] + (topX * dx);
   widgetPY[0] = rangePY[0] + (bottomY * dy);
   widgetPY[1] = rangePY[0] + (topY * dy);
   widgetPZ[0] = rangePZ[0] + (bottomZ * dz);
   widgetPZ[1] = rangePZ[0] + (topZ * dz);
}

void svQDOTSlider::initRec(int h, int w)
{
   recH = h; 
   recW = w;

   double r = (double)recH/10.;

   svVector3 p;
   p[0] = 0;
   p[1] = r;
   for(int i=0;i<=5;i++)
   {
     svVector3 tmp;
     tmp[0] = p[0] + r * cos(M_PI/10. * (double)i + M_PI);
     tmp[1] = p[1] + r * sin(M_PI/10. * (double)i + M_PI);
     tmp[2] = 0;
     recP.push_back(tmp);
   }

   p[0] = w - r;
   p[1] = r;
   for(int i=0;i<=5;i++)
   {
     svVector3 tmp;
     tmp[0] = p[0] + r * cos(M_PI/10. * (double)i + M_PI*1.5);
     tmp[1] = p[1] + r * sin(M_PI/10. * (double)i + M_PI*1.5);
     tmp[2] = 0;
     recP.push_back(tmp);
   }

   p[0] = w - r;
   p[1] = h - r;
   for(int i=0;i<=5;i++)
   {
     svVector3 tmp;
     tmp[0] = p[0] + r * cos(M_PI/10. * (double)i);
     tmp[1] = p[1] + r * sin(M_PI/10. * (double)i);
     tmp[2] = 0;
     recP.push_back(tmp);
   }

   p[0] = 0;
   p[1] = h-r;
   for(int i=0;i<=5;i++)
   {
     svVector3 tmp;
     tmp[0] = p[0] + r * cos(M_PI/10. * (double)i + M_PI/2.);
     tmp[1] = p[1] + r * sin(M_PI/10. * (double)i + M_PI/2.);
     tmp[2] = 0;
     recP.push_back(tmp);
   }

   textW = recW/5.+r;
   svScalar x = (recW+r*2 - textW);
   textW = -r + recW/5.;   

   shiftMag[0] = textW + (svScalar)x/2.;
   shiftMag[1] = (svScalar)recH/10.;
   shiftVX[0] = textW + (svScalar)x/6.;
   shiftVX[1] = shiftMag[1];
   shiftVY[0] = textW + (svScalar)x/2.-r/2.;
   shiftVY[1] = shiftMag[1];
   shiftVZ[0] = textW + (svScalar)x*5./6.-r;
   shiftVZ[1] = shiftMag[1];
   shiftPX = shiftVX;
   shiftPY = shiftVY;
   shiftPZ = shiftVZ;

   scaleSize = ((svScalar)recH - (svScalar)recH/6.)/100.;
}

void svQDOTSlider::setRec(svVector3 pos)
{
   recShift = getProject(pos);
   recShift[1] = image_height - recShift[1];

   if(recShift[0] > image_width - recW)
   {
        recShift[0] = recShift[0] - recW;
   }
   if(recShift[0] < image_pos_x)
   {
        recShift[0] = recShift[0] + recW;
   }
   if(recShift[1] > image_height - recH)
   {
        recShift[1] = recShift[1] - recH;
   }
   if(recShift[1] < 0)
   {
        recShift[1] = recShift[1] + recH;
   }
}

void svQDOTSlider::RenderMag()
{
   glPushMatrix();

   glTranslatef(shiftMag[0], shiftMag[1], 0);
   glScalef(scaleSize, scaleSize, 1);
   char bottomText[20];
   char topText[20];
   char widgetText1[20];
   char widgetText2[20];

   sprintf(bottomText, "%6.2e", rangeMag[0]);
   sprintf(topText, "%6.2e", rangeMag[1]);
   sprintf(widgetText1, "%6.2e", widgetMag[0]);
   sprintf(widgetText2, "%6.2e", widgetMag[1]);

   magSlider->Render(bottomText, topText, widgetText1, widgetText2, "");

   glPopMatrix();
}

void svQDOTSlider::RenderVel()
{
   char bottomText[20];
   char topText[20];
   char widgetText1[20];
   char widgetText2[20];

   glPushMatrix();
   glTranslatef(shiftVX[0], shiftVX[1], 1);
   glScalef(scaleSize, scaleSize, 1);
   sprintf(bottomText, "%.0f", rangeVX[0]);
   sprintf(topText, "%.0f", rangeVX[1]);
   sprintf(widgetText1, "%.0f", widgetVX[0]);
   sprintf(widgetText2, "%.0f", widgetVX[1]);
   vxSlider->Render(bottomText, topText, widgetText1, widgetText2, "X");
   glPopMatrix();

   glPushMatrix();
   glTranslatef(shiftVY[0], shiftVY[1], 1);
   glScalef(scaleSize, scaleSize, 1);
   sprintf(bottomText, "%.0f", rangeVY[0]);
   sprintf(topText, "%.0f", rangeVY[1]);
   sprintf(widgetText1, "%.0f", widgetVY[0]);
   sprintf(widgetText2, "%.0f", widgetVY[1]);
   vySlider->Render(bottomText, topText, widgetText1, widgetText2, "Y");
   glPopMatrix();

   glPushMatrix();
   glTranslatef(shiftVZ[0], shiftVZ[1], 1);
   glScalef(scaleSize, scaleSize, 1);
   sprintf(bottomText, "%.0f", rangeVZ[0]);
   sprintf(topText, "%.0f", rangeVZ[1]);
   sprintf(widgetText1, "%.0f", widgetVZ[0]);
   sprintf(widgetText2, "%.0f", widgetVZ[1]);
   vzSlider->Render(bottomText, topText, widgetText1, widgetText2, "Z");
   glPopMatrix();
}

void svQDOTSlider::RenderPos()
{
   char bottomText[20];
   char topText[20];
   char widgetText1[20];
   char widgetText2[20];

   glPushMatrix();
   glTranslatef(shiftPX[0], shiftPX[1], 1);
   glScalef(scaleSize, scaleSize, 1);
   sprintf(bottomText, "%.2f", rangePX[0]);
   sprintf(topText, "%.2f", rangePX[1]);
   sprintf(widgetText1, "%.2f", widgetPX[0]);
   sprintf(widgetText2, "%.2f", widgetPX[1]);
   pxSlider->Render(bottomText, topText, widgetText1, widgetText2, "X");
   glPopMatrix();

   glPushMatrix();
   glTranslatef(shiftPY[0], shiftPY[1], 1);
   glScalef(scaleSize, scaleSize, 1);
   sprintf(bottomText, "%.2f", rangePY[0]);
   sprintf(topText, "%.2f", rangePY[1]);
   sprintf(widgetText1, "%.2f", widgetPY[0]);
   sprintf(widgetText2, "%.2f", widgetPY[1]);
   pySlider->Render(bottomText, topText, widgetText1, widgetText2, "Y");
   glPopMatrix();

   glPushMatrix();
   glTranslatef(shiftPZ[0], shiftPZ[1], 1);
   glScalef(scaleSize, scaleSize, 1);
   sprintf(bottomText, "%.2f", rangePZ[0]);
   sprintf(topText, "%.2f", rangePZ[1]);
   sprintf(widgetText1, "%.2f", widgetPZ[0]);
   sprintf(widgetText2, "%.2f", widgetPZ[1]);
   pzSlider->Render(bottomText, topText, widgetText1, widgetText2, "Z");
   glPopMatrix();
}

void svQDOTSlider::RenderRec()
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

void svQDOTSlider::RenderText()
{
   char str[20];
   sprintf(str, "Magnitude");
   if(selectMag)
       glColor3f(254./255.,178./255.,76./255.);
   else
       glColor3f(font_color[0], font_color[1], font_color[2]);
   glRasterPos2f(-recH/10.+10.,(float)recH*2./3.+(float)recH/8.);
   for(int j=0;j<strlen(str);j++)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

   sprintf(str, "Orientation");
   if(selectVel)
       glColor3f(254./255.,178./255.,76./255.);
   else
       glColor3f(font_color[0], font_color[1], font_color[2]);
   glRasterPos2f(-recH/10.+10.,(float)recH/3.+(float)recH/8.);
   for(int j=0;j<strlen(str);j++)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);

   sprintf(str, "Space");
   if(selectPos)
       glColor3f(254./255.,178./255.,76./255.);
   else
       glColor3f(font_color[0], font_color[1], font_color[2]);
   glRasterPos2f(-recH/10.+10.,(float)recH/8.);
   for(int j=0;j<strlen(str);j++)
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[j]);
}

void svQDOTSlider::Render()
{
   glEnable(GL_BLEND);

   glPushMatrix();
   glTranslatef(recShift[0], recShift[1], 0);

   RenderRec();    
   RenderText();  
 
   if(selectMag)
   {
        RenderMag();
   }
   else if(selectVel)
   {
        RenderVel();
   } 
   else if(selectPos)
   {
        RenderPos();
   }
        
   glPopMatrix();          
}

void svQDOTSlider::Mouse(int x, int y)
{
   if(x >= recShift[0] && x <= recShift[0] + (float)recW/4.
    &&y >= recShift[1] && y <= recShift[1]+(float)recH)
   {
      if(y <= recShift[1] + (float)recH/3.)
      {
         selectPos = true;
         selectMag = false;
         selectVel = false;
      }
      else if(y <= recShift[1] + (float)recH *2./3.)
      {
         selectPos = false;
         selectMag = false;
         selectVel = true;
      }
      else
      {
         selectMag = true;
         selectVel = false;
         selectPos = false;
      }
   }
}

void svQDOTSlider::widgetMouse(int x, int y)
{
   x = x - recShift[0];
   y = y - recShift[1];

   if(selectMag)
   {
      x = x - shiftMag[0];
      y = y - shiftMag[1];
      x = x/scaleSize;
      y = y/scaleSize;
      magSlider->Mouse(x,y);
      setWidgetMag();
   }
   else if(selectVel)
   {
      int x1, y1;
      x1 = x - shiftVX[0];
      y1 = y - shiftVX[1];
      x1 = x1/scaleSize;
      y1 = y1/scaleSize;
      vxSlider->Mouse(x1,y1);
      int x2, y2;
      x2 = x - shiftVY[0];
      y2 = y - shiftVY[1];
      x2= x2/scaleSize;
      y2 = y2/scaleSize;
      vySlider->Mouse(x2,y2);
      int x3, y3;
      x3 = x - shiftVZ[0];
      y3 = y - shiftVZ[1];
      x3 = x3/scaleSize;
      y3 = y3/scaleSize;
      vzSlider->Mouse(x3,y3);
      setWidgetVel();
   }
   else if(selectPos)
   {
      int x1, y1;
      x1 = x- shiftPX[0];
      y1 = y- shiftPX[1];
      x1= x1/scaleSize;
      y1 = y1/scaleSize;
      pxSlider->Mouse(x1,y1);
      int x2, y2;
      x2 = x - shiftPY[0];
      y2 = y - shiftPY[1];
      x2 = x2/scaleSize;
      y2 = y2/scaleSize;
      pySlider->Mouse(x2,y2);
      int x3, y3;
      x3 = x - shiftPZ[0]; 
      y3 = y - shiftPZ[1];
      x3 = x3/scaleSize;
      y3 = y3/scaleSize;
      pzSlider->Mouse(x3,y3);
      setWidgetPos();
   }
}

void svQDOTSlider::widgetMotion(int x, int y)
{
   y = y - recShift[1];

   if(selectMag)
   {
      y = y - shiftMag[1];
      y = y /scaleSize;
      magSlider->Motion(y);
      setWidgetMag();
   }
   else if(selectVel)
   {
      int y1;
      y1 = y - shiftVX[1];
      y1 = y1/scaleSize;
      vxSlider->Motion(y1);
      int y2;
      y2 = y - shiftVY[1];
      y2 = y2/scaleSize;
      vySlider->Motion(y2);
      int y3;
      y3 = y - shiftVZ[1];
      y3 = y3/scaleSize;
      vzSlider->Motion(y3);
      setWidgetVel();
   }
   else if(selectPos)
   {
      int y1;
      y1 = y - shiftPX[1];
      y1 = y1/scaleSize;
      pxSlider->Motion(y1);
      int y2;
      y2 = y - shiftPY[1];
      y2 = y2/scaleSize;
      pySlider->Motion(y2);
      int y3;
      y3 = y - shiftPZ[1];
      y3 = y3/scaleSize;
      pzSlider->Motion(y3);
      setWidgetPos();
   } 
}
void svQDOTSlider::widgetMovement(int x, int y)
{
   x = x - recShift[0];
   y = y - recShift[1];

   if(selectMag)
   {
      x = x - shiftMag[0];
      y = y - shiftMag[1];
      x = x/scaleSize;
      y = y/scaleSize;
      magSlider->Movement(x,y);
   }
   else if(selectVel)
   {
      int x1, y1;
      x1 = x - shiftVX[0];
      y1 = y - shiftVX[1];
      x1 = x1/scaleSize;
      y1 = y1/scaleSize;
      vxSlider->Movement(x1,y1);
      int x2, y2;
      x2 = x - shiftVY[0];
      y2 = y - shiftVY[1];
      x2 = x2/scaleSize;
      y2 = y2/scaleSize;
      vySlider->Movement(x2,y2);
      int x3, y3;
      x3 = x - shiftVZ[0];
      y3 = y - shiftVZ[1];
      x3 = x3/scaleSize;
      y3 = y3/scaleSize;
      vzSlider->Movement(x3,y3);
   }
   else if(selectPos)
   {
      int x1, y1;
      x1 = x- shiftPX[0];
      y1 = y- shiftPX[1];
      x1 = x1 / scaleSize;
      y1 = y1 / scaleSize;
      pxSlider->Movement(x1,y1);
      int x2, y2;
      x2 = x - shiftPY[0];
      y2 = y - shiftPY[1];
      x2 = x2/scaleSize;
      y2 = y2/scaleSize;
      pySlider->Movement(x2,y2);
      int x3, y3;
      x3 = x - shiftPZ[0];
      y3 = y - shiftPZ[1];
      x3 = x3/scaleSize;
      y3 = y3/scaleSize;
      pzSlider->Movement(x3,y3);
   }
}


/*
void svSlider::Reshape_Widget(int w, int h)
{
	image_height = h;
	image_width = w;
	
	Widget_bound_z[0][0] = 50.+50.+50,;
	Widget_bound_z[0][1] = 50.;
	Widget_bound_z[1][0] = Widget_bound_z[0][0];
	Widget_bound_z[1][1] = h/2.;

//============
        Widget_bound_x[0][0] = 50. + 50.;
        Widget_bound_x[0][1] = 50.;
        Widget_bound_x[1][0] = Widget_bound_x[0][0];
        Widget_bound_x[1][1] = h/2.;

//====================
        Widget_bound_y[0][0] = 50.;
        Widget_bound_y[0][1] = 50.;
        Widget_bound_y[1][0] = Widget_bound_y[0][0];
        Widget_bound_y[1][1] = h/2.;

//==================

	Widget_bound_mag[0][0] = w-50.;//w-350.;
	Widget_bound_mag[0][1] = 50.;
	Widget_bound_mag[1][0] = Widget_bound_mag[0][0];
	Widget_bound_mag[1][1] = h/2.;


  	SetWidget();
}


void svSlider::RenderSlider(svVector3 bottomPos, svVector3 topPos,
                    char *bottomText, char *topText,
                    svVector3 widgetPos1, svVector3 widgetPos2,
                    char *widgetText1, char *widgetText2,
                    svVector3 widgetColor1, svVector3 widgetColor2,
                    char *subject)
{
    glColor3f(1-font_color[0],1-font_color[1],1-font_color[2]);
    glLineWidth(3.);
    glBegin(GL_LINES);
    glVertex2f(bottomPos[0],bottomPos[1]);
    glVertex2f(topPos[0], topPos[1]);
    glEnd();

    glColor3f(font_color[0],font_color[1],font_color[2]);
    glLineWidth(2.);
    glBegin(GL_LINES);
    glVertex2f(bottomPos[0],bottomPos[1]);
    glVertex2f(topPos[0], topPos[1]);
    glEnd();

    glRasterPos2f(bottomPos[0]+2., bottomPos[1]);
    for(int j=0;j<strlen(bottomText);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, bottomText[j]);
    }
    glRasterPos2f(topPos[0]+2., topPos[1]);
    for(int j=0;j<strlen(topText);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, topText[j]);
    }
    glRasterPos2f(widgetPos1[0]+2., widgetPos1[1]);
    for(int j=0;j<strlen(widgetText1);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, widgetText1[j]);
    }
    glRasterPos2f(widgetPos2[0]+2., widgetPos2[1]);
    for(int j=0;j<strlen(widgetText2);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, widgetText2[j]);
    }
    glRasterPos2f(topPos[0]-10, topPos[1]+30);
    for(int j=0;j<strlen(subject);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, subject[j]);
    }

    glColor3f(1-font_color[0],1-font_color[1],1-font_color[2]);
    glBegin(GL_LINE_LOOP);
    glVertex2f(widgetText1[0]-10.,widgetText1[1]-5.);
    glVertex2f(widgetText1[0]+10.,widgetText1[1]-5.);
    glVertex2f(widgetText1[0]+10.,widgetText1[1]+5.);
    glVertex2f(widgetText1[0]-10.,widgetText1[1]+5.);
    glEnd();
    glColor4f(widgetColor1[0],widgetColor1[1],widgetColor1[2],widgetColor1[3]);
    glBegin(GL_QUADS);
    glVertex2f(widgetText1[0]-10.,widgetText1[1]-5.);
    glVertex2f(widgetText1[0]+10.,widgetText1[1]-5.);
    glVertex2f(widgetText1[0]+10.,widgetText1[1]+5.);
    glVertex2f(widgetText1[0]-10.,widgetText1[1]+5.);
    glEnd();

    glColor3f(1-font_color[0],1-font_color[1],1-font_color[2]);
    glBegin(GL_LINE_LOOP);
    glVertex2f(widgetText2[0]-10.,widgetText2[1]-5.);
    glVertex2f(widgetText2[0]+10.,widgetText2[1]-5.);
    glVertex2f(widgetText2[0]+10.,widgetText2[1]+5.);
    glVertex2f(widgetText2[0]-10.,widgetText2[1]+5.);
    glEnd();
    glColor4f(widgetColor2[0],widgetColor2[1],widgetColor2[2],widgetColor2[3]);
    glBegin(GL_QUADS);
    glVertex2f(widgetText2[0]-10.,widgetText2[1]-5.);
    glVertex2f(widgetText2[0]+10.,widgetText2[1]-5.);
    glVertex2f(widgetText2[0]+10.,widgetText2[1]+5.);
    glVertex2f(widgetText2[0]-10.,widgetText2[1]+5.);
    glEnd();

}

void svSlider::Widget_Mag()
{
    char bottomText[20], topText[20];
    sprintf(bottomText, "%6.2e", min_density);
    sprintf(topText, "%6.2e", max_density);
    char text1[20], text2[20];
    sprintf(text1, "%6.2e", range_mag[0]);
    sprintf(text2, "%6.2e", range_mag[1]);

    RenderSlider(Widget_bound_mag[0], Widget_bound_mag[1],
                bottomText, topText,
                Widget_mag[0], Widget_mag[1],
                text1, text2,
                magColor[0], magColor[1],
                "Magnitude");
}

void svSlider::Widget_Z()
{
    char text1[20], text2[20];
    sprintf(text1, "%.2f", zangle_min[0]);
    sprintf(text2, "%.2f", zangle_max[1]);

    RenderSlider(Widget_bound_z[0], Widget_bound_z[1],
                "0", "180",
                Widget_z[0], Widget_z[1],
                text1, text2,
                zColor[0], zColor[1],
                "Positive Z");
}

void svSlider::Widget_X()
{
    char text1[20], text2[20];
    sprintf(text1, "%.2f", xangle_min[0]);
    sprintf(text2, "%.2f", xangle_max[1]);

    RenderSlider(Widget_bound_x[0], Widget_bound_x[1],
                "0", "180",
                Widget_x[0], Widget_x[1],
                text1, text2,
                xColor[0], xColor[1],
                "Positive X");
}


void svSlider::Widget_Y()
{
    char text1[20], text2[20];
    sprintf(text1, "%.2f", yangle_min[0]);
    sprintf(text2, "%.2f", yangle_max[1]);

    RenderSlider(Widget_bound_y[0], Widget_bound_y[1],
                "0", "180",
                Widget_y[0], Widget_y[1],
                text1, text2,
                yColor[0], yColor[1],
                "Positive Y");
}

void svSlider::Mouse(int x, int y)
{
       for(int i=0;i<2;i++)
       {
         magColor[i] = font_color;
         zColor[i] = font_color;
         xColor[i] = font_color;
         yColor[i] = font_color;
       }

       svVector4 color;
       color[0]=253./255.;
       color[1]= 174./255.;
       color[2]= 97./255.;
       color[3]=0.5;

	if(x>=Widget_mag[0][0]-10.&&x<=Widget_mag[0][0]+10.
        && y>=Widget_mag[0][1]-5.&&y<=Widget_mag[0][1]+5.)
        {
             magColor[0] = color;
             magSelect[0] = true;
        }
        else if(x>=Widget_mag[1][0]-10.&&x<=Widget_mag[1][0]+10.
        && y>=Widget_mag[1][1]-5.&&y<=Widget_mag[1][1]+5.)
        {
             magColor[1]=color;
             magSelect[1] = true;
        }
        else if(x>=Widget_z[0][0]-10.&&x<=Widget_z[0][0]+10.
        && y>=Widget_z[0][1]-5.&&y<=Widget_z[0][1]+5.)
        {
             zColor[0] = color;
             zSelect[0] = true;
        }
        else if(x>=Widget_z[1][0]-10.&&x<=Widget_z[1][0]+10.
        && y>=Widget_z[1][1]-5.&&y<=Widget_z[1][1]+5.)
        {
             zColor[1]= color;
             zSelect[1] = true;
        }
        else if(x>=Widget_x[0][0]-10.&&x<=Widget_x[0][0]+10.
        && y>=Widget_x[0][1]-5.&&y<=Widget_x[0][1]+5.)
        {
             xColor[0]= color;
             xSelect[0] = true;
        }
        else if(x>=Widget_x[1][0]-10.&&x<=Widget_x[1][0]+10.
        && y>=Widget_x[1][1]-5.&&y<=Widget_x[1][1]+5.)
        {
             xColor[1]= color;
             xSelect[1] = true;
        }
        else if(x>=Widget_y[0][0]-10.&&x<=Widget_y[0][0]+10.
        && y>=Widget_y[0][1]-5.&&y<=Widget_y[0][1]+5.)
        {
             yColor[0]= color;
             ySelect[0] = true;
        }
        else if(x>=Widget_y[1][0]-10.&&x<=Widget_y[1][0]+10.
        && y>=Widget_y[1][1]-5.&&y<=Widget_y[1][1]+5.)
        {
             yColor[1]= color;
             ySelect[1] = true;
        }
}

void svSlider::SetMagValues(int y, int index)
{
      if(index == 0)
      {
         if(y>=Widget_bound_mag[0][1] && y<Widget_mag[1][1])
         {
              Widget_mag[0][1] = y;
         }
      } 
      else
      {
         if(y>Widget_mag[0][1] && y<=Widget_bound_mag[1][1])
         {
              Widget_mag[1][1] = y;
         }
      }
//===================================================================
        double mag_distance;
        if(min_density>0)
                mag_distance = log10(max_density)  - log10(min_density);
        else
                mag_distance = fabs(log10(max_density));

        svScalar d = fabs(Widget_bound_mag[1][1]-Widget_bound_mag[0][1]);
        svScalar bottomD = fabs(Widget_mag[0][1] - Widget_bound_mag[0][1]);
        svScalar topD = fabs(Widget_mag[1][1] - Widget_bound_mag[0][1]);

        range_mag[0] = bottomD/d;
        range_mag[0] = pow(10.,range_mag[0] * mag_distance) + min_density;
        range_mag[1] = topD/d;
        range_mag[1] = pow(10.,range_mag[1] * mag_distance) + min_density; 
}

void svSlider::SetZValues(int y, int index)
{
      if(index == 0)
      {
         if(y>=Widget_bound_z[0][1] && y<Widget_z[1][1])
         {
              Widget_z[0][1] = y;
         }
      }
      else
      {
         if(y>Widget_z[0][1] && y<=Widget_bound_z[1][1])
         {
              Widget_z[1][1] = y;
         }
      }
//===================================================================
      double z_distance = 180.;
      svScalar d = fabs(Widget_bound_z[1][1]-Widget_bound_z[0][1]);
      svScalar bottomD = fabs(Widget_z[0][1] - Widget_bound_z[0][1]);
      svScalar topD = fabs(Widget_z[1][1] - Widget_bound_z[0][1]);

      range_z[0] = bottomD/d;
      range_z[0] = range_z[0] * d;
      range_z[1] = bottomD/d;
      range_z[1] = range_z[1] * d;
}

void svSlider::SetXValues(int y, int index)
{
      if(index == 0)
      {
         if(y>=Widget_bound_x[0][1] && y<Widget_x[1][1])
         {
              Widget_x[0][1] = y;
         }
      }
      else
      {
         if(y>Widget_x[0][1] && y<=Widget_bound_x[1][1])
         {
              Widget_x[1][1] = y;
         }
      }
//===================================================================
      double x_distance = 180.;
      svScalar d = fabs(Widget_bound_x[1][1]-Widget_bound_x[0][1]);
      svScalar bottomD = fabs(Widget_x[0][1] - Widget_bound_x[0][1]);
      svScalar topD = fabs(Widget_x[1][1] - Widget_bound_x[0][1]);

      range_x[0] = bottomD/d;
      range_x[0] = range_x[0] * d;
      range_x[1] = bottomD/d;
      range_x[1] = range_x[1] * d;
}

void svSlider::SetYValues(int y, int index)
{
      if(index == 0)
      {
         if(y>=Widget_bound_y[0][1] && y<Widget_y[1][1])
         {
              Widget_y[0][1] = y;
         }
      }
      else
      {
         if(y>Widget_y[0][1] && y<=Widget_bound_y[1][1])
         {
              Widget_y[1][1] = y;
         }
      }
      double y_distance = 180.;
      svScalar d = fabs(Widget_bound_y[1][1]-Widget_bound_y[0][1]);
      svScalar bottomD = fabs(Widget_y[0][1] - Widget_bound_y[0][1]);
      svScalar topD = fabs(Widget_y[1][1] - Widget_bound_y[0][1]);

      range_y[0] = bottomD/d;
      range_y[0] = range_y[0] * d;
      range_y[1] = bottomD/d;
      range_y[1] = range_y[1] * d;
}


void svSlider::Motion(int y)
{
     for(int i=0;i<2;i++)
     {
         magColor[i] = font_color;
         zColor[i] = font_color;
         xColor[i] = font_color;
         yColor[i] = font_color;
      }

   if(magSelect[0] || magSelect[1])
   {
      if(magSelect[0])
         SetMagValues(y,0);
      else
         SetMagValues(y,1);
   }
   else if(zSelect[0] || zSelect[1])
   {
      if(zSelect[0])
         SetZValues(y,0);
      else
         SetZValues(y,1);
   }
   else if(xSelect[0] || xSelect[1])
   {
      if(xSelect[0])
         SetXValues(y,0);
      else
         SetXValues(y,1);
   }
   else if(ySelect[0] || ySelect[1])
   {
      if(ySelect[0])
         SetYValues(y,0);
      else
         SetYValues(y,1);
   }
}

void svSlider::Movement(int x, int y)
{
       svVector4 color;
       color[0]=253./255.;
       color[1]= 174./255.;
       color[2]= 97./255.;
       color[3]=0.5;

        if(x>=Widget_mag[0][0]-10.&&x<=Widget_mag[0][0]+10.
        && y>=Widget_mag[0][1]-5.&&y<=Widget_mag[0][1]+5.)
        {
             magColor[0] = color;
        }
        else if(x>=Widget_mag[1][0]-10.&&x<=Widget_mag[1][0]+10.
        && y>=Widget_mag[1][1]-5.&&y<=Widget_mag[1][1]+5.)
        {
             magColor[1]=color;
        }
        else if(x>=Widget_z[0][0]-10.&&x<=Widget_z[0][0]+10.
        && y>=Widget_z[0][1]-5.&&y<=Widget_z[0][1]+5.)
        {
             zColor[0] = color;
        }
        else if(x>=Widget_z[1][0]-10.&&x<=Widget_z[1][0]+10.
        && y>=Widget_z[1][1]-5.&&y<=Widget_z[1][1]+5.)
        {
             zColor[1]= color;
        }
        else if(x>=Widget_x[0][0]-10.&&x<=Widget_x[0][0]+10.
        && y>=Widget_x[0][1]-5.&&y<=Widget_x[0][1]+5.)
        {
             xColor[0]= color;
        }
        else if(x>=Widget_x[1][0]-10.&&x<=Widget_x[1][0]+10.
        && y>=Widget_x[1][1]-5.&&y<=Widget_x[1][1]+5.)
        {
             xColor[1]= color;
        }
        else if(x>=Widget_y[0][0]-10.&&x<=Widget_y[0][0]+10.
        && y>=Widget_y[0][1]-5.&&y<=Widget_y[0][1]+5.)
        {
             yColor[0]= color;
        }
        else if(x>=Widget_y[1][0]-10.&&x<=Widget_y[1][0]+10.
        && y>=Widget_y[1][1]-5.&&y<=Widget_y[1][1]+5.)
        {
             yColor[1]= color;
        }
}

void svSlider::ResetBool()
{

		Widget_select_mag[0] = false;
		Widget_select_mag[1] = false;
		Widget_select_zangle[0]=false;
		Widget_select_zangle[1] = false;
		Widget_select_xy_whole = false;
		Widget_flag = false;
		for(int i=0;i<Widget_select_xy.size();i++)
			Widget_select_xy[i] = false;

}
*/
}


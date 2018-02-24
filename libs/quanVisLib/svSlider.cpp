#include "svSlider.h"

namespace __svl_lib {

svSlider::svSlider()
{
  font_color[0]=0;
  font_color[1]=0;
  font_color[2]=0;
  font_color[3]=0.5;

  boundWidgetPos[0][0] = 0;
  boundWidgetPos[0][1] = 0;
  boundWidgetPos[1][0] = 0;
  boundWidgetPos[1][1] = 100;

  ResetColor();

  widgetPos[0] = boundWidgetPos[0];
  widgetPos[1] = boundWidgetPos[1];
  widgetW = 20;
  widgetH = 10;
}

void svSlider::ResetColor()
{
  widgetColor[0][0] = 1-font_color[0];
  widgetColor[0][1] = 1-font_color[1];
  widgetColor[0][2] = 1-font_color[2];
  widgetColor[1][0] = 1-font_color[0];
  widgetColor[1][1] = 1-font_color[1];
  widgetColor[1][2] = 1-font_color[2];

  widgetColor[0][3] = 0.5;
  widgetColor[1][3] = 0.5;
}
/*
void svSlider::ResetBool()
{
   widgetSelect[0] = false;
   widgetSelect[1] = false;
}
*/
void svSlider::RenderDoubleSlider(string bottomText, string topText,
                       string widgetText1, string widgetText2,
                       string subject)
{
    glPushMatrix();
    glTranslatef(tranx, trany, 0);
    glScalef(scalex, scaley, 1);

    glColor3f(font_color[0],font_color[1],font_color[2]);
    glLineWidth(5.);
    glBegin(GL_LINES);
    glVertex2f(boundWidgetPos[0][0],boundWidgetPos[0][1]);
    glVertex2f(boundWidgetPos[1][0],boundWidgetPos[1][1]);
    glEnd();

    glColor3f(1-font_color[0],1-font_color[1],1-font_color[2]);
    glLineWidth(2.);
    glBegin(GL_LINES);
    glVertex2f(boundWidgetPos[0][0],boundWidgetPos[0][1]);
    glVertex2f(boundWidgetPos[1][0],boundWidgetPos[1][1]);
    glEnd();

    glColor3f(font_color[0],font_color[1],font_color[2]);

    glRasterPos2f(boundWidgetPos[0][0]+11., boundWidgetPos[0][1]-2);
    for(int j=0;j<strlen(bottomText);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, bottomText[j]);
    }
    glRasterPos2f(boundWidgetPos[1][0]+11., boundWidgetPos[1][1]-2);
    for(int j=0;j<strlen(topText);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, topText[j]);
    }
    glRasterPos2f(widgetPos[0][0]+11., widgetPos[0][1]-2);
    for(int j=0;j<strlen(widgetText1);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, widgetText1[j]);
    }
    glRasterPos2f(widgetPos[1][0]+11., widgetPos[1][1]-2);
    for(int j=0;j<strlen(widgetText2);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, widgetText2[j]);
    }
    glRasterPos2f(boundWidgetPos[0][0]-18., boundWidgetPos[1][1]);
    for(int j=0;j<strlen(subject);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, subject[j]);
    }

    glColor3f(font_color[0],font_color[1],font_color[2]);
    glBegin(GL_LINE_LOOP);
    glVertex2f(widgetPos[0][0]-10.,widgetPos[0][1]-5.);
    glVertex2f(widgetPos[0][0]+10.,widgetPos[0][1]-5.);
    glVertex2f(widgetPos[0][0]+10.,widgetPos[0][1]+5.);
    glVertex2f(widgetPos[0][0]-10.,widgetPos[0][1]+5.);
    glEnd();
    glColor4f(widgetColor[0][0],widgetColor[0][1],
              widgetColor[0][2],widgetColor[0][3]);
    glBegin(GL_QUADS);
    glVertex2f(widgetPos[0][0]-10.,widgetPos[0][1]-5.);
    glVertex2f(widgetPos[0][0]+10.,widgetPos[0][1]-5.);
    glVertex2f(widgetPos[0][0]+10.,widgetPos[0][1]+5.);
    glVertex2f(widgetPos[0][0]-10.,widgetPos[0][1]+5.);
    glEnd();

    glColor3f(font_color[0],font_color[1],font_color[2]);
    glBegin(GL_LINE_LOOP);
    glVertex2f(widgetPos[1][0]-10.,widgetPos[1][1]-5.);
    glVertex2f(widgetPos[1][0]+10.,widgetPos[1][1]-5.);
    glVertex2f(widgetPos[1][0]+10.,widgetPos[1][1]+5.);
    glVertex2f(widgetPos[1][0]-10.,widgetPos[1][1]+5.);
    glEnd();
    glColor4f(widgetColor[1][0],widgetColor[1][1],
              widgetColor[1][2],widgetColor[1][3]);
    glBegin(GL_QUADS);
    glVertex2f(widgetPos[1][0]-10.,widgetPos[1][1]-5.);
    glVertex2f(widgetPos[1][0]+10.,widgetPos[1][1]-5.);
    glVertex2f(widgetPos[1][0]+10.,widgetPos[1][1]+5.);
    glVertex2f(widgetPos[1][0]-10.,widgetPos[1][1]+5.);
    glEnd();

    glPopMatrix();
}

void svSlider::Render(string bottomText, string topText,
                       string widgetText1, string widgetText2,
                       string subject)
{
  if(isDouble) RenderDoubleSlider(bottomText, topText, widgetText1, widgetText2, subject)
  else RenderSingleSlider(bottomText, topText, widgetText1, subject);
}

void svSlider::RenderDoubleSlider(string bottomText, string topText,
                       string widgetText1, string widgetText2,
                       string subject)
{
    glPushMatrix();
    glTranslatef(tranx, trany, 0);
    glScalef(scalex, scaley, 1);

    glColor3f(font_color[0],font_color[1],font_color[2]);
    glLineWidth(5.);
    glBegin(GL_LINES);
    glVertex2f(boundWidgetPos[0][0],boundWidgetPos[0][1]);
    glVertex2f(boundWidgetPos[1][0],boundWidgetPos[1][1]);
    glEnd();

    glColor3f(1-font_color[0],1-font_color[1],1-font_color[2]);
    glLineWidth(2.);
    glBegin(GL_LINES);
    glVertex2f(boundWidgetPos[0][0],boundWidgetPos[0][1]);
    glVertex2f(boundWidgetPos[1][0],boundWidgetPos[1][1]);
    glEnd();

    glColor3f(font_color[0],font_color[1],font_color[2]);

    glRasterPos2f(boundWidgetPos[0][0]+11., boundWidgetPos[0][1]-2);
    for(int j=0;j<strlen(bottomText);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, bottomText[j]);
    }
    glRasterPos2f(boundWidgetPos[1][0]+11., boundWidgetPos[1][1]-2);
    for(int j=0;j<strlen(topText);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, topText[j]);
    }
    glRasterPos2f(widgetPos[0][0]+11., widgetPos[0][1]-2);
    for(int j=0;j<strlen(widgetText1);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, widgetText1[j]);
    }
    glRasterPos2f(widgetPos[1][0]+11., widgetPos[1][1]-2);
    for(int j=0;j<strlen(widgetText2);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, widgetText2[j]);
    }
    glRasterPos2f(boundWidgetPos[0][0]-18., boundWidgetPos[1][1]);
    for(int j=0;j<strlen(subject);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, subject[j]);
    }

    glColor3f(font_color[0],font_color[1],font_color[2]);
    glBegin(GL_LINE_LOOP);
    glVertex2f(widgetPos[0][0]-10.,widgetPos[0][1]-5.);
    glVertex2f(widgetPos[0][0]+10.,widgetPos[0][1]-5.);
    glVertex2f(widgetPos[0][0]+10.,widgetPos[0][1]+5.);
    glVertex2f(widgetPos[0][0]-10.,widgetPos[0][1]+5.);
    glEnd();
    glColor4f(widgetColor[0][0],widgetColor[0][1],
              widgetColor[0][2],widgetColor[0][3]);
    glBegin(GL_QUADS);
    glVertex2f(widgetPos[0][0]-10.,widgetPos[0][1]-5.);
    glVertex2f(widgetPos[0][0]+10.,widgetPos[0][1]-5.);
    glVertex2f(widgetPos[0][0]+10.,widgetPos[0][1]+5.);
    glVertex2f(widgetPos[0][0]-10.,widgetPos[0][1]+5.);
    glEnd();

    glColor3f(font_color[0],font_color[1],font_color[2]);
    glBegin(GL_LINE_LOOP);
    glVertex2f(widgetPos[1][0]-10.,widgetPos[1][1]-5.);
    glVertex2f(widgetPos[1][0]+10.,widgetPos[1][1]-5.);
    glVertex2f(widgetPos[1][0]+10.,widgetPos[1][1]+5.);
    glVertex2f(widgetPos[1][0]-10.,widgetPos[1][1]+5.);
    glEnd();
    glColor4f(widgetColor[1][0],widgetColor[1][1],
              widgetColor[1][2],widgetColor[1][3]);
    glBegin(GL_QUADS);
    glVertex2f(widgetPos[1][0]-10.,widgetPos[1][1]-5.);
    glVertex2f(widgetPos[1][0]+10.,widgetPos[1][1]-5.);
    glVertex2f(widgetPos[1][0]+10.,widgetPos[1][1]+5.);
    glVertex2f(widgetPos[1][0]-10.,widgetPos[1][1]+5.);
    glEnd();

    glPopMatrix();
}
void svSlider::RenderSingleSlider(string bottomText, string topText,
                       string widgetText,
                       string subject)
{
    glPushMatrix();
    glTranslatef(tranx, trany, 0);
    glScalef(scalex, scaley, 1);

    glColor3f(font_color[0],font_color[1],font_color[2]);
    glLineWidth(5.);
    glBegin(GL_LINES);
    glVertex2f(boundWidgetPos[0][0],boundWidgetPos[0][1]);
    glVertex2f(boundWidgetPos[1][0],boundWidgetPos[1][1]);
    glEnd();

    glColor3f(1-font_color[0],1-font_color[1],1-font_color[2]);
    glLineWidth(2.);
    glBegin(GL_LINES);
    glVertex2f(boundWidgetPos[0][0],boundWidgetPos[0][1]);
    glVertex2f(boundWidgetPos[1][0],boundWidgetPos[1][1]);
    glEnd();

    glColor3f(font_color[0],font_color[1],font_color[2]);

    glRasterPos2f(boundWidgetPos[0][0]+11., boundWidgetPos[0][1]-2);
    for(int j=0;j<strlen(bottomText);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, bottomText[j]);
    }
    glRasterPos2f(boundWidgetPos[1][0]+11., boundWidgetPos[1][1]-2);
    for(int j=0;j<strlen(topText);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, topText[j]);
    }
    glRasterPos2f(widgetPos[0][0]+11., widgetPos[0][1]-2);
    for(int j=0;j<strlen(widgetText1);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, widgetText[j]);
    }
    glRasterPos2f(boundWidgetPos[0][0]-18., boundWidgetPos[1][1]);
    for(int j=0;j<strlen(subject);j++)
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, subject[j]);
    }

    glColor3f(font_color[0],font_color[1],font_color[2]);
    glBegin(GL_LINE_LOOP);
    glVertex2f(widgetPos[0][0]-10.,widgetPos[0][1]-5.);
    glVertex2f(widgetPos[0][0]+10.,widgetPos[0][1]-5.);
    glVertex2f(widgetPos[0][0]+10.,widgetPos[0][1]+5.);
    glVertex2f(widgetPos[0][0]-10.,widgetPos[0][1]+5.);
    glEnd();
    glColor4f(widgetColor[0][0],widgetColor[0][1],
              widgetColor[0][2],widgetColor[0][3]);
    glBegin(GL_QUADS);
    glVertex2f(widgetPos[0][0]-10.,widgetPos[0][1]-5.);
    glVertex2f(widgetPos[0][0]+10.,widgetPos[0][1]-5.);
    glVertex2f(widgetPos[0][0]+10.,widgetPos[0][1]+5.);
    glVertex2f(widgetPos[0][0]-10.,widgetPos[0][1]+5.);
    glEnd();

    glPopMatrix();
}
void svSlider::SetWidgetColor()
{
  ResetColor();
  svVector4 color;
  color[0]=253./255.;
  color[1]= 174./255.;
  color[2]= 97./255.;
  color[3]=0.5;
  if(widgetSelect[0] && !widgetSelect[1]) widgetColor[0] = color;
  if(!widgetSelect[0] && widgetSelect[1]) widgetColor[1] = color;
}

/*
void svSlider::Mouse(int x, int y)
{
       svVector4 color;
       color[0]=253./255.;
       color[1]= 174./255.;
       color[2]= 97./255.;
       color[3]=0.5;

   if(x>=widgetPos[0][0]-10.&&x<=widgetPos[0][0]+10.
        && y>=widgetPos[0][1]-5.&&y<=widgetPos[0][1]+5.)
        {
             ResetColor();
             widgetColor[0] = color;
             widgetSelect[0] = true;
             widgetSelect[1] = false;
        }
   else if(x>=widgetPos[1][0]-10.&&x<=widgetPos[1][0]+10.
        && y>=widgetPos[1][1]-5.&&y<=widgetPos[1][1]+5.)
        {
             ResetColor();
             widgetColor[1] = color;
             widgetSelect[1] = true;
             widgetSelect[0] =false;
        }
   else
   {
       ResetColor();
       widgetSelect[1] = false;
       widgetSelect[0] = false;
   }

}
*/
/*
void svSlider::Movement(int x, int y)
{
       svVector4 color;
       color[0]=253./255.;
       color[1]= 174./255.;
       color[2]= 97./255.;
       color[3]=0.5;

   if(x>=widgetPos[0][0]-10.&&x<=widgetPos[0][0]+10.
        && y>=widgetPos[0][1]-5.&&y<=widgetPos[0][1]+5.)
        {
             ResetColor();
             widgetColor[0] = color;
        }
   else if(x>=widgetPos[1][0]-10.&&x<=widgetPos[1][0]+10.
        && y>=widgetPos[1][1]-5.&&y<=widgetPos[1][1]+5.)
        {
             ResetColor();
             widgetColor[1] = color;
        }
   else
   {
        ResetColor();
   }
}
*/
/*
void svSlider::Motion(int y)
{
   if(widgetSelect[0])
   {
        if(y >= boundWidgetPos[0][1]
        && y <= widgetPos[1][1]-5.)
        {
           widgetPos[0][1] = y;
        }
   }
   else if(widgetSelect[1])
   {

        if(y <= boundWidgetPos[1][1]
        && y >= widgetPos[0][1]+5.)
        {
           widgetPos[1][1] = y;
        }
   }
}
*/
svScalar svSlider::getTop()
{
   svScalar D = fabs(boundWidgetPos[1][1]-boundWidgetPos[0][1]);
   svScalar top;
   top = fabs(widgetPos[1][1] - boundWidgetPos[0][1])/D;

   return top;
}

svScalar svSlider::getDown()
{
   svScalar D = fabs(boundWidgetPos[1][1]-boundWidgetPos[0][1]);
   svScalar bottom;
   bottom = fabs(widgetPos[0][1] - boundWidgetPos[0][1])/D;

   return bottom;
}


}

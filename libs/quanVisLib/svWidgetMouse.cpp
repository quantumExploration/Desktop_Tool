#include "svWidgetMouse.h"

namespace __svl_lib{

void svWidgetSelect::Reset()
{
  widget->tselect[0] = false;
  widget->tselect[1] = false;
  widget->tselect[2] = false;
  widget->bselect = false;
}

void svWidgetSelect::Select()
{
  if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
  {
    success = false;
    Reset();
    if(mousex > widget->box[0][0]*scalex + widget->tranx
        && mousex < widget->box[2][0]*widget->scalex + widget->tranx
        && mousey > widget->box[0][1]*widget->scaley+widget->trany
        && mousey < (widget->box[0][1]+widget->boxside)*widget->scaley+widget->trany)
    {
        widget->bselect=true;
        success = true;
        isMove = true;
    }
    if(!widget->bselect)
    {
      int value = widget->triangleside * sqrt(3);
      for(int i=0;i<3;i++)
      {
         if(mousex > widget->triangle[i][0][0]*widget->scalex+widget->tranx
         && mousex < (widget->triangle[i][0][0]+widget->triangleside)*widget->scalex + widget->tranx
        &&  mousey > widget->triangle[i][0][1]*widget->scaley+widget->trany
        &&  mousey < (widget->triangle[i][0][1]+value)*widget->scaley + widget->trany)
         {
            if((i==2&&widget->showbox) || i!=2)
            {
                  success = true;
                  isMove = true;
                  widget->tselect[i]=true;
                  break;
            }
         }
      }
    }
  }
 }

void svWidgetMotion::Motion()
{
  svWidget * widget = selectEvent->widget; 
 svScalar movement;
 int x = mousex;
 int y = mousey;
 movement = -selectEvent->mousex + mousex;
 if(widget->bselect)
 {
    if((widget->box[1][0] + movement)> widget->line[0][0] + 1.*widget->boxside
    && (widget->box[0][0]+movement)>widget->line[0][0]
    && (widget->box[2][0]+movement)<widget->line[1][0])
    {
        for(int i=0;i<3;i++)
        {
          widget->box[i][0] = widget->box[i][0] + movement;
          for(int j=0;j<3;j++)
             widget->triangle[i][j][0] =widget->triangle[i][j][0]+movement;
        }
    }
 }
 else
 {
    int index=-1;
    for(int i=0;i<3;i++)
    {
       if(widget->tselect[i])
       {
             index = i;break;
       }
    }
    if( index==0)
    {
        if((widget->box[0][0]+movement)>widget->line[0][0]
        && widget->box[0][0] +movement <= widget->box[1][0]-widget->boxside)
        {
          widget->box[index][0] = widget->box[index][0] + movement;
          for(int j=0;j<3;j++)
            widget->triangle[index][j][0] =widget->triangle[index][j][0]+movement;
        }
   }
   else if(index == 1)
   {
       if(widget->showbox)
       {
        if(widget->box[1][0]+movement >= widget->box[0][0]+widget->boxside
          &&  widget->box[1][0] +movement <= widget->box[2][0]-widget->boxside)
        {
          widget->box[index][0] = widget->box[index][0] + movement;
          for(int j=0;j<3;j++)
            widget->triangle[index][j][0] =widget->triangle[index][j][0]+movement;
        }
      }
      else
      {
        if(widget->box[1][0]+movement >= widget->box[0][0]+widget->boxside
           &&  widget->box[1][0] +movement < widget->line[1][0])
        {
          widget->box[index][0] = widget->box[index][0] + movement;
          for(int j=0;j<3;j++)
            widget->triangle[index][j][0] =widget->triangle[index][j][0]+movement;
          if(!widget->showbox)
          {
             widget->box[index+1][0] = widget->box[index+1][0] + movement;
             for(int j=0;j<3;j++)
              widget->triangle[index+1][j][0] =widget->triangle[index+1][j][0]+movement;
          }
        }
      }
   }
   else if(index == 2)
   {
        if((widget->box[2][0]+movement)<widget->line[1][0]
        && widget->box[2][0]+movement>=widget->box[1][0]+widget->boxside)
        {
          widget->box[index][0] = widget->box[index][0] + movement;
          for(int j=0;j<3;j++)
            widget->triangle[index][j][0] =widget->triangle[index][j][0]+movement;
        }
   }
 }
 selectEvent->mousex = mousex;
 selectEvent->mousey = mousey;
 widget->UpdateState();
}

void svWidgetRelease::Release()
{
  selectEvent->widget->UpdateState();
}

void svWidgetMove::Move()
{
  //Reset();
  svWidget *widget = selectEvent->widget;
  if(mousex > widget->box[0][0]*scalex + widget->tranx
      && mousex < widget->box[2][0]*widget->scalex + widget->tranx
      && mousey > widget->box[0][1]*widget->scaley+widget->trany
      && mousey < (widget->box[0][1]+widget->boxside)*widget->scaley+widget->trany)
  {
      widget->bselect=true;
  }
  else
  {
    widget->bselect = false;
  }
  if(!widget->bselect)
  {
    int value = widget->triangleside * sqrt(3);
    for(int i=0;i<3;i++)
    {
       if(mousex > widget->triangle[i][0][0]*widget->scalex+widget->tranx
       && mousex < (widget->triangle[i][0][0]+widget->triangleside)*widget->scalex + widget->tranx
      &&  mousey > widget->triangle[i][0][1]*widget->scaley+widget->trany
      &&  mousey < (widget->triangle[i][0][1]+value)*widget->scaley + widget->trany)
       {
          if((i==2&&widget->showbox) || i!=2)
          {
                widget->tselect[i]=true;
          }
          else
          {
            widget->tselect[i] = false;
          }
       }
    }
  }
}


}

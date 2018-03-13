#include "svMouseInteraction.h"

namespace __svl_lib{
  void svMouseInteraction::mouse(int button, int state, int x, int y)
  {
    success = false;
    if(state == GLUT_DOWN)
    {
       int i = group->currentMouse;
       if(i<0)return;
//      for(int i=0;i<group->mouseGroups.size();i++)
      {

        if(group->mouseGroups[i]->image!=NULL)
        {
          svImageSelect * select = group->mouseGroups[i]->image->select;
          select->button = button;
          select->mouse_state = state;
          select->mousex  =x ;
          select->mousey = y;
          select->Select();
          if(select->success){success=true;}//break;}
        }
        else if(group->mouseGroups[i]->widget!=NULL)
        {
          svWidgetSelect * select = group->mouseGroups[i]->widget->select;
          select->button = button;
          select->mouse_state = state;
          select->mousex = x;
          select->mousey = y;
          select->Select();
          if(select->success){success=true;}//break;}
        }
        else if(group->mouseGroups[i]->slider!=NULL)
        {
          svROISliderSelect * select = group->mouseGroups[i]->slider->select;
          select->button = button;
          select->mouse_state = state;
          select->mousex  =x ;
          select->mousey = y;
          select->Select();
          if(select->success){success=true;}//break;}
        }
        else if(group->mouseGroups[i]->button!=NULL)
        {
          svButtonSelect * select = group->mouseGroups[i]->button->select;
          select->button = button;
          select->mouse_state = state;
          select->mousex  =x ;
          select->mousey = y;
          select->Select();
          if(select->success){success=true;}//break;}
        }
      }
    }
    else
    {
       int i = group->currentMouse;
       if(i<0)return;
      //for(int i=0;i<group->mouseGroups.size();i++)
      {
        if(group->mouseGroups[i]->image!=NULL)
        {
          svImageRelease * release = group->mouseGroups[i]->image->release;
          release->selectEvent = group->mouseGroups[i]->image->select;
          release->Release();
          //break;
        }
        else if(group->mouseGroups[i]->widget!=NULL)
        {
          group->mouseGroups[i]->widget->release->Release();
          //break;
        }
        else if(group->mouseGroups[i]->slider!=NULL)
        {
          group->mouseGroups[i]->slider->release->Release();
          //break;
        }
        else if(group->mouseGroups[i]->button!=NULL)
        {
          group->mouseGroups[i]->button->release->Release();
        }
      }
    }
  }

  void svMouseInteraction::movement(int x, int y)
  {
    int i = group->currentMouse;
       if(i<0)return;
    //for(int i=0;i<group->mouseGroups.size();i++)
    {
      if(group->mouseGroups[i]->image!=NULL)
      {
        //svImageSelect * select = group->mouseGroups[i]->image->select;
        //if(select->isMove)
        {
            svImageMove * move = group->mouseGroups[i]->image->move;
            move->mousex = x;
            move->mousey = y;
            //move->selectEvent = group->mouseGroups[i]->image->select;
            move->Move();

        }
      }
      else if(group->mouseGroups[i]->widget!=NULL)
      {
        svWidgetMove * move = group->mouseGroups[i]->widget->move;
        move->mousex = x;
        move->mousey = y;
        move->Move();
        //break;
      }
      else if(group->mouseGroups[i]->slider!=NULL)
      {
        svROISliderMove * move = group->mouseGroups[i]->slider->move;
        move->mousex = x;
        move->mousey = y;
        move->Move();
      }
      else if(group->mouseGroups[i]->button!=NULL)
      {
        svButtonMove * move = group->mouseGroups[i]->button->move;
        move->mousex = x;
        move->mousey = y;
        move->Move();
      }
    }
  }

  void svMouseInteraction::motion(int x, int y)
  {
     int i = group->currentMouse;
       if(i<0)return;

    //for(int i=0;i<group->mouseGroups.size();i++)
    {
      if(group->mouseGroups[i]->image!=NULL)
      {
        {
            svImageMotion * motion = group->mouseGroups[i]->image->motion;
            svImageSelect * select = motion->selectEvent;
            if(select->isMove && select->success){
            motion->mousex = x;
            motion->mousey = y;
            //motion->selectEvent = group->mouseGroups[i]->image->select;
            motion->Motion();
            }
            //break;
        }
      }
      else if(group->mouseGroups[i]->widget!=NULL)
      {
        svWidgetMotion * motion = group->mouseGroups[i]->widget->motion;
        svWidgetSelect * select = motion->selectEvent;
        if(select->isMove && select->success)
        {
          motion->mousex = x;
          motion->mousey = y;
          motion->Motion();
          //break;
        }
      }
      else if(group->mouseGroups[i]->slider!=NULL)
      {
        svROISliderMotion * move = group->mouseGroups[i]->slider->motion;
        svROISliderSelect * select = move->selectEvent;
        if(select->isMove && select->success)
        {
          move->mousex = x;
          move->mousey = y;
          move->Motion();
          //break;
        }
      }
    }
  }
}

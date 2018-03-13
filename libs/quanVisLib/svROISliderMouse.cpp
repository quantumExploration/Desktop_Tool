#include "svROISliderMouse.h"

namespace __svl_lib{

void svROISliderSelect::Reset()
{
  for(int i=0;i<7;i++)
  {
    slider->slider[i]->widgetSelect[0] = false;
    slider->slider[i]->widgetSelect[1] = false;
  }
  success = false;
  isMove = false;
  //if(slider->selectRec[0] || slider->selectRec[1] || slider->selectRec[2])
  //  isMove = true;
  //else
  //  isMove = false;
}

void svROISliderSelect::Select()
{
  Reset(); 
  success = false;
  isMove = false;
  svScalar recTranx = slider->recPos[0][0];
  svScalar recTrany = slider->recPos[0][1];
  //check if cursor is inside the panel
  if(mousex > slider->recPos[0][0] && mousex < slider->recPos[1][0]
  && mousey > slider->recPos[0][1] && mousey < slider->recPos[1][1])
  {
    //magnitude slider
    if(slider->selectRec[0])
    {
      for(int i=0;i<2;i++)
      {
        svScalar w = slider->slider[6]->widgetW/2.;
        svScalar h = slider->slider[6]->widgetH/2.;
        svScalar scalex = slider->slider[6]->scalex;
        svScalar scaley = slider->slider[6]->scaley;
        svScalar tranx = slider->slider[6]->tranx + recTranx;
        svScalar trany = slider->slider[6]->trany + recTrany;
        if(mousex > (slider->slider[6]->widgetPos[i][0]-w) * scalex + tranx
        && mousex < (slider->slider[6]->widgetPos[i][0]+w) * scalex + tranx
        && mousey > (slider->slider[6]->widgetPos[i][1]-h) * scaley + trany
        && mousey < (slider->slider[6]->widgetPos[i][1]+h) * scaley + trany)
        {
          slider->slider[6]->widgetSelect[i] = true;
          slider->slider[6]->SetWidgetColor();
          success = true;
          isMove = true;
          break;
        }
      }
    }
    //direction slider
    else if(slider->selectRec[1])
    {
      int sliderindex[3]={3,4,5};
      for(int i=0;i<2;i++)
      {
        for(int j=0;j<3;j++)
        {
          svScalar w = slider->slider[sliderindex[j]]->widgetW/2.;
          svScalar h = slider->slider[sliderindex[j]]->widgetH/2.;
          svScalar scalex = slider->slider[sliderindex[j]]->scalex;
          svScalar scaley = slider->slider[sliderindex[j]]->scaley;
          svScalar tranx = slider->slider[sliderindex[j]]->tranx + recTranx;
          svScalar trany = slider->slider[sliderindex[j]]->trany + recTrany;
          if(mousex > (slider->slider[sliderindex[j]]->widgetPos[i][0]-w) * scalex + tranx
          && mousex < (slider->slider[sliderindex[j]]->widgetPos[i][0]+w) * scalex + tranx
          && mousey > (slider->slider[sliderindex[j]]->widgetPos[i][1]-h) * scaley + trany
          && mousey < (slider->slider[sliderindex[j]]->widgetPos[i][1]+h) * scaley + trany)
          {
            slider->slider[sliderindex[j]]->SetWidgetColor();
            slider->slider[sliderindex[j]]->widgetSelect[i] = true;
            success = true;
            isMove = true;
            break;
          }
        }
        if(success)break;
      }
     }
      else if(slider->selectRec[2])
      {
        int sliderindex[3]={0,1,2};
        for(int i=0;i<2;i++)
        {
          for(int j=0;j<3;j++)
          {
            svScalar w = slider->slider[sliderindex[j]]->widgetW/2.;
            svScalar h = slider->slider[sliderindex[j]]->widgetH/2.;
            svScalar scalex = slider->slider[sliderindex[j]]->scalex;
            svScalar scaley = slider->slider[sliderindex[j]]->scaley;
            svScalar tranx = slider->slider[sliderindex[j]]->tranx + recTranx;
            svScalar trany = slider->slider[sliderindex[j]]->trany + recTrany;
            if(mousex > (slider->slider[sliderindex[j]]->widgetPos[i][0]-w) * scalex + tranx
            && mousex < (slider->slider[sliderindex[j]]->widgetPos[i][0]+w) * scalex + tranx
            && mousey > (slider->slider[sliderindex[j]]->widgetPos[i][1]-h) * scaley + trany
            && mousey < (slider->slider[sliderindex[j]]->widgetPos[i][1]+h) * scaley + trany)
            { cerr<<sliderindex[j]<<endl;
              slider->slider[sliderindex[j]]->SetWidgetColor();
              slider->slider[sliderindex[j]]->widgetSelect[i] = true;
              success = true;
              isMove = true;
              //break;
            }
          }
          if(success)break;
        }
      }
    }
      //magnitude text
     /* if(!success)
      {
        if(mousex > slider->magRec[0][0]+recTranx
        && mousex < slider->magRec[1][0]+recTranx
        && mousey > slider->magRec[0][1]+recTrany
        && mousey < slider->magRec[1][1]+recTrany)
        {
          slider->selectRec[0] = true;
          slider->selectRec[1] = false;
          slider->selectRec[2] = false;
          success = true;
        }
        else if(mousex > slider->dirRec[0][0]+recTranx
        && mousex < slider->dirRec[1][0]+recTranx
        && mousey > slider->dirRec[0][1]+recTrany
        && mousey < slider->dirRec[1][1]+recTrany)
        {
          slider->selectRec[0] = false;
          slider->selectRec[1] = true;
          slider->selectRec[2] = false;
          success = true;
        }
        else if(mousex > slider->posRec[0][0]+recTranx
        && mousex < slider->posRec[1][0]+recTranx
        && mousey > slider->posRec[0][1]+recTrany
        && mousey < slider->posRec[1][1]+recTrany)
        {
          slider->selectRec[0] = false;
          slider->selectRec[1] = false;
          slider->selectRec[2] = true;
          success = true;
        }
        else
        {
          slider->selectRec[0] = false;
          slider->selectRec[1] = false;
          slider->selectRec[2] = false;
        }
      }
     */
}

void svROISliderMotion::Motion()
{
  svROISlider *slider = selectEvent->slider;

  svScalar recTranx = slider->recPos[0][0];
  svScalar recTrany = slider->recPos[0][1];
  svScalar w = slider->slider[0]->widgetW/2.;
  svScalar h = slider->slider[0]->widgetH/2.;
  svScalar scalex = slider->slider[0]->scalex;
  svScalar scaley = slider->slider[0]->scaley;
//  svScalar movement;
//  movement = -//selectEvent->mousey + mousey;
  if(selectEvent->success && selectEvent->isMove)
  {
    if(slider->selectRec[0] || slider->selectRec[1] || slider->selectRec[2])
    {
      int sliderindex[3];
      int end=0;
      if(slider->selectRec[0])
      {
        sliderindex[0]=6;
        end = 1;
      }
      else if(slider->selectRec[1])
      {
          sliderindex[0]=3;
          sliderindex[1]=4;
          sliderindex[2]=5;
          end = 3;
      }
      else if(slider->selectRec[2])
      {
        sliderindex[0]=0;
        sliderindex[1]=1;
        sliderindex[2]=2;
        end = 3;
      }
      for(int i=0;i<end;i++)//direction
      {
        svScalar tranx = slider->slider[sliderindex[i]]->tranx + recTranx;
        svScalar trany = slider->slider[sliderindex[i]]->trany + recTrany;

        if(slider->slider[sliderindex[i]]->widgetSelect[0])
        {
          if(mousey < slider->slider[sliderindex[i]]->boundWidgetPos[0][1]*scaley+trany)
          {
            slider->slider[sliderindex[i]]->widgetPos[0][1]=slider->slider[sliderindex[i]]->boundWidgetPos[0][1];
          }
          else if(mousey >= slider->slider[sliderindex[i]]->boundWidgetPos[0][1]*scaley + trany
          && mousey <= (slider->slider[sliderindex[i]]->widgetPos[1][1]-h)*scaley + trany)
          {
            slider->slider[sliderindex[i]]->widgetPos[0][1]=(mousey-trany)/scaley;// +=movement/scaley;//(mousey-trany)/scaley;
          }
          break;
        }
        else if(slider->slider[sliderindex[i]]->widgetSelect[1])
        {
          if(mousey > slider->slider[sliderindex[i]]->boundWidgetPos[1][1]*scaley+trany)
          {
            slider->slider[sliderindex[i]]->widgetPos[1][1]=slider->slider[sliderindex[i]]->boundWidgetPos[1][1];
          }
          else if(mousey >= (slider->slider[sliderindex[i]]->widgetPos[0][1]+h)*scaley + trany
          && mousey <= (slider->slider[sliderindex[i]]->boundWidgetPos[1][1])*scaley+trany)
          {
            slider->slider[sliderindex[i]]->widgetPos[1][1]=(mousey-trany)/scaley;// +=movement/scaley;//(mousey-trany)/scaley;
          }
          break;
        }
      }
      if(slider->selectRec[0])
      {
        slider->setWidgetMag();
      }
      else if(slider->selectRec[1])
      {
        slider->setWidgetVel();
      }
      else if(slider->selectRec[2])
      {
        slider->setWidgetPos();
      }
    }
  }
  //selectEvent->mousey = mousey;
}


void svROISliderRelease::Release()
{
  selectEvent->Reset();
  selectEvent->slider->UpdateState();
  for(int i=0;i<7;i++)
      selectEvent->slider->slider[i]->SetWidgetColor();
}

void svROISliderMove::Move()
{
  svROISlider *slider = selectEvent->slider;

  svScalar recTranx = slider->recPos[0][0];
  svScalar recTrany = slider->recPos[0][1];
  //check if cursor is inside the panel
  if(mousex > slider->recPos[0][0] && mousex < slider->recPos[1][0]
  && mousey > slider->recPos[0][1] && mousey < slider->recPos[1][1])
  {
    //magnitude slider
    if(slider->selectRec[0])
    {
      for(int i=0;i<2;i++)
      {
        svScalar w = slider->slider[6]->widgetW/2.;
        svScalar h = slider->slider[6]->widgetH/2.;
        svScalar scalex = slider->slider[6]->scalex;
        svScalar scaley = slider->slider[6]->scaley;
        svScalar tranx = slider->slider[6]->tranx + recTranx;
        svScalar trany = slider->slider[6]->trany + recTrany;
        if(mousex > (slider->slider[6]->widgetPos[i][0]-w) * scalex + tranx
        && mousex < (slider->slider[6]->widgetPos[i][0]+w) * scalex + tranx
        && mousey > (slider->slider[6]->widgetPos[i][1]-h) * scaley + trany
        && mousey < (slider->slider[6]->widgetPos[i][1]+h) * scaley + trany)
        {
          slider->slider[6]->widgetSelect[i] = true;
        }
        else
        {
          slider->slider[6]->widgetSelect[i] = false;
        }
        slider->slider[6]->SetWidgetColor();
      }
    }
    //direction slider
    else if(slider->selectRec[1])
    {
      int sliderindex[3]={3,4,5};
      for(int i=0;i<2;i++)
      {
        for(int j=0;j<3;j++)
        {
          svScalar w = slider->slider[sliderindex[j]]->widgetW/2.;
          svScalar h = slider->slider[sliderindex[j]]->widgetH/2.;
          svScalar scalex = slider->slider[sliderindex[j]]->scalex;
          svScalar scaley = slider->slider[sliderindex[j]]->scaley;
          svScalar tranx = slider->slider[sliderindex[j]]->tranx + recTranx;
          svScalar trany = slider->slider[sliderindex[j]]->trany + recTrany;
          if(mousex > (slider->slider[sliderindex[j]]->widgetPos[i][0]-w) * scalex + tranx
          && mousex < (slider->slider[sliderindex[j]]->widgetPos[i][0]+w) * scalex + tranx
          && mousey > (slider->slider[sliderindex[j]]->widgetPos[i][1]-h) * scaley + trany
          && mousey < (slider->slider[sliderindex[j]]->widgetPos[i][1]+h) * scaley + trany)
          {
            slider->slider[sliderindex[j]]->widgetSelect[i] = true;
          }
          else
          {
            slider->slider[sliderindex[j]]->widgetSelect[i] = false;
          }
          slider->slider[sliderindex[j]]->SetWidgetColor();
        }
      }
     }
      //position slider
      else if(slider->selectRec[2])
      {
        int sliderindex[3]={0,1,2};
        for(int i=0;i<2;i++)
        {
          for(int j=0;j<3;j++)
          {
            svScalar w = slider->slider[sliderindex[j]]->widgetW/2.;
            svScalar h = slider->slider[sliderindex[j]]->widgetH/2.;
            svScalar scalex = slider->slider[sliderindex[j]]->scalex;
            svScalar scaley = slider->slider[sliderindex[j]]->scaley;
            svScalar tranx = slider->slider[sliderindex[j]]->tranx + recTranx;
            svScalar trany = slider->slider[sliderindex[j]]->trany + recTrany;
            if(mousex > (slider->slider[sliderindex[j]]->widgetPos[i][0]-w) * scalex + tranx
            && mousex < (slider->slider[sliderindex[j]]->widgetPos[i][0]+w) * scalex + tranx
            && mousey > (slider->slider[sliderindex[j]]->widgetPos[i][1]-h) * scaley + trany
            && mousey < (slider->slider[sliderindex[j]]->widgetPos[i][1]+h) * scaley + trany)
            {
              slider->slider[sliderindex[j]]->widgetSelect[i] = true;
            }
            else
            {
              slider->slider[sliderindex[j]]->widgetSelect[i] = false;
            }
          slider->slider[sliderindex[j]]->SetWidgetColor();
          }
        }
      }
      
      bool temp[3];
      for(int i=0;i<3;i++) temp[i]=slider->selectRec[i];

        if(mousex > slider->magRec[0][0]+recTranx
        && mousex < slider->magRec[1][0]+recTranx
        && mousey > slider->magRec[0][1]+recTrany
        && mousey < slider->magRec[1][1]+recTrany)
        {
          slider->selectRec[0] = true;
          slider->selectRec[1] = false;
          slider->selectRec[2] = false;
        }
        else if(mousex > slider->dirRec[0][0]+recTranx
        && mousex < slider->dirRec[1][0]+recTranx
        && mousey > slider->dirRec[0][1]+recTrany
        && mousey < slider->dirRec[1][1]+recTrany)
        {
          slider->selectRec[0] = false;
          slider->selectRec[1] = true;
          slider->selectRec[2] = false;
        }
        else if(mousex > slider->posRec[0][0]+recTranx
        && mousex < slider->posRec[1][0]+recTranx
        && mousey > slider->posRec[0][1]+recTrany
        && mousey < slider->posRec[1][1]+recTrany)
        {
          slider->selectRec[0] = false;
          slider->selectRec[1] = false;
          slider->selectRec[2] = true;
        }
        else
        {
          slider->selectRec[0] = temp[0];//false;
          slider->selectRec[1] = temp[1];//false;
          slider->selectRec[2] = temp[2];//false;
        }

    }
}


}

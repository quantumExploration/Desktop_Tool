#include "svButtonMouse.h"

namespace __svl_lib{

void svButtonSelect::Reset()
{
  for(int i=0;i<9;i++) buttons->select[i] = true;
  for(int i=0;i<9;i++) selectstate[i] = true;
}

void svButtonSelect::Select()
{
  if(button == GLUT_LEFT_BUTTON)
  {
    success = false;
    isMove = false;
//    Reset();
    for(int i=0;i<9;i++)
    {
      if(mousex > buttons->pos[i][0] && mousex < buttons->pos[i][0]+buttons->bw
      && mousey > buttons->pos[i][1] && mousey < buttons->pos[i][1]+buttons->bh)
      {
        if(selectstate[i]) buttons->select[i] = false;
        else buttons->select[i] = true;
        cout<<buttons->select[i]<<endl;
        selectstate[i] = buttons->select[i];
        success = true;
        break;
      }
    }
  }
}

void svButtonRelease::Release()
{
  selectEvent->buttons->UpdateState();
}

void svButtonMove::Move()
{
    svSymmetryButtons *buttons = selectEvent->buttons;
    for(int i=0;i<9;i++)
    {
      if(mousex > buttons->pos[i][0] && mousex < buttons->pos[i][0]+buttons->bw
      && mousey > buttons->pos[i][1] && mousey < buttons->pos[i][1]+buttons->bh)
      {
        if(selectEvent->selectstate[i]) buttons->select[i] = false;
        else buttons->select[i] = true;
      }
      else
      {
        buttons->select[i] = selectEvent->selectstate[i]; 
      }
    }
}


}

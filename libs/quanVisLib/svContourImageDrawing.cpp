#include "svContourImageDrawing.h"
#include <fstream>

using namespace std;

namespace __svl_lib {

svContourImageDrawing::svContourImageDrawing():svContourImage()
{

}

svContourImageDrawing::svContourImageDrawing(svQDOTData *d):svContourImage(d)
{

}

void svContourImageDrawing::Render()
{
   glPushMatrix();
   glTranslatef(tranx, trany, 0);
   svScalar x, y;
   x = 0;
   y = 0;
   glColor3f(0,0,0);
   glBegin(GL_LINES);
   for(int i=0;i<seed_num;i++)
   {
       for(int j=0;j<contour_count[i];j++)
       {
          if(contour_display[i][j])
          {
            for(int t=0;t<contour_pixel[i][j].size();)
            {
                glVertex2f(contour_pixel[i][j][t][0],
                          contour_pixel[i][j][t][1]);
                glVertex2f(contour_pixel[i][j][t+1][0],
                          contour_pixel[i][j][t+1][1]);
                t+=2;
            }
           }
       }
//      if((i+1)%segment == 0)
//      {
//          x = 0;
//          y = y - sidey;
//      }
//      else
//      {
//          x = x + sidex;
//      }
   }
   glEnd();

   x=0;y=0;
   glColor3f(1,0,0);
   glBegin(GL_LINES);
   for(int i=0;i<seed_num;i++)
   {
      for(int t=0;t<contour_valid_pixel[i].size();)
      {
                glVertex2f(contour_valid_pixel[i][t][0],
                          contour_valid_pixel[i][t][1]);
                glVertex2f(contour_valid_pixel[i][t+1][0],
                          contour_valid_pixel[i][t+1][1]);
                t+=2;
      }
//      if((i+1)%segment == 0)
//      {
//          x = 0;
//          y = y - sidey;
//      }
//     else
//      {
//          x = x + sidex;
//      }
   }
   glEnd();
   glPopMatrix();
}

void svContourImageDrawing::Move()
{
   int previous_layer = select_layer;
    
   if(previous_layer>=0)
   {
      select_layer = -1;
      SetPosition();
      SetContourPosition();
      select_pixel = -1;
   }
}

void svContourImageDrawing::Move(int mousex, int mousey)
{
   mousex = mousex - tranx;
   mousey = mousey - trany;
   svScalar x,y;
   int previous_layer =  select_layer;
   bool flag = false;

   select_layer = -1;

   for(int i=0;i<seed_num;i++)
   {
       if(previous_layer == i)
       {
          x = sidex * largeratio;
          y = sidey * largeratio;
       }
       else
       {
          x = sidex ;
          y = sidey ;
       }

       if(mousex >= image_pos[i][0] && mousex <= image_pos[i][0]+x
       && mousey >= image_pos[i][1] && mousey <= image_pos[i][1]+y)
       {
           flag = true;
           select_layer = i;
           break;
       }
   } 
  if(previous_layer != select_layer || select_layer<0)
  {
   SetPosition();
   SetContourPosition();
   select_pixel = -1;
  }
}

void svContourImageDrawing::Mouse(int mousex, int mousey,
                                  int &layer, int &index, svScalar &den)
{
//   mousex = mousex - tranx;
//   mousey = mousey - trany;
   svScalar x, y;
   x = 0;y=0;
   layer = -1;
   index = -1;

   layer = select_layer;

   if(select_layer >= 0)
   {

//      for(int i=0;i<seed_num;i++)
//     {
//        if(mousex >= x && mousex <= x+sidex
//        && mousey >= y && mousey <= y+sidey)
//        {
//           select_layer = i;
//           break;
//        }
//        if((i+1)%segment == 0)
//        {
//          x = 0;
//          y = y - sidey;
//        }
//        else
//        {
//          x = x + sidex;
//        }
//     }
//   }
//   else
//   {

     if(mousex-tranx >= image_pos[select_layer][0] 
      && mousex-tranx <=image_pos[select_layer][0]+sidex*largeratio
      && mousey-trany >= image_pos[select_layer][1] 
      && mousey-trany <=image_pos[select_layer][1]+sidey*largeratio)
    {
      int i = select_layer;
      double td, dd;
      td = 9e+9;
      for(int j=0;j<myData->GetSeed(i);j++)
      {
              dd = (mousex-pixel[i][j][0]-x)*(mousex-pixel[i][j][0]-x)
                 + (mousey-pixel[i][j][1]-y)*(mousey-pixel[i][j][1]-y);
              if(td>dd)
              {
                    td = dd;
                    index = j;
                    select_pixel = j;
                    decide_pixel = j;
                    decide_layer = select_layer;
                    den = myData->GetMag(select_layer,j);//mag[select_layer][j];
              }
      }
    }
   }
//           layer = i;
//           double td, dd;
//           td = 9e+9;
//         for(int j=0;j<data[i].size();j++)
//         {
        //      cerr<<mousex<<" "<<mousey<<" "<<pixel[i][j][0]+x<<" "<<pixel[i][j][1]+y<<endl;
//              dd = (mousex-pixel[i][j][0]-x)*(mousex-pixel[i][j][0]-x)
//                 + (mousey-pixel[i][j][1]-y)*(mousey-pixel[i][j][1]-y);
//              if(td>dd)
//              {
//                    td = dd;
//                    index = j;
//              }
//         }
//         break;
 //     }

//      if((i+1)%segment == 0)
//      {
//         x = 0;
//          y = y - sidey;
//      }
//      else
//      {
//          x = x + sidex;
//      }
//   }
//   }
}

}

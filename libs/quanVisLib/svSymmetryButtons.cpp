#include "svSymmetryButtons.h"

namespace __svl_lib{

svSymmetryButtons::svSymmetryButtons(svRawSymmetryData *data)
{
  myData = data;
  buttoncolors = new svVector4[8];
  for(int i=0;i<9;i++)select[i]=true;
}


void svSymmetryButtons::Init()
{
  for(int i=0;i<9;i++)
     splitSize[i] = 0;
  dataSize = 0;
  for(int i=0;i<myData->splitData.size();i++)
  {
     for(int j=0;j<myData->splitData[i].size();j++)
     {
         splitSize[i]++;
         dataSize++;
     }
  }
  //cout<<dataSize<<endl;
}

void svSymmetryButtons::Reshape(int left, int top)
{
   bh = 40.;
   bw = bh  * 1.8;

   for(int i=0;i<9;i++)
   {
      pos[i][0] = (float)left + bw * (float)i + bw/4. * (float)i;
      pos[i][1] = (float)top - bh * 2.;
   }
}

void svSymmetryButtons::Render()
{
   glEnable(GL_MULTISAMPLE);
   char str[5];
   svVector4 color;
   color[0]=0.5;color[1]=0.5;color[2]=0.5;color[3]=0.5;
   for(int i=0;i<8;i++)
   {
        if(select[i])
          DrawArrowDownButton(i,
                       bw, bh,//*8.5,
                       pos[i],
                       buttoncolors[i]);
         else
          DrawArrowUpButton(i,bw, bh,
                       pos[i],
                       buttoncolors[i]);
   }

	if(select[8])
          DrawTextDownButton("Others",bw, bh,
                       pos[8] );
         else
          DrawTextUpButton("Others",bw,bh,
                       pos[8] );


   svVector3 end = pos[SYMMETRY_TYPE];
   end[0] = end[0] + bw;//texture_size*texture_ratio;
   DrawHorizonBars(bw, bh, pos,dataSize, splitSize, 9);

}

void svSymmetryButtons::UpdateState()
{
   for(int i=0;i<9;i++)
      myState->rP->symmetryVisible[i] = select[i];
   myState->updateVisible = true;
}

}

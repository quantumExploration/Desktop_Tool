/*  svGlyph
 */
#include "svGlyph.h"
#include <algorithm>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include "svException.h"
#include "svUtil.h"
#include "svCCL.h"


using namespace std;

namespace __svl_lib {
/*
svGlyph::svGlyph(svQDOTData * myData){
  this->myData = myData;

  seed_num = myData->GetSeed();
  //glyphColors = new svVector4Array[seed_num];
  glyphWidth = new svScalarArray[seed_num];

  svVector4 color;
  color[0]=1;color[1]=1;color[2]=1;color[3]=1;
  for(int i=0;i<seed_num;i++){
     int s = myData->GetSeed(i);
     for(int j =0;j<s;j++){
    //    glyphColors[i].add(color);
        glyphWidth[i].add(1);
     } 
  }
}
*/
svGlyph::svGlyph(vector< vector<Spin *> > myData){
  this->myData = myData;
  seed_num = myData.size();
    glyphWidth = new svScalarArray[seed_num];
  svVector4 color;
  color[0]=1;color[1]=1;color[2]=1;color[3]=1;
  for(int i=0;i<seed_num;i++){
     int s = myData[i].size();
     for(int j =0;j<s;j++){
        glyphWidth[i].add(1);
     } 
  }
}
void svGlyph::SetColors(svColors *color){
  myColor = color;
  glyphColors = myColor->GetColors();
}

void svGlyph::SetBBox()
{
  lbbox=svVector3(9e+5,9e+5,9e+5);
  rbbox=svVector3(-9e+5,-9e+5,-9e+5);

  for(int i=0;i<seed_num;i++)
  {
      for(int j=0;j<myData[i].size();j++)
      {//cerr<<j<<" "<<i<<endl;
           svVector3 glyph = (*myData[i][j]).pos;
           if(glyph[0] > rbbox[0]) rbbox[0] = glyph[0];
           if(glyph[1] > rbbox[1]) rbbox[1] = glyph[1];
           if(glyph[2] > rbbox[2]) rbbox[2] = glyph[2];
           if(glyph[0] < lbbox[0]) lbbox[0] = glyph[0];
           if(glyph[1] < lbbox[1]) lbbox[1] = glyph[1];
           if(glyph[2] < lbbox[2]) lbbox[2] = glyph[2];
      }
  }
}

svGlyph::~svGlyph()
{
  clean();

}


void svGlyph::clean()
{
  glyphSize = 0;

  if (glyphWidth != NULL) {
    for (int i=0; i<seed_num; i++)  
	glyphWidth [i].free();
    delete [] glyphWidth;
    glyphWidth=NULL;
  }
/*
  if (glyphColors != NULL) {
    for (int i=0; i<seed_num; i++)  
	glyphColors[i].free();
    delete [] glyphColors;
    glyphColors=NULL;
  };*/
}
/*
void svGlyph::DisableColor()
{
  for(int i=0;i<seed_num;i++)
  {
	 // glyphColors[i].free();
	  for(int j=0;j<myData->GetSeed(i);j++)
	  {
		  svVector4 color;
		  color[0] = 1;
		  color[1] = 1;
		  color[2] = 1;
		  color[3] = 1;
		  glyphColors[i][j] = color;
	  }
  }
}

void svGlyph::SetColor(svVector4 color)
{
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<myData->GetSeed(i);j++)
         {
             glyphColors[i][j] = color;
         }
     }

}
void svGlyph::SetSolidColor()
{
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<myData->GetSeed(i);j++)
         {
             glyphColors[i][j][0] = 1;
             glyphColors[i][j][1] = 1;
             glyphColors[i][j][2] = 1;
         }
     }
}
void svGlyph::SetColor(int index1, int index2, svVector4 color1, svVector4 color2)
{
     SetColor(color1);
     for(int i=0;i<seed_num;i++)
     {
             glyphColors[i][index1] = color2;
             glyphColors[i][index2] = color2;
     }
}
void svGlyph::UpdateColor(){
  for(int i=0;i<seed_num;i++){
     glyphColors[i].free();
          for(int j=0;j<myData->GetSeed(i);j++)
         {
  svVector4 color;
  color[0]=1;color[1]=1;color[2]=1;color[3]=1;
        glyphColors[i].add(color);
        glyphWidth[i].add(1);

         }
  }
}
void svGlyph::SetColorByCluster(svIntArray index, svVector4 c)
{
     SetColor(c);
     svColors *color = new svColors();
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<myData->GetSeed(i);j++)
         {
              int c = myData->GetClusterLabel(i,j);//clusterLabel[i][j];
              bool flag = false;
              for(int t=0;t<index.size();t++)
              {
                  if(c == index[t])
                  {
                     flag = true;
                     break;
                  }
              }             
            if(flag)
               glyphColors[i][j] = color->GetDiscreteColors(c);
         }
     }
     delete color;
}
void svGlyph::SetColorBySymmetryType()
{
     svColors *color = new svColors();
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<myData->GetSeed(i);j++)
         {
             int c;// = myData->GetSType(i,j);//stype[i][j];
             glyphColors[i][j] = color->Get8Colors(c);
         }
     }
     delete color;
}
void svGlyph::SetColorByPower()
{
     svColors *color = new svColors();
     svScalarArray value;

     for(int i=numPower-1;i>=0;i--)
     {
          value.add((svScalar)i);
     }
//cerr<<value.size()<<endl;
     svVector4Array cc = color->GetDivergingColors(value);//GetContinuousColors(value);
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<myData->GetSeed(i);j++)
         {
            // if(exp[i][j]>0)
            // {
                  svInt e = numPower-(myData->GetExp(i,j)+scaling)-1; 
                if(e>=0)
                  glyphColors[i][j] = cc[e]; 
                else
                 {
                       glyphColors[i][j][0]=1;
                       glyphColors[i][j][1]=1;
                       glyphColors[i][j][2]=1;
                 }
                 glyphColors[i][j][3] = 0.5;
           // }
         }
     }
     value.free();
     delete color;
}
void svGlyph::SetColorByCluster()
{
     svColors *color = new svColors();
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<myData->GetSeed(i);j++)
         {
              int c = myData->GetClusterLabel(i,j);//clusterLabel[i][j];
             glyphColors[i][j] = color->GetDiscreteColors(c);
         }
     }
     delete color;
}
void svGlyph::SetColorByCluster(svIntArray cluster)
{
     svColors *color = new svColors();
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<myData->GetSeed(i);j++)
         {
              bool flag = false;
              int c = myData->GetClusterLabel(i,j);//clusterLabel[i][j];
              for(int t=0;t<cluster.size();t++)
              {
                   if(c == cluster[t])
                   {
                           flag = true;break;
                   }
              }
             if(flag)
             glyphColors[i][j] = color->GetDiscreteColors(c);
         }
     }
     delete color;
}

void svGlyph::SetColorByClusterMag()
{
     svScalarArray clustermag;
     int count[myData->GetMaxClusterLabel()+1];
     for(int i=0;i<myData->GetMaxClusterLabel()+1;i++)
     {
         count[i] = 0;
         clustermag.add(0);
     }
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<myData->GetSeed(i);j++)
         {
             int c=myData->GetClusterLabel(i,j);//clusterLabel[i][j];
             if(c>=0)
             {
                clustermag[c] = clustermag[c]+myData->GetMag(i,j);
                count[c]++;
             }
         }
     }

    for(int i=0;i<myData->GetMaxClusterLabel()+1;i++)
    {
        if(count[i]>0)
           clustermag[i] = clustermag[i]/count[i];
    }

    vector<svScalar> cmag;
    for(int i=0;i<myData->GetMaxClusterLabel()+1;i++)
    {
          cmag.push_back(clustermag[i]);
    }

    svIntArray order;
    sort(cmag.begin(), cmag.end());
    for(int i=0;i<myData->GetMaxClusterLabel()+1;i++)
    {
         double td = 9e+9;double dd;
         int in=-1;
         for(int j=0;j<myData->GetMaxClusterLabel()+1;j++)
         {
               dd = fabs(cmag[j] - clustermag[i]);
               if(dd < td){
                   bool flag = false;
                   for(int t=0;t<order.size();t++)
                   {
                        if(order[t] == myData->GetMaxClusterLabel()-j){flag = true; break;} 
                   } 
                   if(!flag){td = dd; in = j;}
               }
         }
         order.add(myData->GetMaxClusterLabel()-in);
//         cerr<<clustermag[i]<<" "<<maxClusterLabel-in<<endl;
    }

     svColors *color = new svColors();
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<myData->GetSeed(i);j++)
         {
              int c = myData->GetClusterLabel(i,j);//clusterLabel[i][j];
             if(c==-1)
                 glyphColors[i][j] = color->GetDiscreteColors(c);
             else
             {
                  //clusterLabel[i][j] = order[c]; ????
                  glyphColors[i][j] = color->GetDiscreteColors(order[c]);
             }
         }
     }
     
     order.free();
     delete color; 
}
*/
void svGlyph::DrawGrid(svVector3 startPos, svVector3 dir1, svVector3 dir2,
                       svScalar stepD1, svScalar stepD2,
                       svInt stepN1, svInt stepN2)
{
     glDisable(GL_LIGHTING);
     glDisable(GL_LIGHT0);
       
     svVector3 pos = startPos;
     for(int i=0;i<=stepN1;i++)
     {
         glBegin(GL_LINES);
         glVertex3f(pos[0],pos[1],pos[2]);
         svVector3 end = pos + stepN2*stepD2*dir2;
         glVertex3f(end[0], end[1], end[2]);
         glEnd();
         pos =  pos + stepD1 * dir1;
     }

     pos = startPos;
     for(int i=0;i<=stepN2;i++)
     {
         glBegin(GL_LINES);
         glVertex3f(pos[0],pos[1],pos[2]);
         svVector3 end = pos + stepN1*stepD1*dir1;
         glVertex3f(end[0], end[1], end[2]);
         glEnd();
         pos =  pos + stepD2 * dir2;
     }

}
/*
void svGlyph::DrawSilkPlane(svVector3 planeDir)
{
  glLineWidth(1.0);

   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(1.0, 1.0); 
    for(int i=0;i<seed_num;i++)
    {
        if(myData->GetSeed(i)>0)
        {
             glColor4f(render_property.color[0],
                    render_property.color[1],
                    render_property.color[2],
                  render_property.color[3]);
             glBegin(GL_QUADS);
             if(planeDir[2] == 1.)
             {
                 glVertex3f(lbbox[0],lbbox[1],glyph[i][0][2]);
                 glVertex3f(rbbox[0],lbbox[1],glyph[i][0][2]);
                 glVertex3f(rbbox[0],rbbox[1],glyph[i][0][2]);
                 glVertex3f(lbbox[0],rbbox[1],glyph[i][0][2]);
             }     
             glEnd();
             glColor4f(1-render_property.color[0],
                    1-render_property.color[1],
                    1-render_property.color[2],
                  render_property.color[3]);
             glBegin(GL_LINE_LOOP);
             if(planeDir[2] == 1.)
             {
                 glVertex3f(lbbox[0],lbbox[1],glyph[i][0][2]);
                 glVertex3f(rbbox[0],lbbox[1],glyph[i][0][2]);
                 glVertex3f(rbbox[0],rbbox[1],glyph[i][0][2]);
                 glVertex3f(lbbox[0],rbbox[1],glyph[i][0][2]);
             }
             glEnd();
        }
    }
   glDisable(GL_POLYGON_OFFSET_FILL);

}
*/

}

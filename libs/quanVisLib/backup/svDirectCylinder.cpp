#include "svDirectCylinder.h"
#include <algorithm>

using namespace std;

namespace __svl_lib {

svDirectCylinder::svDirectCylinder(svVectorField *f) : svCylinderGlyph(f)
{

}
void svDirectCylinder::RenderSample()
{
    for(int i=0;i<sampleLabel.size();)
    {
           int layer = sampleLabel[i];
           int index = sampleLabel[i+1];

           svScalar length = mag[layer][index] * glyphScale;
           svVector3 end = glyph[layer][index]
                       + dir[layer][index] * length;
           glColor4f(glyphColors[layer][index][0],glyphColors[layer][index][1],
                     glyphColors[layer][index][2],glyphColors[layer][index][3]);
           RenderCylinder(glyph[layer][index], dir[layer][index],
                       glyphTubeRadius,
                       length, CYLINDERSLICE);
           i+=2;
    }
}

void svDirectCylinder::GenerateTubes()
{
    int count1 = 0;
    int count2 = 0;
    for(int i =0;i<seed_num;i++)
    {
                for(int j=0;j<glyph[i].size();j++)
                {
                        double radius = glyphTubeRadius;
                        svVector3 end;
                        svScalar scale = mag[i][j]*glyphScale;
                        svVector3 tube_seg_norm[(CYLINDERSLICE+1)*2];
                        svVector3  tube_seg_pos[(CYLINDERSLICE+1)*2];

                        GetCylinder(glyph[i][j], dir[i][j], radius,
                                 scale,  CYLINDERSLICE,
                                  tube_seg_norm, tube_seg_pos);

                        int start= count2;
                       tube_base_vertices[count2].pos[0]=glyph[i][j][0];
                       tube_base_vertices[count2].pos[1]=glyph[i][j][1];
                       tube_base_vertices[count2].pos[2]=glyph[i][j][2];
                       tube_base_vertices[count2].norm[0]=-dir[i][j][0];
                       tube_base_vertices[count2].norm[1]=-dir[i][j][1];
                       tube_base_vertices[count2].norm[2]=-dir[i][j][2];
                       tube_base_vertices[count2].color[0]=glyphColors[i][j][0];
                       tube_base_vertices[count2].color[1]=glyphColors[i][j][1];
                       tube_base_vertices[count2].color[2]=glyphColors[i][j][2];
                       tube_base_vertices[count2].color[3]=glyphColors[i][j][3];
                        count2++;
                        for(int t=0;t<(CYLINDERSLICE+1)*2;t++)
                        {
                            tube_vertices[count1].pos[0]=tube_seg_pos[t][0];
                            tube_vertices[count1].pos[1]=tube_seg_pos[t][1];
                            tube_vertices[count1].pos[2]=tube_seg_pos[t][2];
                            tube_vertices[count1].norm[0]=tube_seg_norm[t][0];
                            tube_vertices[count1].norm[1]=tube_seg_norm[t][1];
                            tube_vertices[count1].norm[2]=tube_seg_norm[t][2];
                            tube_vertices[count1].color[0]=glyphColors[i][j][0];
                            tube_vertices[count1].color[1]=glyphColors[i][j][1];
                            tube_vertices[count1].color[2]=glyphColors[i][j][2];
                            tube_vertices[count1].color[3]=glyphColors[i][j][3];
                            if(t%2==0)
                        {
                        tube_base_vertices[count2].pos[0]=tube_seg_pos[t][0];
                        tube_base_vertices[count2].pos[1]=tube_seg_pos[t][1];
                        tube_base_vertices[count2].pos[2]=tube_seg_pos[t][2];
                        tube_base_vertices[count2].norm[0]=-dir[i][j][0];
                        tube_base_vertices[count2].norm[1]=-dir[i][j][1];
                        tube_base_vertices[count2].norm[2]=-dir[i][j][2];
                        tube_base_vertices[count2].color[0]=glyphColors[i][j][0];
                        tube_base_vertices[count2].color[1]=glyphColors[i][j][1];
                        tube_base_vertices[count2].color[2]=glyphColors[i][j][2];
                        tube_base_vertices[count2].color[3]=glyphColors[i][j][3];
                        count2++;
                         }
                            count1++;

                        }
                }
    }
}

void svDirectCylinder::GenerateIndex()
{  

    int index1 = 0;
    int count1 = 0;
    int index2 = 0;
    int count2 = 0;

    int abindex = 0;
    int abcount = 0;
    int tbindex = 0;
    int tbcount = 0;

    for(int i =0;i<seed_num;i++)
    {
                for(int j=0;j<glyph[i].size();j++)
                {
                        int start1 = tbcount;
                        tbcount++;
                        for(int t=0;t<(CYLINDERSLICE+1)*2;t++)
                        {
                            if(t%2==0&&t<CYLINDERSLICE*2)
                            {
                                 if(visibleLabel[i][j])
                                 {
                                  tube_indices[index1]  =count1;
                                  tube_indices[index1+1]=count1+1;
                                  tube_indices[index1+2]=count1+3;
                                  tube_indices[index1+3]=count1+2;
                                  index1+=4;

                                  tube_base_indices[tbindex]  =start1;
                                  tube_base_indices[tbindex+1]=tbcount+1;
                                  tube_base_indices[tbindex+2]=tbcount;
                                  tbindex+=3;
                                 }
                             }
                             count1++;
                             if(t%2==0) tbcount++;
                        }
                }
   }

}

void svDirectCylinder::Generate(DirectProperty &property, ViewProperty &viewproperty)
                       
{
  cleanup();

  cylinderEnd1 = new svVector3Array[seed_num];
  cylinderEnd2 = new svVector3Array[seed_num];
  cylinderEnd3 = new svVector3Array[seed_num];
  cylinderEnd4 = new svVector3Array[seed_num];

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  display_mode = SV_DISPLAYLIST;

  svGlyph::STILL_UPDATE = false;

        if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

   for(int i=0;i<seed_num;i++)
   {
      for(int j=0;j<glyph[i].size();j++)
      {
         if(roiLabel[i][j])
         {
            if(visibleLabel[i][j])
            {
             svVector3 v = svGetPerpendicularVector(dir[i][j]);
             svVector3 v_eye = GetVerticalByeye(glyph[i][j],dir[i][j],viewproperty);
               svVector3 p1, p2, p3, p4;
               glDisable(GL_LIGHTING);
               glDisable(GL_LIGHT0);

//legend-----------------               
               glColor4f(cproperty.legendcolor[0], cproperty.legendcolor[1], cproperty.legendcolor[2],cproperty.legendcolor[3]);

               svScalar lh = cproperty.legend*property.UnitHeight*cproperty.scaley;
               glBegin(GL_LINES);
               p1 = glyph[i][j];
               p2 = glyph[i][j] + lh * dir[i][j];
               glVertex3f(p1[0], p1[1], p1[2]);
               glVertex3f(p2[0], p2[1], p2[2]);
               glEnd();

               svVector3 head = glyph[i][j] + (svScalar)(cproperty.radius*1.1) * v;
               p2 = glyph[i][j];
               svScalar m = mag[i][j] * pow(10., property.shift);
               svScalar degree = 0;

               for(int tt=0;tt<(int)m;tt++)
               {
                       glBegin(GL_LINE_LOOP);
                       for(int t=0;t<cproperty.SEGMENT_NUM;t++)
                       {
                             p1 = svGetRotatePoint(head, p2, dir[i][j], degree);
                             degree = t * 360./cproperty.SEGMENT_NUM;
                             glVertex3f(p1[0], p1[1], p1[2]);
                       }
                       head = head + dir[i][j];
                       p2 = p2 + dir[i][j];
                       degree = 0;
                       glEnd();  
               }
//cylinder-----------------
               glColor4f(glyphColors[i][j][0],
                         glyphColors[i][j][1],
                         glyphColors[i][j][2],
                         alpha);

               glEnable(GL_LIGHTING);
               glEnable(GL_LIGHT0);

               head = glyph[i][j] + cproperty.radius * v;
               svVector3 head2 = glyph[i][j] + cproperty.radius * v + mag[i][j]*cproperty.scaley * property.UnitHeight * dir[i][j];
               p2 = glyph[i][j];
               p4 = glyph[i][j] + mag[i][j]*cproperty.scaley * property.UnitHeight * dir[i][j];
               degree = 0;

               glBegin(GL_QUAD_STRIP);
               for(int t=0;t<cproperty.SEGMENT_NUM;t++)
               {
                     p1 = svGetRotatePoint(head, p2, dir[i][j], degree);
                     p3 = svGetRotatePoint(head2, p4, dir[i][j], degree);
                     degree = t * 360./cproperty.SEGMENT_NUM;
                     svVector3 v1 = normalize(p1 - p2);
                     glNormal3f(v1[0], v1[2], v1[2]);
                     glVertex3f(p1[0], p1[1], p1[2]);
                     svVector3 v2 = normalize(p3 - p4);
                     glNormal3f(v2[0], v2[2], v2[2]);
                     glVertex3f(p3[0], p3[1], p3[2]);
               }
               glEnd(); 

               glBegin(GL_POLYGON);
               glNormal3f(-dir[i][j][0],-dir[i][j][1], -dir[i][j][2]);
               for(int t=0;t<cproperty.SEGMENT_NUM;t++)
               {
                     p1 = svGetRotatePoint(head, p2, dir[i][j], degree);
                     degree = t * 360./cproperty.SEGMENT_NUM;
                     glVertex3f(p1[0], p1[1], p1[2]);
               }
               glEnd();

               glBegin(GL_POLYGON);
               glNormal3f(dir[i][j][0],dir[i][j][1], dir[i][j][2]);
               for(int t=0;t<cproperty.SEGMENT_NUM;t++)
               {
                     p1 = svGetRotatePoint(head2, p4, dir[i][j], degree);
                     degree = t * 360./cproperty.SEGMENT_NUM;
                     glVertex3f(p1[0], p1[1], p1[2]);
               }
               glEnd();
//=====================================================
               p1 = glyph[i][j] - cproperty.radius * v_eye;
               p2 = glyph[i][j] + cproperty.radius * v_eye;
               p3 = glyph[i][j] + mag[i][j]*cproperty.scaley * property.UnitHeight * dir[i][j] + cproperty.radius * v_eye;
               p4 = glyph[i][j] + mag[i][j]*cproperty.scaley * property.UnitHeight * dir[i][j] - cproperty.radius * v_eye;  

                cylinderEnd1[i].add(p1);
                cylinderEnd2[i].add(p2);
                cylinderEnd3[i].add(p3);
                cylinderEnd4[i].add(p4);

              }
            }
        }
       }

        glEndList();

    }
void svDirectCylinder::Generate(DirectProperty &property, ViewProperty &viewproperty, int index)
                       
{
  cleanup();

  cylinderEnd1 = new svVector3Array[seed_num];
  cylinderEnd2 = new svVector3Array[seed_num];
  cylinderEnd3 = new svVector3Array[seed_num];
  cylinderEnd4 = new svVector3Array[seed_num];

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  display_mode = SV_DISPLAYLIST;

  svGlyph::STILL_UPDATE = false;

        if(glIsList(display_list))
                glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);

   for(int i=index;i<index+1;i++)
   {
      for(int j=0;j<glyph[i].size();j++)
      {
         if(roiLabel[i][j])
         {
            if(visibleLabel[i][j])
            {
             svVector3 v = svGetPerpendicularVector(dir[i][j]);
             svVector3 v_eye = GetVerticalByeye(glyph[i][j],dir[i][j],viewproperty);
               svVector3 p1, p2, p3, p4;
               glDisable(GL_LIGHTING);
               glDisable(GL_LIGHT0);

//legend-----------------               
               glColor4f(cproperty.legendcolor[0], cproperty.legendcolor[1], cproperty.legendcolor[2],cproperty.legendcolor[3]);

               svScalar lh = cproperty.legend*property.UnitHeight*cproperty.scaley;
               glBegin(GL_LINES);
               p1 = glyph[i][j];
               p2 = glyph[i][j] + lh * dir[i][j];
               glVertex3f(p1[0], p1[1], p1[2]);
               glVertex3f(p2[0], p2[1], p2[2]);
               glEnd();

               svVector3 head = glyph[i][j] + (svScalar)(cproperty.radius*1.1) * v;
               p2 = glyph[i][j];
               svScalar m = mag[i][j] * pow(10., property.shift);
               svScalar degree = 0;

               for(int tt=0;tt<(int)m;tt++)
               {
                       glBegin(GL_LINE_LOOP);
                       for(int t=0;t<cproperty.SEGMENT_NUM;t++)
                       {
                             p1 = svGetRotatePoint(head, p2, dir[i][j], degree);
                             degree = t * 360./cproperty.SEGMENT_NUM;
                             glVertex3f(p1[0], p1[1], p1[2]);
                       }
                       head = head + dir[i][j];
                       p2 = p2 + dir[i][j];
                       degree = 0;
                       glEnd();  
               }
//cylinder-----------------
               glColor4f(glyphColors[i][j][0],
                         glyphColors[i][j][1],
                         glyphColors[i][j][2],
                         alpha);

               glEnable(GL_LIGHTING);
               glEnable(GL_LIGHT0);

               head = glyph[i][j] + cproperty.radius * v;
               svVector3 head2 = glyph[i][j] + cproperty.radius * v + mag[i][j]*cproperty.scaley * property.UnitHeight * dir[i][j];
               p2 = glyph[i][j];
               p4 = glyph[i][j] + mag[i][j]*cproperty.scaley * property.UnitHeight * dir[i][j];
               degree = 0;

               glBegin(GL_QUAD_STRIP);
               for(int t=0;t<cproperty.SEGMENT_NUM;t++)
               {
                     p1 = svGetRotatePoint(head, p2, dir[i][j], degree);
                     p3 = svGetRotatePoint(head2, p4, dir[i][j], degree);
                     degree = t * 360./cproperty.SEGMENT_NUM;
                     svVector3 v1 = normalize(p1 - p2);
                     glNormal3f(v1[0], v1[2], v1[2]);
                     glVertex3f(p1[0], p1[1], p1[2]);
                     svVector3 v2 = normalize(p3 - p4);
                     glNormal3f(v2[0], v2[2], v2[2]);
                     glVertex3f(p3[0], p3[1], p3[2]);
               }
               glEnd(); 

               glBegin(GL_POLYGON);
               glNormal3f(-dir[i][j][0],-dir[i][j][1], -dir[i][j][2]);
               for(int t=0;t<cproperty.SEGMENT_NUM;t++)
               {
                     p1 = svGetRotatePoint(head, p2, dir[i][j], degree);
                     degree = t * 360./cproperty.SEGMENT_NUM;
                     glVertex3f(p1[0], p1[1], p1[2]);
               }
               glEnd();

               glBegin(GL_POLYGON);
               glNormal3f(dir[i][j][0],dir[i][j][1], dir[i][j][2]);
               for(int t=0;t<cproperty.SEGMENT_NUM;t++)
               {
                     p1 = svGetRotatePoint(head2, p4, dir[i][j], degree);
                     degree = t * 360./cproperty.SEGMENT_NUM;
                     glVertex3f(p1[0], p1[1], p1[2]);
               }
               glEnd();
//=====================================================
               p1 = glyph[i][j] - cproperty.radius * v_eye;
               p2 = glyph[i][j] + cproperty.radius * v_eye;
               p3 = glyph[i][j] + mag[i][j]*cproperty.scaley * property.UnitHeight * dir[i][j] + cproperty.radius * v_eye;
               p4 = glyph[i][j] + mag[i][j]*cproperty.scaley * property.UnitHeight * dir[i][j] - cproperty.radius * v_eye;  

                cylinderEnd1[i].add(p1);
                cylinderEnd2[i].add(p2);
                cylinderEnd3[i].add(p3);
                cylinderEnd4[i].add(p4);

              }
            }
        }
       }

        glEndList();

    }

void svDirectCylinder::Render()
{   
  glCallList(display_list);
}

}

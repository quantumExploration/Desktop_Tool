#include "svLTCylinder.h"
#include <algorithm>
#define REPEAT_CYLINDER_TEXTURE 5
#define SUB_SEGMENT 4
using namespace std;

namespace __svl_lib {

svLTCylinder::svLTCylinder(svVectorField *f) : svCylinderGlyph(f)
{
}

void svLTCylinder::RenderSample()
{
    for(int i=0;i<sampleLabel.size();)
    {
           int layer = sampleLabel[i];
           int index = sampleLabel[i+1];

           svScalar radius = glyphScale/2.;
           svScalar length = coe[layer][index] * glyphScale;
           RenderCylinderTexture(glyph[layer][index], 
                       dir[layer][index],
                       radius,
                       length,
                       glyphColors[layer][index],
                       (exp[layer][index]+expScale)/(svScalar)(numPower-1),
                       2, numPower-1);
           i+=2;
    }
}

void svLTCylinder::UpdateLTData()
{
    CleanData();

    dataSize = 0;
    for(int i=0;i<seed_num;i++)
    {
        for(int j=0;j<glyph[i].size();j++)
        {
               dataSize++;
        }
    }
    cylinder_segment = (REPEAT_CYLINDER_TEXTURE)*(SUB_SEGMENT);

    tube_vertices_size = dataSize*(cylinder_segment) * 2 * 2;
    tube_base_vertices_size = dataSize*((cylinder_segment)*2+1);

    cerr<<"lt data "<<tube_vertices_size<<" "<<tube_base_vertices_size<<endl;
    tube_vertices = new VBOVertex[tube_vertices_size];//svVector3Array[seed_num];
    tube_base_vertices = new VBOVertex[tube_base_vertices_size];//svVector3Array[seed_num];

    legend_vertices = new VBOVertex[dataSize*2];
}
void svLTCylinder::UpdateLTIndex()
{
     CleanIndex();

     int count = 0;
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<glyph[i].size();j++)
          {
                   if(visibleLabel[i][j])
                   {
                       count++;
                   }
          }
     }
     tube_indices_size = count*4*(cylinder_segment);//CYLINDERSLICE;
     tube_base_indices_size = count*3*(2*cylinder_segment-1);//CYLINDERSLICE;
     tube_indices = new int[(tube_indices_size)];
     tube_base_indices = new int[tube_base_indices_size];
     cerr<<"lt index "<<tube_indices_size<<" "<<tube_base_indices_size<<endl;
}

void svLTCylinder::GenerateTubes()
{
    int count1 = 0;
    int count2 = 0;

    for(int i =0;i<seed_num;i++)
    {
            for(int j=0;j<glyph[i].size();j++)
            {
                       double radius = glyphScale;
                       svVector3 end;
                       svScalar scale = (coe[i][j])*glyphScale;

                       svVector3 tube_seg_norm[(cylinder_segment)*2*2];
                       svVector3 tube_seg_pos[(cylinder_segment)*2*2];

                       svScalar ratio;
                       if(numPower>1)
                       {
                          ratio = (exp[i][j]+expScale)/(svScalar)(numPower-1);
                       }
                       else
                       {
                           ratio = 1;//exp[i][j]+expScale;
                       }
                       GetCylinderTexture(glyph[i][j], dir[i][j], radius,
                                 scale,   ratio,
                                  SUB_SEGMENT, REPEAT_CYLINDER_TEXTURE, 
                                  tube_seg_norm, tube_seg_pos);

                       int start= count2;
                       tube_base_vertices[count2].pos[0]=glyph[i][j][0];
                       tube_base_vertices[count2].pos[1]=glyph[i][j][1];
                       tube_base_vertices[count2].pos[2]=glyph[i][j][2];
                       tube_base_vertices[count2].norm[0]=-dir[i][j][0];
                       tube_base_vertices[count2].norm[1]=-dir[i][j][1];
                       tube_base_vertices[count2].norm[2]=-dir[i][j][2];
                       tube_base_vertices[count2].color[0]=1;//glyphColors[i][j][0];
                       tube_base_vertices[count2].color[1]=1;//glyphColors[i][j][1];
                       tube_base_vertices[count2].color[2]=1;//glyphColors[i][j][2];
                       tube_base_vertices[count2].color[3]=1;//glyphColors[i][j][3];

                       count2++;

                       int seg = cylinder_segment*2/REPEAT_CYLINDER_TEXTURE;
                       int white_end = 1;
                       for(int t=0;t<cylinder_segment*2*2;t++)
                       {
                            tube_vertices[count1].pos[0]=tube_seg_pos[t][0];
                            tube_vertices[count1].pos[1]=tube_seg_pos[t][1];
                            tube_vertices[count1].pos[2]=tube_seg_pos[t][2];
                            tube_vertices[count1].norm[0]=tube_seg_norm[t][0];
                            tube_vertices[count1].norm[1]=tube_seg_norm[t][1];
                            tube_vertices[count1].norm[2]=tube_seg_norm[t][2];

                            int mm = SUB_SEGMENT*2;
                            if((t/mm)%2==0)//t/seg>=0 && t/seg<=3)
                            {
                              tube_vertices[count1].color[0]=1;//glyphColors[i][j][0];
                              tube_vertices[count1].color[1]=1;//glyphColors[i][j][1];
                              tube_vertices[count1].color[2]=1;//glyphColors[i][j][2];
                              tube_vertices[count1].color[3]=1;//glyphColors[i][j][3];
                            }
                            else
                            {
                              tube_vertices[count1].color[0]=0;//glyphColors[i][j][0];
                              tube_vertices[count1].color[1]=0;//glyphColors[i][j][1];
                              tube_vertices[count1].color[2]=0;//glyphColors[i][j][2];
                              tube_vertices[count1].color[3]=glyphColors[i][j][3];
                            }
                            if(t%2==0)
                            {
                               tube_base_vertices[count2].pos[0]=tube_seg_pos[t][0];
                               tube_base_vertices[count2].pos[1]=tube_seg_pos[t][1];
                               tube_base_vertices[count2].pos[2]=tube_seg_pos[t][2];
                               tube_base_vertices[count2].norm[0]=-dir[i][j][0];
                               tube_base_vertices[count2].norm[1]=-dir[i][j][1];
                               tube_base_vertices[count2].norm[2]=-dir[i][j][2];
                               tube_base_vertices[count2].color[0]=1;//glyphColors[i][j][0];
                               tube_base_vertices[count2].color[1]=1;//glyphColors[i][j][1];
                               tube_base_vertices[count2].color[2]=1;//glyphColors[i][j][2];
                               tube_base_vertices[count2].color[3]=1;//glyphColors[i][j][3];
                               count2++;
                             }
                            count1++;
                        }
              }
    }
    cerr<<"lt geneate tube "<<count1<<" "<<count2<<endl;
}

void svLTCylinder::GenerateIndex()
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
                     for(int t=0;t<cylinder_segment*2*2;t++)
                     {
                            if(t%4==0) //&&t<cylinder_segment*2*2-4
                            {
                                 if(visibleLabel[i][j])
                                 {
                                    tube_indices[index1]=count1;
                                    tube_indices[index1+1]=count1+1;
                                    tube_indices[index1+2]=count1+3;
                                    tube_indices[index1+3]=count1+2;
                                    index1+=4;
                                  }
                            }
                            count1++;
                      }
                     for(int t=0;t<cylinder_segment*2-1;t++)
                     {//cerr<<t<<" "<<tbcount<<" "<<tbindex<<" "<<" "<<tube_base_vertices_size<<" "<<tube_base_indices_size<<endl;
                           if(visibleLabel[i][j])
                           {
                                  tube_base_indices[tbindex]  =start1;
                                  tube_base_indices[tbindex+1]=tbcount+1;
                                  tube_base_indices[tbindex+2]=tbcount;
                                  tbindex+=3;
                           }
                           tbcount++;
                     }
                     tbcount++;
             }
   }
cerr<<"lt generate index "<<count1<<" "<<tbcount<<" "<<index1<<" "<<tbindex<<endl;
}

void svLTCylinder::GenerateLegend()
{
    int count = 0;
    for(int i =0;i<seed_num;i++)
    {
                for(int j=0;j<glyph[i].size();j++)
                {
                        double radius = glyphRadius;
                        svVector3 end;
                        svScalar scale = 5.*glyphScale;
                        end[0] = glyph[i][j][0]+scale*dir[i][j][0];
                        end[1] = glyph[i][j][1]+scale*dir[i][j][1];
                        end[2] = glyph[i][j][2]+scale*dir[i][j][2];
                        legend_vertices[count].pos[0]=glyph[i][j][0];
                        legend_vertices[count].pos[1]=glyph[i][j][1];
                        legend_vertices[count].pos[2]=glyph[i][j][2];
                        legend_vertices[count+1].pos[0]=end[0];
                        legend_vertices[count+1].pos[1]=end[1];
                        legend_vertices[count+1].pos[2]=end[2];
                        end[0] = glyph[i][j][0]+scale*dir[i][j][0];
                        end[1] = glyph[i][j][1]+scale*dir[i][j][1];
                        end[2] = glyph[i][j][2]+scale*dir[i][j][2];

                 }

      }
}

void svLTCylinder::RenderLegend()
{
   glDisable(GL_LIGHTING);
   int count = 0;
   glColor3f(0.,0.,0.);
   glBegin(GL_LINES);
   for(int i=0;i<seed_num;i++)
   {
        for(int j=0;j<glyph[i].size();j++)
        {
             if(visibleLabel[i][j])
            {
             glVertex3f(legend_vertices[count].pos[0],
                       legend_vertices[count].pos[1],
                       legend_vertices[count].pos[2]);
             glVertex3f(legend_vertices[count+1].pos[0],
                       legend_vertices[count+1].pos[1],
                       legend_vertices[count+1].pos[2]);

            }
             count+=2;
        }
   }
   glEnd();
   glEnable(GL_LIGHTING);

}

}

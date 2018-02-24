#include "svLLPCylinder.h"
#include <algorithm>

using namespace std;

namespace __svl_lib {

svLLPCylinder::svLLPCylinder(svVectorField *f) : svCylinderGlyph(f)
{

}
void svLLPCylinder::RenderSample()
{
    for(int i=0;i<sampleLabel.size();)
    {
           int layer = sampleLabel[i];
           int index = sampleLabel[i+1];

           svScalar length = (exp[layer][index]+expScale) * glyphScale;
           svVector3 end = glyph[layer][index]
                       + dir[layer][index] * length;
           glColor4f(glyphColors[layer][index][0],glyphColors[layer][index][1],
                     glyphColors[layer][index][2],glyphColors[layer][index][3]);
           RenderCylinder(glyph[layer][index], dir[layer][index],
                       glyphScale/2.,//glyphTubeRadius,
                       length, CYLINDERSLICE);
           length = coe[layer][index] * glyphScale;
           end = glyph[layer][index]
                       + dir[layer][index] * length;
           RenderCylinder(glyph[layer][index], dir[layer][index],
                       glyphScale/4.,//glyphTubeRadius/2.,
                       length, CYLINDERSLICE);
           i+=2;
    }
}
void svLLPCylinder::UpdateLLPData()
{
    CleanData();
   
    dataSize = 0;
    for(int i=0;i<seed_num;i++)
    {
        for(int j=0;j<glyph[i].size();j++)
              dataSize++;
    }

     tube_vertices_size = dataSize*(CYLINDERSLICE+1)*2*2;
     tube_base_vertices_size = dataSize*(CYLINDERSLICE+1+1);
cerr<<"llp data "<<tube_vertices_size<<" "<<tube_base_vertices_size<<endl;
     tube_vertices = new VBOVertex[tube_vertices_size];//svVector3Array[seed_num];
     tube_base_vertices = new VBOVertex[tube_base_vertices_size];//svVector3Array[seed_num];
     legend_vertices = new VBOVertex[dataSize*2];
}
void svLLPCylinder::UpdateLLPIndex()
{
     CleanIndex();

     int count = 0;
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<glyph[i].size();j++)
          {
                   if(visibleLabel[i][j])
                          count++;
          }
     }
     tube_indices_size = count*4*CYLINDERSLICE*2;
     tube_base_indices_size = count*3*CYLINDERSLICE;
     tube_indices = new int[tube_indices_size];
     tube_base_indices = new int[tube_base_indices_size];
cerr<<"llp index"<<tube_indices_size <<" "<< tube_base_indices_size<<endl;
}
void svLLPCylinder::GenerateTubes()
{
    int count1 = 0;
    int count2 = 0;
    for(int i =0;i<seed_num;i++)
    {
                for(int j=0;j<glyph[i].size();j++)
                {
                        double radius = glyphScale;//glyphTubeRadius;
                        svVector3 end;
                        svScalar scale = (exp[i][j]+expScale)*glyphScale;
                        svVector3 tube_seg_norm[(CYLINDERSLICE+1)*2];
                        svVector3 tube_seg_pos[(CYLINDERSLICE+1)*2];

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
//====================================================
                       radius = glyphScale/2.;//glyphTubeRadius/2.;
                        scale = coe[i][j]*glyphScale;

                        GetCylinder(glyph[i][j], dir[i][j], radius,
                                 scale,  CYLINDERSLICE,
                                  tube_seg_norm, tube_seg_pos);

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
                            count1++;
                        }
                }
    }
cerr<<"lt geneate "<<count1<<" "<<count2<<endl;
}

void svLLPCylinder::GenerateIndex()
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
                            if(t%2==0&&t<(CYLINDERSLICE)*2)
                            {
                                 if(visibleLabel[i][j])
                                 {
                                  tube_indices[index1]  =count1;
                                  tube_indices[index1+1]=count1+1;
                                  tube_indices[index1+2]=count1+3;
                                  tube_indices[index1+3]=count1+2;
                                  index1+=4;
                                  }
                            }
                            count1++;
                      }
                     for(int t=0;t<CYLINDERSLICE;t++)
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
                     for(int t=0;t<(CYLINDERSLICE+1)*2;t++)
                     {
                            if(t%2==0&&t<(CYLINDERSLICE)*2)
                            {
                                 if(visibleLabel[i][j])
                                 {
                                  tube_indices[index1]  =count1;
                                  tube_indices[index1+1]=count1+1;
                                  tube_indices[index1+2]=count1+3;
                                  tube_indices[index1+3]=count1+2;
                                  index1+=4;
                                  }
                            }
                            count1++;
                      }
             }
   }
cerr<<"llp geneate index "<<count1<<" "<<tbcount<<" "<<index1<<" "<<tbindex<<endl;
}

void svLLPCylinder::GenerateLegend()
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
                        count+=2;
                 }

      }
}

void svLLPCylinder::RenderLegend()
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

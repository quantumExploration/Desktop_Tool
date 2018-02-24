#include "svLogCylinder.h"
#include <algorithm>
#include <cmath>

using namespace std;

namespace __svl_lib {

svLogCylinder::svLogCylinder(svVectorField *f) : svCylinderGlyph(f)
{

}
void svLogCylinder::RenderSample()
{
    for(int i=0;i<sampleLabel.size();)
    {
           int layer = sampleLabel[i];
           int index = sampleLabel[i+1];

           svScalar den;
           if(mag>0) den = log10(mag[layer][index]);
           else den = 0;

           svScalar length = den * glyphScale;
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

void svLogCylinder::GenerateTubes()
{
    int count1 = 0;
    int count2 = 0;
    for(int i =0;i<seed_num;i++)
    {
                for(int j=0;j<glyph[i].size();j++)
                {
                        double radius = glyphTubeRadius;
                        svVector3 end;
                        svScalar den;
                        if(mag[i][j]>0)
                              den = log10(mag[i][j]);
                        else
                              den = 0;
                        svScalar scale = den*glyphScale;
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

void svLogCylinder::GenerateIndex()
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

}

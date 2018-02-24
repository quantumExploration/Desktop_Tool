#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include "svLTArrow.h"
#include "svException.h"
#include "svUtil.h"

// should change later - add to renderproperty
#define REPEAT_CYLINDER_TEXTURE 5
#define SUB_SEGMENT 4

using namespace std;

namespace __svl_lib {
svLTArrow::svLTArrow(svQDOTData * d):svArrowGlyph(d)//svVectorField* f, int numPlane)
{
 
  //field = f;
  // only handle internal format
  //ENABLE_COLOR=false;
  //ENABLE_LINEWIDTH = false;

  //display_mode = SV_STORE;
  //myData->GetPhysicalDimension(&lbbox, &rbbox);

//  glyphScale = DEFAULT_GLYPH_SCALE;
//  glyphSize= DEFAULT_GLYPH_SIZE;
//  glyphRadius = DEFAULT_GLYPH_RADIUS;
//  display_list = DEFAULT_DISPLAYLIST;
//   expScale = 16;
/*  svScalar minExp = 9e+9;
  svScalar maxExp = -9e+9;
  for(int i=0;i<myData->splitData.size();i++){
     for(int j=0;j<myData->splitData[i].size();j++){
        svScalar exp = (*myData->splitData[i][j]).exp;
        if(maxExp <exp && (*myData->splitData[i][j]).coe>1e-8) maxExp = exp;
        if(minExp >exp && (*myData->splitData[i][j]).coe>1e-8) minExp = exp;
     }
  }*/ 
  expScale = fabs(myData->minExp);
  numPower = myData->maxExp - myData->minExp + 1;

  //cerr<<expScale<<" "<<numPower<<" "<<myData->myQDOT->maxExp<<" "<<myData->myQDOT->minExp<<endl;

  secondColors = new svVector4Array[seed_num];
  svVector4 black(1,1,1,1);
  for(int i=0;i<seed_num;i++){
     for(int j=0;j<myData->splitData[i].size();j++){
        secondColors[i].add(black);
     }
  }

}

void svLTArrow::SetColors(svColors* color){
  color->GetColors(myData, glyphColors, secondColors);
}

void svLTArrow::UpdateData(){
    CleanData();
    cleancolor();

    int dataSize = 0;
    for(int i=0;i<seed_num;i++){
        for(int j=0;j<myData->splitData[i].size();j++)
        {
               dataSize++;
        }
    }

     glyphColors = new svVector4Array[seed_num];
     svVector4 black(0,0,0,1);
     for(int i=0;i<seed_num;i++){
       for(int j=0;j<myData->splitData[i].size();j++){
         glyphColors[i].add(black);
       }
     }
  secondColors = new svVector4Array[seed_num];
  svVector4 white(1,1,1,1);
  for(int i=0;i<seed_num;i++){
     for(int j=0;j<myData->splitData[i].size();j++){
        secondColors[i].add(white);
     }
  }

   cerr<<"dataSize "<<dataSize<<endl;
    cylinder_segment = (REPEAT_CYLINDER_TEXTURE)*(SUB_SEGMENT);
    int arrow_segment = (REPEAT_CYLINDER_TEXTURE)*(SUB_SEGMENT);

    arrow_vertices_size = dataSize*((arrow_segment)*2+arrow_segment);
    arrow_base_vertices_size = dataSize*(arrow_segment*2+1);
    arrow_vertices = new VBOVertex[arrow_vertices_size];
    arrow_base_vertices = new VBOVertex[arrow_base_vertices_size];
 
    tube_vertices_size = dataSize*(cylinder_segment) * 2 * 2;
    tube_base_vertices_size = dataSize*((cylinder_segment)*2+1);
    tube_vertices = new VBOVertex[tube_vertices_size];//svVector3Array[seed_num];
    tube_base_vertices = new VBOVertex[tube_base_vertices_size];//svVector3Array[seed_num];

    legend_vertices = new VBOVertex[dataSize*2];
}

void svLTArrow::UpdateIndex(){
    CleanIndex();

    int count = 0;
     for(int i=0;i<seed_num;i++){
          for(int j=0;j<myData->splitData[i].size();j++)
          {
                   if(myData->state->qdotVisible.at(myData->splitData[i][j]))//myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
                   {
                       count++;
                   }
          }
     }
     arrow_indices_size = count*3*cylinder_segment;
     arrow_base_indices_size = count*3*(2*cylinder_segment-1);
     arrow_indices = new GLuint[arrow_indices_size];
     arrow_base_indices = new GLuint[arrow_base_indices_size];

     tube_indices_size = count*4*(cylinder_segment);//CYLINDERSLICE;
     tube_base_indices_size = count*3*(2*cylinder_segment-1);//CYLINDERSLICE;
     tube_indices = new GLuint[(int)(tube_indices_size)];
     tube_base_indices = new GLuint[tube_base_indices_size];
     cerr<<"lt index "<<tube_indices_size<<" "<<tube_base_indices_size<<endl;
}
/*
void svLTArrow::UpdateIndex(int region){
    CleanIndex();

    int count = 0;
     for(int i=0;i<seed_num;i++){
          for(int j=0;j<myData->GetSeed(i);j++)
          {
                   if(myData->GetVisibleLabel(i,j)&& myData->GetRegion(i,j)==region)//visibleLabel[i][j])
                   {
                       count++;
                   }
          }
     }
cerr<<"update index "<<count<<endl;
     arrow_indices_size = count*3*cylinder_segment;
     arrow_base_indices_size = count*3*(2*cylinder_segment-1);
     arrow_indices = new GLuint[arrow_indices_size];
     arrow_base_indices = new GLuint[arrow_base_indices_size];

     tube_indices_size = count*4*(cylinder_segment);//CYLINDERSLICE;
     tube_base_indices_size = count*3*(2*cylinder_segment-1);//CYLINDERSLICE;
     tube_indices = new GLuint[(tube_indices_size)];
     tube_base_indices = new GLuint[tube_base_indices_size];
//     cerr<<"lt index "<<tube_indices_size<<" "<<tube_base_indices_size<<endl;
}
*/


void svLTArrow::GenerateTubes(){
    int count1 = 0;
    int count2 = 0;

    svVector4 white;
    white[0]=1; white[1]=1; white[2]=1; white[3]=1;//glyphColors[0][0][3];
    for(int i =0;i<seed_num;i++){
            for(int j=0;j<myData->splitData[i].size();j++){
                 Spin d = *(myData->splitData[i][j]);
                       double radius = glyphScale;
                       svVector3 end;
                       svScalar scale = (d.coe)*glyphScale;
                       svVector3 tube_seg_norm[(cylinder_segment)*2*2];
                       svVector3 tube_seg_pos[(cylinder_segment)*2*2];
                       svVector4 tube_seg_col[(cylinder_segment)*2*2];
                       d.dir.normalize();
                       svScalar ratio;
                       if(numPower>1)
                       {
                          ratio = (d.exp+expScale)/(svScalar)(numPower-1);
                       }
                       else
                       {
                           ratio = 1;//exp[i][j]+expScale;
                       }//cerr<<myData->GetExp(i,j)<<" "<<expScale<<" "<<numPower<<endl;
                       //ratio = (d.exp + expScale)/(13.-1.);
                       GetCylinderTexture(d.pos,d.dir, 
                                 radius,
                                 scale,   ratio,
                                  SUB_SEGMENT, REPEAT_CYLINDER_TEXTURE,
                                  tube_seg_norm, tube_seg_pos, tube_seg_col,
                                  secondColors[i][j], glyphColors[i][j]);
                       svVector3 glyph = d.pos;//myData->GetGlyph(i,j);
                       //cerr<<glyph[0]<<" "<<glyph[1]<<" "<<glyph[2]<<endl;
                       svVector3 dir = d.dir;//myData->GetDir(i,j);
                       int start= count2;
                       tube_base_vertices[count2].pos[0]=glyph[0];
                       tube_base_vertices[count2].pos[1]=glyph[1];
                       tube_base_vertices[count2].pos[2]=glyph[2];
                       tube_base_vertices[count2].norm[0]=-dir[0];
                       tube_base_vertices[count2].norm[1]=-dir[1];
                       tube_base_vertices[count2].norm[2]=-dir[2];
                       tube_base_vertices[count2].color[0]=secondColors[i][j][0];
                       tube_base_vertices[count2].color[1]=secondColors[i][j][1];
                       tube_base_vertices[count2].color[2]=secondColors[i][j][2];
                       tube_base_vertices[count2].color[3]=secondColors[i][j][3];

                       count2++;
                       int seg = cylinder_segment*2/REPEAT_CYLINDER_TEXTURE;
                       int white_end = 1;
                       for(int t=0;t<cylinder_segment*2*2;t++)
                       {
                            tube_seg_norm[t].normalize();
                            tube_vertices[count1].pos[0]=tube_seg_pos[t][0];
                            tube_vertices[count1].pos[1]=tube_seg_pos[t][1];
                            tube_vertices[count1].pos[2]=tube_seg_pos[t][2];
                            tube_vertices[count1].norm[0]=tube_seg_norm[t][0];
                            tube_vertices[count1].norm[1]=tube_seg_norm[t][1];
                            tube_vertices[count1].norm[2]=tube_seg_norm[t][2];
                            tube_vertices[count1].color[0]=tube_seg_col[t][0];
                            tube_vertices[count1].color[1]=tube_seg_col[t][1];
                            tube_vertices[count1].color[2]=tube_seg_col[t][2];
                            tube_vertices[count1].color[3]=tube_seg_col[t][3];
                            if(t%2==0)
                            {
                               tube_base_vertices[count2].pos[0]=tube_seg_pos[t][0];
                               tube_base_vertices[count2].pos[1]=tube_seg_pos[t][1];
                               tube_base_vertices[count2].pos[2]=tube_seg_pos[t][2];
                               tube_base_vertices[count2].norm[0]=-dir[0];
                               tube_base_vertices[count2].norm[1]=-dir[1];
                               tube_base_vertices[count2].norm[2]=-dir[2];
                               tube_base_vertices[count2].color[0]=secondColors[i][j][0];
                               tube_base_vertices[count2].color[1]=secondColors[i][j][1];
                               tube_base_vertices[count2].color[2]=secondColors[i][j][2];
                               tube_base_vertices[count2].color[3]=secondColors[i][j][3];
                               count2++;
                             }
                            count1++;
                        }
              }
    }
    cerr<<"lt geneate tube "<<count1<<" "<<count2<<endl;
}

void svLTArrow::SaveTubetoOSG(char *file){
        //int size = glyphPos[0].size();
        ofstream outfile(file);
        outfile<<"Geode{"<<endl;
        outfile<<"      name "<<file<<endl;
        outfile<<"      nodeMask 0xffffffff"<<endl;
        outfile<<"      cullingActive TRUE"<<endl;
        outfile<<"      num_drawables 1"<<endl;
        outfile<<"      Geometry{"<<endl;
        outfile<<"              DataVariance DYNAMIC"<<endl;
        outfile<<"              useDisplayList FALSE"<<endl;
        outfile<<"              useVertexBufferObjects TRUE"<<endl;


        //------------------------------------------------------------------
        outfile<<"              PrimitiveSets "<<1<<endl;
        outfile<<"              {"<<endl;
        //------------------Arrows-----------------------------
        outfile<<"                      DrawElementsUInt QUADS "<<tube_indices_size<<endl;
        outfile<<"                      {"<<endl;
        cerr<<tube_indices_size<<endl;
        for(int i=0;i<tube_indices_size;i+=4)
        {
              outfile<<"                              "<<i<<" "<<i+2<<" "<<i+3<<" "<<i+1<<endl;       
        }
        outfile<<"                      }"<<endl;
        outfile<<"              }"<<endl;

	int size = tube_indices_size*(cylinder_segment) * 2 * 2;
        cerr<<size<<endl;
	int count1 = 0;
        outfile<<"              VertexArray Vec3Array "<<size<<endl;
        outfile<<"              {"<<endl;
    	for(int i =0;i<seed_num;i++){
            for(int j=0;j<myData->splitData[i].size();j++){
                 bool visible = myData->state->qdotVisible.at(myData->splitData[i][j]);
                 for(int t=0;t<cylinder_segment*2*2;t++){
			if(visible){
				outfile<<"                      "
					<<tube_vertices[count1].pos[0]<<" "
                            		<<tube_vertices[count1].pos[1]<<" "
                            		<<tube_vertices[count1].pos[2]<<endl;
			}
                  	count1++;
                 }
            }
        }
        outfile<<"              }"<<endl;


        //------------Normal Vector--------------------
	count1= 0;
        outfile<<"              NormalBinding PER_VERTEX"<<endl;
        outfile<<"              NormalArray Vec3Array "<<size<<endl;
        outfile<<"              {"<<endl;
    	for(int i =0;i<seed_num;i++){
            for(int j=0;j<myData->splitData[i].size();j++){
                 bool visible = myData->state->qdotVisible.at(myData->splitData[i][j]);
                 for(int t=0;t<cylinder_segment*2*2;t++){
			if(visible){
				outfile<<"                      "
					<<tube_vertices[count1].norm[0]<<" "
                            		<<tube_vertices[count1].norm[1]<<" "
                            		<<tube_vertices[count1].norm[2]<<endl;
			}
                  	count1++;
                 }
            }
        }
        outfile<<"              }"<<endl;

        //-------------------Color array---------------------------
	count1=0;
        outfile<<"              ColorBinding PER_VERTEX"<<endl;
        outfile<<"              ColorArray Vec4Array "<<size<<endl;
        outfile<<"              {"<<endl;
    	for(int i =0;i<seed_num;i++){
            for(int j=0;j<myData->splitData[i].size();j++){
                 bool visible = myData->state->qdotVisible.at(myData->splitData[i][j]);
                 for(int t=0;t<cylinder_segment*2*2;t++){
			if(visible){
				outfile<<"                      "
					<<tube_vertices[count1].color[0]<<" "
                            		<<tube_vertices[count1].color[1]<<" "
                            		<<tube_vertices[count1].color[2]<<" "
					<<tube_vertices[count1].color[3]<<endl;
			}
                  	count1++;
                 }
            }
        }
        outfile<<"              }"<<endl;
        outfile<<"      }"<<endl;
        outfile<<"}"<<endl;
        outfile.close();
}

void svLTArrow::SaveArrowtoOSG(char *file){
        //int size = glyphPos[0].size();
            int arrow_segment = (REPEAT_CYLINDER_TEXTURE)*(SUB_SEGMENT);

	ofstream outfile(file);

        outfile<<"Geode{"<<endl;
        outfile<<"      name "<<file<<endl;
        outfile<<"      nodeMask 0xffffffff"<<endl;
        outfile<<"      cullingActive TRUE"<<endl;
        outfile<<"      num_drawables 1"<<endl;
        outfile<<"      Geometry{"<<endl;
        outfile<<"              DataVariance DYNAMIC"<<endl;
        outfile<<"              useDisplayList FALSE"<<endl;
        outfile<<"              useVertexBufferObjects TRUE"<<endl;


        //------------------------------------------------------------------
        outfile<<"              PrimitiveSets "<<1<<endl;
        outfile<<"              {"<<endl;
        //------------------Arrows-----------------------------
        outfile<<"                      DrawElementsUInt TRIANGLES "<<arrow_indices_size<<endl;
        outfile<<"                      {"<<endl;
        for(int i=0;i<arrow_indices_size;i+=3)
        {
              outfile<<"                              "<<i<<" "<<i+1<<" "<<i+2<<endl;       
        }
        outfile<<"                      }"<<endl;
        outfile<<"              }"<<endl;

	int size = arrow_indices_size*((arrow_segment)*2+arrow_segment);
        outfile<<"              VertexArray Vec3Array "<<size<<endl;
        outfile<<"              {"<<endl;
        int count2 = 0;
  	for(int i =0;i<seed_num;i++){
             for(int j=0;j<myData->splitData[i].size();j++){
                     bool visible = myData->state->qdotVisible.at(myData->splitData[i][j]);
                     for(int t=0;t<cylinder_segment*3;t++){
                            if(visible){
					outfile<<"                      "
                        			<<arrow_vertices[count2].pos[0]
                        			<<" "<<arrow_vertices[count2].pos[1]
                        			<<" "<<arrow_vertices[count2].pos[2]<<endl;
                            }
                            count2++;
                      }
             }
   	}
        outfile<<"              }"<<endl;


        //------------Normal Vector--------------------
        outfile<<"              NormalBinding PER_VERTEX"<<endl;
        outfile<<"              NormalArray Vec3Array "<<size<<endl;
        outfile<<"              {"<<endl;
	count2= 0 ;
  	for(int i =0;i<seed_num;i++){
             for(int j=0;j<myData->splitData[i].size();j++){
                     bool visible = myData->state->qdotVisible.at(myData->splitData[i][j]);
                     for(int t=0;t<cylinder_segment*3;t++){
                            if(visible){
					outfile<<"                      "
                        			<<arrow_vertices[count2].norm[0]
                        			<<" "<<arrow_vertices[count2].norm[1]
                        			<<" "<<arrow_vertices[count2].norm[2]<<endl;
                            }
                            count2++;
                      }
             }
   	}
        outfile<<"              }"<<endl;

        //-------------------Color array---------------------------
        outfile<<"              ColorBinding PER_VERTEX"<<endl;
        outfile<<"              ColorArray Vec4Array "<<size<<endl;
        outfile<<"              {"<<endl;
	count2=0;
  	for(int i =0;i<seed_num;i++){
             for(int j=0;j<myData->splitData[i].size();j++){
                     bool visible = myData->state->qdotVisible.at(myData->splitData[i][j]);
                     for(int t=0;t<cylinder_segment*3;t++){
                            if(visible){
					outfile<<"                      "
                        			<<arrow_vertices[count2].color[0]
                        			<<" "<<arrow_vertices[count2].color[1]
                        			<<" "<<arrow_vertices[count2].color[2]
                        			<<" "<<arrow_vertices[count2].color[3]<<endl;
                            }
                            count2++;
                      }
             }
   	}
        outfile<<"              }"<<endl;
        outfile<<"      }"<<endl;
        outfile<<"}"<<endl;
        outfile.close();
}
/*
void svLTArrow::GenerateIndex(int region){
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
             for(int j=0;j<myData->GetSeed(i);j++)
             {
                     int start1 = tbcount;
                     tbcount++;
                     for(int t=0;t<cylinder_segment*2*2;t++)
                     {
                            if(t%4==0) //&&t<cylinder_segment*2*2-4
                            {
                                 if(myData->GetVisibleLabel(i,j) && myData->GetRegion(i,j)==region)//visibleLabel[i][j])
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
                     for(int t=0;t<cylinder_segment*2-1;t++){
                           if(myData->GetVisibleLabel(i,j)&& myData->GetRegion(i,j)==region)//visibleLabel[i][j])
                           {
                                  tube_base_indices[tbindex]  =start1;
                                  tube_base_indices[tbindex+1]=tbcount;
                                  tube_base_indices[tbindex+2]=tbcount+1;
                                  tbindex+=3;
                           }
                           tbcount++;
                     }
                     tbcount++;
             }
   }

   for(int i =0;i<seed_num;i++){
             for(int j=0;j<myData->GetSeed(i);j++){
                     int start2 = abcount;
                     abcount++;
                     for(int t=0;t<cylinder_segment*3;t++){
                            if(t%3==0){
                                 if(myData->GetVisibleLabel(i,j)&& myData->GetRegion(i,j)==region){
                                    arrow_indices[index2]=count2;
                                    arrow_indices[index2+1]=count2+1;
                                    arrow_indices[index2+2]=count2+2;
                                    index2+=3;
                                  }
                            }
                            count2++;
                      }
                     for(int t=0;t<cylinder_segment*2-1;t++){
                           if(myData->GetVisibleLabel(i,j)&& myData->GetRegion(i,j)==region)//visibleLabel[i][j])
                           {
                                  arrow_base_indices[abindex]  =start2;
                                  arrow_base_indices[abindex+1]=abcount;
                                  arrow_base_indices[abindex+2]=abcount+1;
                                  abindex+=3;
                           }
                           abcount++;
                     }
                     abcount++;
             }
   }
cerr<<"lt generate index "<<count2<<" "<<abcount<<" "<<index2<<" "<<abindex<<" "<<arrow_indices_size<<" "<<arrow_base_indices_size<<" "<<arrow_vertices_size<<" "<<arrow_base_vertices_size<<endl;
}
*/

void svLTArrow::GenerateIndex(){
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
             for(int j=0;j<myData->splitData[i].size();j++)//myData->GetSeed(i);j++)
             {
                     int start1 = tbcount;
                     tbcount++;
                     bool visible = myData->state->qdotVisible.at(myData->splitData[i][j]);
                     for(int t=0;t<cylinder_segment*2*2;t+=4)
                     {
                            if(t%4==0) //&&t<cylinder_segment*2*2-4
                            {
                                 if(visible)//myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
                                 {
                                    tube_indices[index1]=count1;
                                    tube_indices[index1+1]=count1+1;
                                    tube_indices[index1+2]=count1+3;
                                    tube_indices[index1+3]=count1+2;
                                    index1+=4;
                                  }
                            }
                            count1+=4;
                      }
                     for(int t=0;t<cylinder_segment*2-1;t++){
                           if(visible)//myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
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

   for(int i =0;i<seed_num;i++){
             for(int j=0;j<myData->splitData[i].size();j++){
                     int start2 = abcount;
                     abcount++;
                    bool visible = myData->state->qdotVisible.at(myData->splitData[i][j]);

                     for(int t=0;t<cylinder_segment*3;t++){
                            if(t%3==0){
                                 if(visible){//myData->GetVisibleLabel(i,j)){
                                    arrow_indices[index2]=count2;
                                    arrow_indices[index2+1]=count2+1;
                                    arrow_indices[index2+2]=count2+2;
                                    index2+=3;
                                  }
                            }
                            count2++;
                      }
                     for(int t=0;t<cylinder_segment*2-1;t++){
                           if(visible)//myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
                           {
                                  arrow_base_indices[abindex]  =start2;
                                  arrow_base_indices[abindex+1]=abcount+1;
                                  arrow_base_indices[abindex+2]=abcount;
                                  abindex+=3;
                           }
                           abcount++;
                     }
                     abcount++;
             }
   }
cerr<<"lt generate index "<<count1<<" "<<count2<<" "<<abcount<<" "<<index2<<" "<<abindex<<" "<<arrow_indices_size<<" "<<arrow_base_indices_size<<" "<<arrow_vertices_size<<" "<<arrow_base_vertices_size<<endl;

//   cerr<<"lt generate index "<<count1<<" "<<tbcount<<" "<<index1<<" "<<tbindex<<endl;
}
/*
void svLTArrow::GenerateLegend(){
    int count = 0;
    for(int i =0;i<seed_num;i++)
    {
                for(int j=0;j<myData->GetSeed(i);j++)
                {
                        double radius = glyphRadius;
                        svVector3 end;
                        svScalar scale = 5.*glyphScale;
                        svVector3 glyph = myData->GetGlyph(i,j);
                        svVector3 dir = myData->GetGlyph(i,j);
                        end[0] = glyph[0]+scale*dir[0];
                        end[1] = glyph[1]+scale*dir[1];
                        end[2] = glyph[2]+scale*dir[2];
                        legend_vertices[count].pos[0]=glyph[0];
                        legend_vertices[count].pos[1]=glyph[1];
                        legend_vertices[count].pos[2]=glyph[2];
                        legend_vertices[count+1].pos[0]=end[0];
                        legend_vertices[count+1].pos[1]=end[1];
                        legend_vertices[count+1].pos[2]=end[2];
                        end[0] = glyph[0]+scale*dir[0];
                        end[1] = glyph[1]+scale*dir[1];
                        end[2] = glyph[2]+scale*dir[2];
                 }
      }
}

void svLTArrow::RenderLegend()
{
   glDisable(GL_LIGHTING);
   int count = 0;
   glColor3f(0.,0.,0.);
   glBegin(GL_LINES);
   for(int i=0;i<seed_num;i++)
   {
        for(int j=0;j<myData->GetSeed(i);j++)
        {
             if(myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
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
*/


void svLTArrow::GenerateArrows()
{
    int count1 = 0;
    int index = 0;
    int count2 = 0;

    svVector4 white;
    white[0]=1; white[1]=1; white[2]=1; white[3]=1;//glyphColors[0][0][3];

    for(int i =0;i<seed_num;i++)
    {
                for(int j=0;j<myData->splitData[i].size();j++)
                {
                     Spin d = *(myData->splitData[i][j]);
                        double radius = glyphRadius;
                        svVector3 end;
                        svScalar scale = d.coe*glyphScale;
                        svVector3  glyph = d.pos;//myData->GetGlyph(i,j);
                        svVector3  dir = d.dir;//myData->GetDir(i,j);
                        svScalar exp = d.exp;//myData->GetExp(i,j);
                        svScalar ratio;
                       if(numPower>1)
                       {
                          ratio = (exp+expScale)/(svScalar)(numPower-1);
                       }
                       else
                       {
                           ratio = 1;//exp[i][j]+expScale;
                       }
                       if(d.coe<1e-8) ratio = 0;
                       // ratio = (exp+expScale)/(13.-1.);
                        end[0] = glyph[0]+scale*dir[0];
                        end[1] = glyph[1]+scale*dir[1];
                        end[2] = glyph[2]+scale*dir[2];
                        svVector3 cone_seg_norm[cylinder_segment*3];
                        svVector3  cone_seg_pos[cylinder_segment*3];
                        svVector4 cone_seg_col[cylinder_segment*3];
                        GetConeTexture(end, dir, radius, 
                                 radius*3., ratio, SUB_SEGMENT, REPEAT_CYLINDER_TEXTURE,
                                  cone_seg_norm, cone_seg_pos, cone_seg_col,
                                  secondColors[i][j], glyphColors[i][j]);
                       int start= count2;
                       arrow_base_vertices[count2].pos[0]=end[0];
                       arrow_base_vertices[count2].pos[1]=end[1];
                       arrow_base_vertices[count2].pos[2]=end[2];
                       arrow_base_vertices[count2].norm[0]=-dir[0];
                       arrow_base_vertices[count2].norm[1]=-dir[1];
                       arrow_base_vertices[count2].norm[2]=-dir[2];
                       arrow_base_vertices[count2].color[0]=secondColors[i][j][0];
                       arrow_base_vertices[count2].color[1]=secondColors[i][j][1];
                       arrow_base_vertices[count2].color[2]=secondColors[i][j][2];
                       arrow_base_vertices[count2].color[3]=secondColors[i][j][3];

                       count2++;

                       for(int t=0;t<cylinder_segment * 3; t++){
                          arrow_vertices[count1].pos[0]=cone_seg_pos[t][0];
                          arrow_vertices[count1].pos[1]=cone_seg_pos[t][1];
                          arrow_vertices[count1].pos[2]=cone_seg_pos[t][2];
                          arrow_vertices[count1].norm[0]=cone_seg_norm[t][0];
                          arrow_vertices[count1].norm[1]=cone_seg_norm[t][1];
                          arrow_vertices[count1].norm[2]=cone_seg_norm[t][2]; 
                          arrow_vertices[count1].color[0]=cone_seg_col[t][0];
                          arrow_vertices[count1].color[1]=cone_seg_col[t][1];
                          arrow_vertices[count1].color[2]=cone_seg_col[t][2];
                          arrow_vertices[count1].color[3]=cone_seg_col[t][3];
                          if(t%3!=2){
                             arrow_base_vertices[count2].pos[0]=cone_seg_pos[t][0];
                             arrow_base_vertices[count2].pos[1]=cone_seg_pos[t][1];
                             arrow_base_vertices[count2].pos[2]=cone_seg_pos[t][2];
                             arrow_base_vertices[count2].norm[0]=-dir[0];
                             arrow_base_vertices[count2].norm[1]=-dir[1];
                             arrow_base_vertices[count2].norm[2]=-dir[2];
                             arrow_base_vertices[count2].color[0]=secondColors[i][j][0];
                             arrow_base_vertices[count2].color[1]=secondColors[i][j][1];
                             arrow_base_vertices[count2].color[2]=secondColors[i][j][2];
                             arrow_base_vertices[count2].color[3]=secondColors[i][j][3];
                             count2++;
                          }
                          count1++;
                       }
                }
   }
 //  cerr<<"arrow "<<count<<" "<<index<<" "<<arrow_vertices_size<<" "<<arrow_indices_size<<" "<<arrow_base_vertices_size<<endl;
}
/*
void svSplitArrow::GenerateVBO()
{
   arrow_indices_size = dataSize*3*ARROWSLICE;
   int num =dataSize*(ARROWSLICE+1); 

   glGenBuffers(1, &ARROW_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, ARROW_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*num,
                arrow_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   glGenBuffers(1, &ARROW_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*arrow_indices_size,
                arrow_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


   num =dataSize*(CYLINDERSLICE+1)*2;

   glGenBuffers(1, &TUBE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, TUBE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*num,
                tube_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   glGenBuffers(1, &TUBE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*tube_indices_size,
                tube_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

}

void svSplitArrow::RenderVBO()
{
   glBindBuffer(GL_ARRAY_BUFFER, ARROW_VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_IVBO);

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);

   glVertexPointer(3, GL_FLOAT, sizeof(VBOVertex), (char*)NULL+0);
   glNormalPointer(GL_FLOAT, sizeof(VBOVertex), (char*)NULL+sizeof(float)*3);
   glColorPointer(4, GL_FLOAT, sizeof(VBOVertex),(char*)NULL+sizeof(float)*6);

   glDrawElements(GL_TRIANGLES, arrow_indices_size, GL_UNSIGNED_INT,NULL);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);

//==============================================

   glBindBuffer(GL_ARRAY_BUFFER, TUBE_VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_IVBO);

   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_NORMAL_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);

   glVertexPointer(3, GL_FLOAT, sizeof(VBOVertex), (char*)NULL+0);
   glNormalPointer(GL_FLOAT, sizeof(VBOVertex), (char*)NULL+sizeof(float)*3);
   glColorPointer(4, GL_FLOAT, sizeof(VBOVertex),(char*)NULL+sizeof(float)*6);

   glDrawElements(GL_QUADS, tube_indices_size, GL_UNSIGNED_INT,NULL);

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);

}
*/
//need to be fixed
/*void svLTArrow::RenderSample()
{
    for(int i=0;i<myData->GetSampleLabelSize();)//sampleLabel.size();)
    {
           int layer = myData->GetSampleLabel(i);//sampleLabel[i];
           int index = myData->GetSampleLabel(i+1);//sampleLabel[i+1];

           svScalar coe = myData->GetCoe(layer, index);
           svVector3 glyph = myData->GetGlyph(layer, index);
           svVector3 dir = myData->GetDir(layer, index);

           svScalar length = coe * glyphScale;
           svVector3 end = glyph 
                       + dir * length;
           glColor4f(glyphColors[layer][index][0],glyphColors[layer][index][1],
                     glyphColors[layer][index][2],glyphColors[layer][index][3]);
           RenderCone(end, dir, glyphRadius, 
                       glyphRadius*3, ARROWSLICE);
           RenderCylinder(glyph, dir, 
                       glyphTubeRadius,
                       length, CYLINDERSLICE);  
           i+=2;
    }	
}*/
/*
void svSplitArrow::Render()
{
   
  //if(display_mode == SV_IMMEDIATE) {
    //Generate();
  //}

  if(display_mode == SV_DISPLAYLIST && glIsList(display_list)) {
    glCallList(display_list);
   // cerr<<display_list<<endl;
  }
  else if (display_mode == SV_STORE) {
    RenderStore();
  }
  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);
}

void svSplitArrow::RenderStore() const
{
  if(svPrimitive::STILL_UPDATE ) 
  {
    return;
  }
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);     
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);

    glEnable( GL_LINE_SMOOTH );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
  for(int i=0; i<seed_num; i++)
  {
     glEnable(GL_LIGHTING);
     glEnable(GL_NORMALIZE);

     glLineWidth(1.0);
     glColor3f(0.9, 0.9f, 0.2);
     svVector3 vel=glyph[i][0].getValue();
     if(vel[0]>lbbox[0] && vel[0]<rbbox[0] &&
        vel[1]>lbbox[1] && vel[1]<rbbox[1] &&
        vel[2]>lbbox[2] && vel[2]<rbbox[2])
     {
       glPushMatrix();
       glTranslatef(vel[0],vel[1],vel[2]);
//       glutSolidSphere(glyphSize,10,10);
       glPopMatrix();
     }; 
     glDisable(GL_LIGHTING);
  };
}
*/

void svLTArrow::cleancolor(){
  for(int i=0;i<seed_num;i++) secondColors[i].free();
  delete [] secondColors;
}

}

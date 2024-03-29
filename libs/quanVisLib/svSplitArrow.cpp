#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include "svSplitArrow.h"
#include "svException.h"
#include "svUtil.h"

// should change later - add to renderproperty


using namespace std;

namespace __svl_lib {
svSplitArrow::svSplitArrow(svVectorField *f):svArrowGlyph(f)//svVectorField* f, int numPlane)
{
  //field = f;
  // only handle internal format
  ENABLE_COLOR=false;
  ENABLE_LINEWIDTH = false;

  display_mode = SV_STORE;
  field->GetPhysicalDimension(&lbbox, &rbbox);

//  glyphScale = DEFAULT_GLYPH_SCALE;
//  glyphSize= DEFAULT_GLYPH_SIZE;
//  glyphRadius = DEFAULT_GLYPH_RADIUS;
//  display_list = DEFAULT_DISPLAYLIST;
}

void svSplitArrow::GenerateSample(int frequency)//(int contour, svScalar z1, svScalar z2, int z, int frequency)
{
  display_mode = SV_DISPLAYLIST;
  glEnable(GL_CULL_FACE);
  svGlyph::STILL_UPDATE = false;

   if(glIsList(display_list))
		glDeleteLists(display_list, 1); 
    glNewList(display_list, GL_COMPILE);
    int f = 1./(float)frequency*200;
    if(frequency>20) f = 1;
    for(int i =0;i<seed_num;i++)
    {		
            //if(i>=z1 && i<=z2)
            //{
            int count = 0;
            
		for(int j=0;j<myData->GetSeed(i);j++)
		{
			if(myData->GetVisibleLabel(i,j))//visibleLabel[i][j]) //&& count%2==0) 
			{
                              count ++;
                        }
                        if(count%f==0&&myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
                        {
			glColor4f(glyphColors[i][j][0],glyphColors[i][j][1],glyphColors[i][j][2],alpha);
			double radius = glyphRadius;

			//-----------------endpoint-----------------------
			svVector3 end1, end2;
//			svScalar exp = getNumOfIntegerDigits(mag[i][j]);
//			svScalar coe = mag[i][j]/pow(10.,(double)exp);
                        svScalar scale1 = myData->GetCoe(i,j)*glyphScale;
//cerr<<mag[i][j]<<" "<<glyphScale<<" "<<exp<<" "<<pow(10.,(double)exp)<<" "<<coe<<" "<<scale1<<" ";
                        svScalar scale2 = (myData->GetExp(i,j)+expScale)*glyphScale;
                        if(fabs(myData->GetExp(i,j))<1e-16)scale2 = 0; 
//cerr<<scale2<<endl;
                        svVector3  glyph = myData->GetGlyph(i,j);
                        svVector3  dir = myData->GetDir(i,j);
			end1[0] = glyph[0]+scale1*dir[0];
			end1[1] = glyph[1]+scale1*dir[1];
			end1[2] = glyph[2]+scale1*dir[2];
			end2[0] = glyph[0]+scale2*dir[0];
                        end2[1] = glyph[1]+scale2*dir[1];
                        end2[2] = glyph[2]+scale2*dir[2];
			//------------------render -------------------
                        RenderCone(end1, dir, radius, radius*3, 3);
                        //RenderSphere(end2, radius, dir[i][j],3,4);
                        svVector3 end;
                        svScalar length;
                            length = scale1;
                        //RenderCylinderTexture(glyph[i][j], dir[i][j], glyphTubeRadius,
                        //               length, 
                        //               glyphColors[i][j],
                         //              (exp+expScale)/12., 2, 12);
                        RenderCylinder(glyph, dir, glyphTubeRadius,
                                       length, 3); 

  //                      glColor3f(0,0,0);
//                        RenderCylinder(glyph[i][j], dir[i][j], 0.005,
    //                                   5.*glyphScale, 3);
                        //  count++;
			}
                   // if(count>40)break;
		}	
            //}
          //i = i + z;
	}	
    glDisable(GL_CULL_FACE);
	
	glEndList();
}

void svSplitArrow::GenerateArrows()
{
    //arrow_indices_size = 0;
    int count = 0;
    int index = 0;
   //cerr<<"arrow "<<count<<" "<<index<<" "<<arrow_vertices_size<<" "<<arrow_indices_size<<" "<<arrow_base_vertices_size<<endl;

    for(int i =0;i<seed_num;i++)
    {
                for(int j=0;j<myData->GetSeed(i);j++)
                {
                        double radius = glyphRadius;
                        svVector3 end;
                        svScalar scale = myData->GetCoe(i,j)*glyphScale;
                        svVector3  glyph = myData->GetGlyph(i,j);
                        svVector3  dir = myData->GetDir(i,j);
                        end[0] = glyph[0]+scale*dir[0];
                        end[1] = glyph[1]+scale*dir[1];
                        end[2] = glyph[2]+scale*dir[2];
                        svVector3 cone_seg_norm[ARROWSLICE];
                        svVector3  cone_seg_pos[ARROWSLICE];
                        GetCone(end, dir, radius, 
                                 radius*3,  ARROWSLICE,
                                  cone_seg_norm, cone_seg_pos);
                        int start1 = count;
                   arrow_vertices[start1].pos[0]= end[0]+radius*3*dir[0];
                   arrow_vertices[start1].pos[1]= end[1]+radius*3*dir[1];
                   arrow_vertices[start1].pos[2]= end[2]+radius*3*dir[2];
                   arrow_vertices[start1].norm[0]= dir[0];
                   arrow_vertices[start1].norm[1]= dir[1];
                   arrow_vertices[start1].norm[2]= dir[2];
                   arrow_vertices[start1].color[0]=glyphColors[i][j][0];
                   arrow_vertices[start1].color[1]=glyphColors[i][j][1];
                   arrow_vertices[start1].color[2]=glyphColors[i][j][2];
                   arrow_vertices[start1].color[3]=glyphColors[i][j][3];
                   int start2 = count;
                   arrow_base_vertices[start2].pos[0]= end[0];
                   arrow_base_vertices[start2].pos[1]= end[1];
                   arrow_base_vertices[start2].pos[2]= end[2];
                   arrow_base_vertices[start2].norm[0]= -dir[0];
                   arrow_base_vertices[start2].norm[1]= -dir[1];
                   arrow_base_vertices[start2].norm[2]= -dir[2];
                   arrow_base_vertices[start2].color[0]=glyphColors[0];
                   arrow_base_vertices[start2].color[1]=glyphColors[1];
                   arrow_base_vertices[start2].color[2]=glyphColors[2];
                   arrow_base_vertices[start2].color[3]=glyphColors[3];
                   count++;
                        for(int t=0;t<ARROWSLICE;t++)
                        {
                   /*           arrow_indices[index]=start1;
                              if(t==ARROWSLICE-1)
                              {
                                arrow_indices[index+1] = count;
                                arrow_indices[index+2] = count-(ARROWSLICE-1);
                              }
                              else
                              {
                                arrow_indices[index+1] = count;
                                arrow_indices[index+2] = count+1;
                              }
*/

                            arrow_vertices[count].pos[0]=cone_seg_pos[t][0];
                            arrow_vertices[count].pos[1]=cone_seg_pos[t][1];
                            arrow_vertices[count].pos[2]=cone_seg_pos[t][2];
                            arrow_vertices[count].norm[0]=cone_seg_norm[t][0];
                            arrow_vertices[count].norm[1]=cone_seg_norm[t][1];
                            arrow_vertices[count].norm[2]=cone_seg_norm[t][2];
                            arrow_vertices[count].color[0]=glyphColors[i][j][0];
                            arrow_vertices[count].color[1]=glyphColors[i][j][1];
                            arrow_vertices[count].color[2]=glyphColors[i][j][2];
                            arrow_vertices[count].color[3]=glyphColors[i][j][3];

                       arrow_base_vertices[count].pos[0]=cone_seg_pos[t][0];
                       arrow_base_vertices[count].pos[1]=cone_seg_pos[t][1];
                       arrow_base_vertices[count].pos[2]=cone_seg_pos[t][2];
                       arrow_base_vertices[count].norm[0]=-dir[0];
                       arrow_base_vertices[count].norm[1]=-dir[1];
                       arrow_base_vertices[count].norm[2]=-dir[2];
                       arrow_base_vertices[count].color[0]=glyphColors[i][j][0];
                       arrow_base_vertices[count].color[1]=glyphColors[i][j][1];
                       arrow_base_vertices[count].color[2]=glyphColors[i][j][2];
                       arrow_base_vertices[count].color[3]=glyphColors[i][j][3];

                            count++;
                            index+=3;
                        }
                }
   }
   //cerr<<"arrow "<<count<<" "<<index<<" "<<arrow_vertices_size<<" "<<arrow_indices_size<<" "<<arrow_base_vertices_size<<endl;
}
void svSplitArrow::GenerateTubes()
{
    int count1 = 0;
    int count2 = 0;
    for(int i =0;i<seed_num;i++)
    {
                for(int j=0;j<myData->GetSeed(i);j++)
                {
                        double radius = glyphTubeRadius;
                        svVector3 end;
                        svScalar scale = myData->GetCoe(i,j)*glyphScale;
                        svVector3 tube_seg_norm[(CYLINDERSLICE+1)*2];
                        svVector3  tube_seg_pos[(CYLINDERSLICE+1)*2];
                        svVector3  glyph = myData->GetGlyph(i,j);
                        svVector3  dir = myData->GetDir(i,j);
                        GetCylinder(glyph, dir, radius,
                                 scale,  CYLINDERSLICE,
                                  tube_seg_norm, tube_seg_pos);
                        int start= count2;
                       tube_base_vertices[count2].pos[0]=glyph[0];
                       tube_base_vertices[count2].pos[1]=glyph[1];
                       tube_base_vertices[count2].pos[2]=glyph[2];
                       tube_base_vertices[count2].norm[0]=-dir[0];
                       tube_base_vertices[count2].norm[1]=-dir[1];
                       tube_base_vertices[count2].norm[2]=-dir[2];
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
                        tube_base_vertices[count2].norm[0]=-dir[0];
                        tube_base_vertices[count2].norm[1]=-dir[1];
                        tube_base_vertices[count2].norm[2]=-dir[2];
                        tube_base_vertices[count2].color[0]=glyphColors[i][j][0];
                        tube_base_vertices[count2].color[1]=glyphColors[i][j][1];
                        tube_base_vertices[count2].color[2]=glyphColors[i][j][2];
                        tube_base_vertices[count2].color[3]=glyphColors[i][j][3];
                        count2++;
                         }
/*
                            if(t%2==0&&t<CYLINDERSLICE*2)
                            {
                                  tube_indices[index]  =count;
                                  tube_indices[index+1]=count+1;
                                  tube_indices[index+2]=count+3;
                                  tube_indices[index+3]=count+2;
                                  index+=4;
                             } 
 */
                            count1++;
                                       
                        }
                }
   }
   //cerr<<"tube "<<count1<<" "<<tube_vertices_size<<" "<<count2<<" "<<tube_base_vertices_size<<endl;
}

void svSplitArrow::GenerateIndex()
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
                for(int j=0;j<myData->GetSeed(i);j++)
                {
                        int start1 = tbcount;
                        tbcount++;
                        for(int t=0;t<(CYLINDERSLICE+1)*2;t++)
                        {
                            if(t%2==0&&t<CYLINDERSLICE*2)
                            {
                                 if(myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
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

                        start1 = count2;
                        int start2 = count2;
                        count2++;
                        for(int t=0;t<ARROWSLICE;t++)
                        {
                            if(myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
                            {
                              arrow_indices[index2]=start1;
                              arrow_base_indices[abindex]=start1;
                              if(t==ARROWSLICE-1)
                              {
                                arrow_indices[index2+1] = count2;
                                arrow_indices[index2+2] = count2-(ARROWSLICE-1);
                                arrow_base_indices[abindex+1] = count2;
                                arrow_base_indices[abindex+2] = count2-(ARROWSLICE-1);
                              }
                              else
                              {
                                arrow_indices[index2+1] = count2;
                                arrow_indices[index2+2] = count2+1;
                                arrow_base_indices[abindex+1] = count2;
                                arrow_base_indices[abindex+2] = count2+1;
                              }
                              index2+=3;
                              abindex+=3;
                             }
                            count2++;
                        }
                }
   }
   //cerr<<arrow_indices_size<<" "<<tube_indices_size<<" "<<count1<<" "<<count2<<" "<<index1<<" "<<index2<<" "<<arrow_base_indices_size<<" "<<tube_base_indices_size<<" "<<tbindex<<" "<<abindex<<" "<<tbcount<<endl;
}

void svSplitArrow::GenerateLegend()
{
    int count = 0;
    for(int i =0;i<seed_num;i++)
    {
                for(int j=0;j<myData->GetSeed(i);j++)
                {
                        double radius = glyphRadius;
                        svVector3 end;
                        svScalar scale = 5.*glyphScale;
                        svVector3  glyph = myData->GetGlyph(i,j);
                        svVector3  dir = myData->GetDir(i,j);
                        end[0] = glyph[0]+scale*dir[0];
                        end[1] = glyph[1]+scale*dir[1];
                        end[2] = glyph[2]+scale*dir[2];
                        legend_vertices[count].pos[0]=glyph[0];
                        legend_vertices[count].pos[1]=glyph[1];
                        legend_vertices[count].pos[2]=glyph[2];
                        legend_vertices[count+1].pos[0]=end[0];
                        legend_vertices[count+1].pos[1]=end[1];
                        legend_vertices[count+1].pos[2]=end[2];
                        count+=2;

                 //       legend_vertices[i].add(end);
                   /*     svVector3 tube_seg_norm[(CYLINDERSLICE+1)*2];
                        svVector3  tube_seg_pos[(CYLINDERSLICE+1)*2];
                        GetCylinder(glyph[i][j], dir[i][j], radius,
                                 scale,  CYLINDERSLICE,
                                  tube_seg_norm, tube_seg_pos);
                        for(int t=0;t<(CYLINDERSLICE+1)*2;t++)
                        {
                               tube_vertices[i].add(tube_seg_pos[t]);
                               tube_normals[i].add(tube_seg_norm[t]);
                        }
                   */

                }
   }
}

void svSplitArrow::RenderLegend()
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

void svSplitArrow::GenerateSpring()
{
  display_mode = SV_DISPLAYLIST;
  svGlyph::STILL_UPDATE = false;

	if(glIsList(display_list))
		glDeleteLists(display_list, 1); 
    glNewList(display_list, GL_COMPILE);
    
    for(int i =0;i<seed_num;i++)
    {		
		for(int j=0;j<myData->GetSeed(i);j++)
		{
			if(visibleLabel[i][j])
			{
   			   glColor4f(glyphColors[i][j][0],
                                     glyphColors[i][j][1],
                                     glyphColors[i][j][2],alpha);
			   double radius = glyphRadius;
                           svVector3  glyph = myData->GetGlyph(i,j);
                           svVector3  dir = myData->GetDir(i,j);
			   svVector3 end1, end2;
                           svScalar scale1 = myData->GetCoe(i,j)*glyphScale;
			   end1[0] = glyph[0]+scale1*dir[0];
			   end1[1] = glyph[1]+scale1*dir[1];
			   end1[2] = glyph[2]+scale1*dir[2];
                           RenderCone(end1, dir, radius, 
                                     radius*3, ARROWSLICE);

                           double angle_x, angle_z;
                           GetRotateAngle(dir, angle_x, angle_z);

                           glPushMatrix();
                           glRotatef(angle_z,0,0,1);
                           glRotatef(angle_x,1,0,0);
                           glScalef(radius, radius, 1);
                           RenderSpring(glyph, 
                                        myData->GetExp(i,j)+scaling, 
                                        scale1,glyphScale/10.);
                           glPopMatrix();
			}
		}	
	}	

	
	glEndList();
}
void svSplitArrow::Generate() // this read specific data format
{
 // svGlyph::STILL_UPDATE = true;

  //numOfCriticalPoints = GetNumberOfCriticalPoints(infile);
  //seed_num = numOfCriticalPoints;

  //cerr <<"num of criticalpoints = " <<  numOfCriticalPoints << endl;
  //eigen_vectors = new svVector3[numOfCriticalPoints*3];
  //critical_point_type = new svUint[numOfCriticalPoints];
  //glyph= new svVector3Array[numOfCriticalPoints]; 

  //readCriticalPointInfo(infile, glyph, 
   //      eigen_values_r, eigen_values_i, eigen_vectors,
    //     critical_point_type, seed_num);

//cerr << "====================================" << endl;
//cerr << "SVCRTICAL POINT GLYPH: NUM OF CRTICAL POINTS ==>  " << seed_num << endl;
//cerr << "====================================" << endl;
  // write to display list
  //cleanDisplayList(SV_DISPLAYLIST);
  //BuildDisplayListFromStore();
  //cleanStreamLine();

 // if(isContour)
 // {
      //ComputeContours();
  
      /*for(int i=0;i<seed_num;i++)
      {
           if(contourLabel[i])
           {
                   SetContourData(i);
           }
      }*/
 // }


  display_mode = SV_DISPLAYLIST;
  //display_list = 1;
  svGlyph::STILL_UPDATE = false;

	if(glIsList(display_list))
		glDeleteLists(display_list, 1); 
    glNewList(display_list, GL_COMPILE);

//   GLfloat mat[4];
//   mat[0] = 0.135; mat[1] = 0.2225; mat[2] = 0.1575; mat[3] = 1.0;
   //glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
//   mat[0] = 0.54; mat[1] = 0.89; mat[2] = 0.63;
//   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
//   mat[0] = 0.2; mat[1] = 0.2; mat[2] = 0.2;mat[3]=0.2;
//   glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
//   glMaterialf(GL_FRONT, GL_SHININESS, 20);
    
    for(int i =0;i<seed_num;i++)
    {		
		for(int j=0;j<myData->GetSeed(i);j++)
		{
			if(myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
			{
			glColor4f(glyphColors[i][j][0],glyphColors[i][j][1],glyphColors[i][j][2],alpha);
			double radius = glyphRadius;

	//-----------------endpoint-----------------------
			svVector3 end1, end2;
		//	svScalar exp = getNumOfIntegerDigits(mag[i][j]);
		//	svScalar coe = mag[i][j]/pow(10.,(double)exp);
                        svScalar scale1 = myData->GetCoe(i,j)*glyphScale;
                        svScalar scale2 = (myData->GetExp(i,j)+expScale)*glyphScale;
                        if(fabs(myData->GetExp(i,j))<1e-16)scale2 = 0;
                        svVector3  glyph = myData->GetGlyph(i,j);
                        svVector3  dir = myData->GetDir(i,j); 
			end1[0] = glyph[0]+scale1*dir[0];
			end1[1] = glyph[1]+scale1*dir[1];
			end1[2] = glyph[2]+scale1*dir[2];
//			end2[0] = glyph[i][j][0]+scale2*dir[i][j][0];
//                      end2[1] = glyph[i][j][1]+scale2*dir[i][j][1];
//                      end2[2] = glyph[i][j][2]+scale2*dir[i][j][2];
			//------------------render -------------------
                        RenderCone(end1, dir, radius, radius*3, ARROWSLICE);
                //        RenderSphere(end2, radius, dir[i][j],SPHERESLICE, SPHERESTACK);
                        svVector3 end;
                        svScalar length = scale1;
        /*                if(scale1 > scale2)
                        {
                            end = end1; 
                            length = scale1;
                        } 
                        else
                        {
                            end = end2;
                            length = scale2;
                        }
*/
                        //RenderCylinderTexture(glyph[i][j], dir[i][j], glyphTubeRadius,
                        //               length, 
                        //               glyphColors[i][j],
                         //              (exp+expScale)/12., 2, 12);
                        RenderCylinder(glyph, dir, glyphTubeRadius,
                                       length, CYLINDERSLICE); 

                        glColor3f(0,0,0);
                        RenderCylinder(glyph, dir, 0.005,
                                       5.*glyphScale, 5);
			}
		}	
	}	

	
	glEndList();
}
void svSplitArrow::GenerateTexture() // this read specific data format
{
 // svGlyph::STILL_UPDATE = true;

  //numOfCriticalPoints = GetNumberOfCriticalPoints(infile);
  //seed_num = numOfCriticalPoints;

  //cerr <<"num of criticalpoints = " <<  numOfCriticalPoints << endl;
  //eigen_vectors = new svVector3[numOfCriticalPoints*3];
  //critical_point_type = new svUint[numOfCriticalPoints];
  //glyph= new svVector3Array[numOfCriticalPoints]; 

  //readCriticalPointInfo(infile, glyph, 
   //      eigen_values_r, eigen_values_i, eigen_vectors,
    //     critical_point_type, seed_num);

//cerr << "====================================" << endl;
//cerr << "SVCRTICAL POINT GLYPH: NUM OF CRTICAL POINTS ==>  " << seed_num << endl;
//cerr << "====================================" << endl;
  // write to display list
  //cleanDisplayList(SV_DISPLAYLIST);
  //BuildDisplayListFromStore();
  //cleanStreamLine();

 // if(isContour)
 // {
      //ComputeContours();
  
      /*for(int i=0;i<seed_num;i++)
      {
           if(contourLabel[i])
           {
                   SetContourData(i);
           }
      }*/
 // }


  display_mode = SV_DISPLAYLIST;
  //display_list = 1;
  svGlyph::STILL_UPDATE = false;

	if(glIsList(display_list))
		glDeleteLists(display_list, 1); 
    glNewList(display_list, GL_COMPILE);

   GLfloat mat[4];
   mat[0] = 0.135; mat[1] = 0.2225; mat[2] = 0.1575; mat[3] = 1.0;
   //glMaterialfv(GL_FRONT, GL_AMBIENT, mat);
   mat[0] = 0.54; mat[1] = 0.89; mat[2] = 0.63;
//   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);
   mat[0] = 0.2; mat[1] = 0.2; mat[2] = 0.2;mat[3]=0.2;
//   glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
//   glMaterialf(GL_FRONT, GL_SHININESS, 20);
    
    for(int i =0;i<seed_num;i++)
    {		
		//cerr<<i<<" "<<glyph[i].size()<<endl;
		for(int j=0;j<myData->GetSeed(i);j++)
		{
			if(myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
			{
			glColor4f(glyphColors[i][j][0],glyphColors[i][j][1],glyphColors[i][j][2],alpha);
			double radius = glyphRadius;

			//-----------------endpoint-----------------------
			svVector3 end1, end2;
			svScalar exp = myData->GetExp(i,j);
                        svScalar coe = myData->GetCoe(i,j);
//			svScalar exp = getNumOfIntegerDigits(mag[i][j]);
//			svScalar coe = mag[i][j]/pow(10.,(double)exp);
                        svScalar scale1 = coe*glyphScale;
//cerr<<mag[i][j]<<" "<<glyphScale<<" "<<exp<<" "<<pow(10.,(double)exp)<<" "<<coe<<" "<<scale1<<" ";
                        svScalar scale2 = (exp+expScale)*glyphScale;
                        if(fabs(exp)<1e-16)scale2 = 0; 
//cerr<<scale2<<endl;
                        svVector3  glyph = myData->GetGlyph(i,j);
                        svVector3  dir = myData->GetDir(i,j);
			end1[0] = glyph[0]+scale1*dir[0];
			end1[1] = glyph[1]+scale1*dir[1];
			end1[2] = glyph[2]+scale1*dir[2];
		//	end2[0] = glyph[i][j][0]+scale2*dir[i][j][0];
                //        end2[1] = glyph[i][j][1]+scale2*dir[i][j][1];
                //        end2[2] = glyph[i][j][2]+scale2*dir[i][j][2];
			//------------------render -------------------
                        RenderCone(end1, dir, radius, radius*3, ARROWSLICE);
                //        RenderSphere(end2, radius, dir[i][j],SPHERESLICE, SPHERESTACK);
                        svVector3 end;
                        svScalar length = scale1;
                       /* if(scale1 > scale2)
                        {
                            end = end1; 
                            length = scale1;
                        } 
                        else
                        {
                            end = end2;
                            length = scale2;
                        }*/
                        RenderCylinderTexture(glyph, dir, glyphTubeRadius,
                                       length, 
                                       glyphColors[i][j],
                                      (exp+expScale)/(numPower-1), REPEAT_TEXTURE, (numPower-1));
                        //RenderCylinder(glyph[i][j], dir[i][j], glyphTubeRadius,
                        //               length, CYLINDERSLICE); 

                        glColor3f(0,0,0);
                        RenderCylinder(glyph, dir, 0.005,
                                       5.*glyphScale, 5);
			}
		}	
	}	

	
	glEndList();
}

void svSplitArrow::RenderSample()
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
}
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

}

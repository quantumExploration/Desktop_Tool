#include "svDirectArrow.h"


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include "svException.h"
#include "svUtil.h"

// should change later - add to renderproperty


using namespace std;

namespace __svl_lib {
svDirectArrow::svDirectArrow(svQDOTData *d):svArrowGlyph(d)//svVectorField* f, int numPlane)
{
  //field = f;
  // only handle internal format
  ENABLE_COLOR=false;
  ENABLE_LINEWIDTH = false;

  display_mode = SV_STORE;
  myData->GetPhysicalDimension(&lbbox, &rbbox);

//  glyphScale = DEFAULT_GLYPH_SCALE;
//  glyphSize= DEFAULT_GLYPH_SIZE;
//  glyphRadius = DEFAULT_GLYPH_RADIUS;
//  display_list = DEFAULT_DISPLAYLIST;
}

void svDirectArrow::SaveToFile(char *fname)
{
    ofstream outfile(fname);

    int count = 0;

    for(int i=0;i<seed_num;i++)
    {
          for(int j=0;j<myData->GetSeed(i);j++)
          {
              count++;
          }
    }

    outfile<<count<<endl;

    for(int i=0;i<seed_num;i++)
    {
         // outfile<<glyph[i].size()<<endl;
          for(int j=0;j<myData->GetSeed(i);j++)
          {
              svVector3 glyph = myData->GetGlyph(i,j);
              svScalar mag = myData->GetMag(i,j);
              svVector3 dir = myData->GetDir(i,j);
              outfile<<2<<endl;
              outfile<<glyph[0]<<" "<<glyph[1]<<" "<<glyph[2]
               <<" "<<glyphColors[i][j][0]<<" "<<glyphColors[i][j][1]<<" "
               <<glyphColors[i][j][2]<<endl;
               svVector3 end;
               end[0] = glyph[0]+mag*glyphScale*dir[0];
               end[1] = glyph[1]+mag*glyphScale*dir[1];
               end[2] = glyph[2]+mag*glyphScale*dir[2];
//cerr<<end[0]<<glyphScale<<" "<<mag[i][j]<<endl;
            outfile<<end[0]<<" "<<end[1]<<"  "<<end[2]
               <<" "<<glyphColors[i][j][0]<<" "<<glyphColors[i][j][1]<<" "
               <<glyphColors[i][j][2]<<endl;
          }
    }

    outfile.close();
}
/*void svDirectArrow::GenerateSample(int frequency) // this read specific data format
{
  display_mode = SV_DISPLAYLIST;
  //display_list = 1;
  svGlyph::STILL_UPDATE = false;

	if(glIsList(display_list))
		glDeleteLists(display_list, 1); 
    glNewList(display_list, GL_COMPILE);

    int f = 1./(float)frequency*200;
    if(frequency>20) f = 1;

    for(int i =0;i<seed_num;i++)
    {		
                int count = 0;
		for(int j=0;j<glyph[i].size();j++)
		{
                        if(visibleLabel[i][j]) //&& count%2==0) 
                        {
                              count ++;
                        }
                        if(count%f==0&&visibleLabel[i][j])
                        {

                       // cerr<<i<<" "<<j<<endl;
			glColor4f(glyphColors[i][j][0],glyphColors[i][j][1],glyphColors[i][j][2],alpha);

			svScalar radius = glyphRadius;
		
			//-----------------endpoint-----------------------
			svVector3 end;
			end[0] = glyph[i][j][0]+mag[i][j]*glyphScale*dir[i][j][0];
			end[1] = glyph[i][j][1]+mag[i][j]*glyphScale*dir[i][j][1];
			end[2] = glyph[i][j][2]+mag[i][j]*glyphScale*dir[i][j][2];
			

                        RenderCone(end, dir[i][j], radius, radius * 3, 3) ; 
                        RenderCylinder(glyph[i][j], dir[i][j], glyphTubeRadius,
                                        mag[i][j]*glyphScale, 2);
                       // count++;
                        } 
                       // if(count>40)break;
             }
       }


	glEndList();
}
*/
void svDirectArrow::GenerateArrows()
{    int count = 0;
    int index = 0;
    for(int i =0;i<seed_num;i++)
    {
                for(int j=0;j<myData->GetSeed(i);j++)
                {
                        double radius = glyphRadius;
                        svVector3 end;
                        svScalar mag = myData->GetMag(i,j);
                        svVector3 glyph = myData->GetGlyph(i,j);
                        svVector3 dir = myData->GetDir(i,j);
                        svScalar scale = mag*glyphScale;
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
                   arrow_base_vertices[start2].color[0]=glyphColors[i][j][0];
                   arrow_base_vertices[start2].color[1]=glyphColors[i][j][1];
                   arrow_base_vertices[start2].color[2]=glyphColors[i][j][2];
                   arrow_base_vertices[start2].color[3]=glyphColors[i][j][3];
                   count++;
                        for(int t=0;t<ARROWSLICE;t++)
                        {
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
}
void svDirectArrow::GenerateTubes()
{
    int count1 = 0;
    int count2 = 0;
    for(int i =0;i<seed_num;i++)
    {
                for(int j=0;j<myData->GetSeed(i);j++)
                {
                        double radius = glyphTubeRadius;
                        svVector3 end;
                        svScalar mag = myData->GetMag(i,j);
                        svVector3 glyph = myData->GetGlyph(i,j);
                        svVector3 dir = myData->GetDir(i,j);
                        svScalar scale = mag*glyphScale;
                        svVector3 tube_seg_norm[(CYLINDERSLICE+1)*2];
                        svVector3  tube_seg_pos[(CYLINDERSLICE+1)*2];
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
                            count1++;

                        }
                }
   }
}
void svDirectArrow::GenerateIndex()
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
}
/*
void svDirectArrow::Generate() // this read specific data format
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
  
 // }

//  glEnable(GL_LIGHTING);
//  glEnable(GL_LIGHT0);
//  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

//  glEnable(GL_DEPTH_TEST);
  display_mode = SV_DISPLAYLIST;
  //display_list = 1;
  svGlyph::STILL_UPDATE = false;

	if(glIsList(display_list))
		glDeleteLists(display_list, 1); 
    glNewList(display_list, GL_COMPILE);
//  glEnable(GL_LIGHTING);
//  glEnable(GL_LIGHT0);
//  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

 // glEnable(GL_DEPTH_TEST);

    for(int i =0;i<seed_num;i++)
    {		
		for(int j=0;j<glyph[i].size();j++)
		{
			if(visibleLabel[i][j])// && sampleLabel[i][j])
			{
                       // cerr<<i<<" "<<j<<endl;
			glColor4f(glyphColors[i][j][0],glyphColors[i][j][1],glyphColors[i][j][2],alpha);

			svScalar radius = glyphRadius;
			//-----------------endpoint-----------------------
			svVector3 end;
			end[0] = glyph[i][j][0]+mag[i][j]*glyphScale*dir[i][j][0];
			end[1] = glyph[i][j][1]+mag[i][j]*glyphScale*dir[i][j][1];
			end[2] = glyph[i][j][2]+mag[i][j]*glyphScale*dir[i][j][2];
			
			//cerr<<mag[i][j]<<" "<<glyphScale<<" "<<dir[i][j][0]<<" "<<end[0]<<" "<<glyph[i][j][0]<<endl;

			//------------------render -------------------
                        RenderCone(end, dir[i][j], radius, radius * 3, ARROWSLICE) ; 
                        RenderCylinder(glyph[i][j], dir[i][j], glyphTubeRadius,
                                        mag[i][j]*glyphScale, CYLINDERSLICE);
                     } 
             }
       }

	glEndList();
}
*/
void svDirectArrow::RenderSample()
{
    for(int i=0;i<myData->GetSampleLabelSize();)//sampleLabel.size();)
    {
           int layer = myData->GetSampleLabel(i);// sampleLabel[i];
           int index = myData->GetSampleLabel(i+1);//sampleLabel[i+1];

           svScalar mag = myData->GetMag(layer, index);
           svVector3 glyph = myData->GetGlyph(layer, index);
           svVector3 dir = myData->GetDir(layer, index);

           svScalar length = mag * glyphScale;
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
/*void svDirectArrow::Render()
{
 
  glDisable(GL_BLEND); 
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LINE_SMOOTH);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//  glEnable(GL_TEXTURE_2D); 
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
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);

  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
}

void svDirectArrow::RenderStore() const
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

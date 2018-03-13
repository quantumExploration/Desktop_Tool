#include <GL/glew.h>

#include "svArrowGlyph.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include "svException.h"
#include "svUtil.h"

using namespace std;

namespace __svl_lib{

svArrowGlyph::svArrowGlyph(svQDOTData *d)//svQDOTData *d):svGlyph(d) //svVectorField *f):svGlyph()
{
  myData = d;
  seed_num = (myData->splitData).size();

  glyphColors = new svVector4Array[seed_num];
  svVector4 white(1,1,1,1);
  for(int i=0;i<seed_num;i++){
     for(int j=0;j<myData->splitData[i].size();j++){
        glyphColors[i].add(white);
     }
  }

  arrow_vertices = new VBOVertex[1];
  arrow_indices = new GLuint[1];
  arrow_base_vertices = new VBOVertex[1];
  arrow_base_indices = new GLuint[1];
  tube_vertices = new VBOVertex[1];
  legend_vertices = new VBOVertex[1];
  tube_indices = new GLuint[1];
  tube_base_vertices = new VBOVertex[1];
  tube_base_indices = new GLuint[1];
  //arrow_indices_size = 0;
}
void svArrowGlyph::Clean()
{
     cleanup();
}
void svArrowGlyph::CleanData()
{
      delete [] arrow_vertices;
      delete [] arrow_base_vertices;
      delete [] legend_vertices;
      delete [] tube_vertices;
      delete [] tube_base_vertices;

     for(int i=0;i<seed_num;i++){
        glyphColors[i].free();
     }
     delete [] glyphColors;
}
void svArrowGlyph::CleanIndex()
{
     delete [] arrow_indices;
     delete [] tube_indices;
     delete [] arrow_base_indices;
     delete [] tube_base_indices;
}
void svArrowGlyph::Update()
{
     UpdateData();
     UpdateIndex();
}
void svArrowGlyph::UpdateData()
{
     //arrow_indices_size = dataSize*3*ARROWSLICE;
     //tube_indices_size = dataSize * 4 * CYLINDERSLICE;

     CleanData();
    seed_num = myData->splitData.size();
    int dataSize = 0;
    for(int i=0;i<seed_num;i++)
        for(int j=0;j<myData->splitData[i].size();j++)//myData->GetSeed(i);j++)
              dataSize++;

     glyphColors = new svVector4Array[seed_num];
     svVector4 black(0,0,0,1);
     for(int i=0;i<seed_num;i++){
       for(int j=0;j<myData->splitData[i].size();j++){
         glyphColors[i].add(black);
       }
     }

     cerr<<"dataSize "<<dataSize<<endl;
     arrow_vertices_size = dataSize*(ARROWSLICE+1);
     tube_vertices_size = dataSize*(CYLINDERSLICE+1)*2;
     arrow_base_vertices_size = dataSize*(ARROWSLICE+1);
     tube_base_vertices_size = dataSize*(CYLINDERSLICE+1+1);

     arrow_vertices = new VBOVertex[arrow_vertices_size];//svVector3Array[seed_num];
     tube_vertices = new VBOVertex[tube_vertices_size];//svVector3Array[seed_num];
     arrow_base_vertices = new VBOVertex[arrow_base_vertices_size];//svVector3Array[seed_num];
     tube_base_vertices = new VBOVertex[tube_base_vertices_size];//svVector3Array[seed_num];
     legend_vertices = new VBOVertex[dataSize*2];//svVector3Array[seed_num];
     //arrow_indices = new GLuint[dataSize*3*ARROWSLICE];//svIntArray[seed_num];
     //tube_indices = new GLuint[dataSize*4*CYLINDERSLICE];//svIntArray[seed_num];
}

void svArrowGlyph::UpdateIndex()
{
     CleanIndex();

     int count = 0;
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<myData->splitData[i].size();j++)//->GetSeed(i);j++)
          {
              if((*myData->qdotVisible).at(myData->splitData[i][j])
             && (*myData->sampleVisible).at(myData->splitData[i][j]))//j%sample==0)
                    count++;
          }
     }

     //cerr<<count<<endl;
     arrow_indices_size = count*3*ARROWSLICE;
     tube_indices_size = count*4*CYLINDERSLICE;
     arrow_base_indices_size = count*3*ARROWSLICE;
     tube_base_indices_size = count*3*CYLINDERSLICE;
     arrow_indices = new GLuint[arrow_indices_size];
     tube_indices = new GLuint[tube_indices_size];
     arrow_base_indices = new GLuint[arrow_base_indices_size];
     tube_base_indices = new GLuint[tube_base_indices_size];
}
/*
void svArrowGlyph::UpdateIndex(int region)
{
     CleanIndex();

     int count = 0;
     for(int i=0;i<seed_num;i++)
     {
          for(int j=0;j<myData->GetSeed(i);j++)
          {
                   if(myData->GetVisibleLabel(i,j) && myData->GetRegion(i,j)==region)
                          count++;
          }
     }
     cerr<<"updateindex "<<count<<endl;
     arrow_indices_size = count*3*ARROWSLICE;
cerr<<arrow_indices_size<<endl;
     tube_indices_size = count*4*CYLINDERSLICE;
     arrow_base_indices_size = count*3*ARROWSLICE;
     tube_base_indices_size = count*3*CYLINDERSLICE;
     arrow_indices = new GLuint[arrow_indices_size];
     tube_indices = new GLuint[tube_indices_size];
     arrow_base_indices = new GLuint[arrow_base_indices_size];
     tube_base_indices = new GLuint[tube_base_indices_size];
}
*/
void svArrowGlyph::UpdateIndexVBO()
{
   if(glIsBuffer(ARROW_IVBO))
           glDeleteBuffers(1, &ARROW_IVBO);
   glGenBuffers(1, &ARROW_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*arrow_indices_size,
                arrow_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

   if(glIsBuffer(ARROW_BASE_IVBO))
           glDeleteBuffers(1, &ARROW_BASE_IVBO);
   glGenBuffers(1, &ARROW_BASE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_BASE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*arrow_base_indices_size,
                arrow_base_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);


   if(glIsBuffer(TUBE_IVBO))
           glDeleteBuffers(1, &TUBE_IVBO);
   glGenBuffers(1, &TUBE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*tube_indices_size,
                tube_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

   if(glIsBuffer(TUBE_BASE_IVBO))
           glDeleteBuffers(1, &TUBE_BASE_IVBO);
   glGenBuffers(1, &TUBE_BASE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_BASE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*tube_base_indices_size,
                tube_base_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

}

void svArrowGlyph::UpdateArrowVBO()
{
cerr<<"update arrow vbo"<<endl;
   if(glIsBuffer(ARROW_VBO))
           glDeleteBuffers(1, &ARROW_VBO);
   glGenBuffers(1, &ARROW_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, ARROW_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*arrow_vertices_size,
                arrow_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);
cerr<<"arrow vbo"<<endl;
   if(glIsBuffer(ARROW_BASE_VBO))
           glDeleteBuffers(1, &ARROW_BASE_VBO);
   glGenBuffers(1, &ARROW_BASE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, ARROW_BASE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*arrow_base_vertices_size,
                arrow_base_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);
}

void svArrowGlyph::UpdateTubeVBO()
{
   if(glIsBuffer(TUBE_VBO))
          glDeleteBuffers(1,&TUBE_VBO);
   glGenBuffers(1, &TUBE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, TUBE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*tube_vertices_size,
                tube_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   if(glIsBuffer(TUBE_BASE_VBO))
        glDeleteBuffers(1,&TUBE_BASE_VBO);
   glGenBuffers(1, &TUBE_BASE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, TUBE_BASE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*tube_base_vertices_size,
                tube_base_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);
}

void svArrowGlyph::GenerateVBO()
{
//   arrow_indices_size = dataSize*3*ARROWSLICE;
//   int num =dataSize*(ARROWSLICE+1);

   glGenBuffers(1, &ARROW_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, ARROW_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*arrow_vertices_size,
                arrow_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   glGenBuffers(1, &ARROW_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*arrow_indices_size,
                arrow_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

   glGenBuffers(1, &ARROW_BASE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, ARROW_BASE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*arrow_base_vertices_size,
                arrow_base_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   glGenBuffers(1, &ARROW_BASE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_BASE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*arrow_base_indices_size,
                arrow_base_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

   glGenBuffers(1, &TUBE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, TUBE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*tube_vertices_size,
                tube_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   glGenBuffers(1, &TUBE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*tube_indices_size,
                tube_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

   glGenBuffers(1, &TUBE_BASE_VBO);
   glBindBuffer(GL_ARRAY_BUFFER, TUBE_BASE_VBO);
   glBufferData(GL_ARRAY_BUFFER, sizeof(VBOVertex)*tube_base_vertices_size,
                tube_base_vertices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ARRAY_BUFFER,0);

   glGenBuffers(1, &TUBE_BASE_IVBO);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_BASE_IVBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*tube_base_indices_size,
                tube_base_indices,GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}
void svArrowGlyph::RenderVBO()
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

//   glBindBuffer(GL_ARRAY_BUFFER, 0);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//   glDisableClientState(GL_VERTEX_ARRAY);
//   glDisableClientState(GL_NORMAL_ARRAY);
//   glDisableClientState(GL_COLOR_ARRAY);

   glBindBuffer(GL_ARRAY_BUFFER, ARROW_BASE_VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ARROW_BASE_IVBO);

   glVertexPointer(3, GL_FLOAT, sizeof(VBOVertex), (char*)NULL+0);
   glNormalPointer(GL_FLOAT, sizeof(VBOVertex), (char*)NULL+sizeof(float)*3);
   glColorPointer(4, GL_FLOAT, sizeof(VBOVertex),(char*)NULL+sizeof(float)*6);

   glDrawElements(GL_TRIANGLES, arrow_base_indices_size, GL_UNSIGNED_INT,NULL);

//========================================

   glBindBuffer(GL_ARRAY_BUFFER, TUBE_VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_IVBO);

   glVertexPointer(3, GL_FLOAT, sizeof(VBOVertex), (char*)NULL+0);
   glNormalPointer(GL_FLOAT, sizeof(VBOVertex), (char*)NULL+sizeof(float)*3);
   glColorPointer(4, GL_FLOAT, sizeof(VBOVertex),(char*)NULL+sizeof(float)*6);

   glDrawElements(GL_QUADS, tube_indices_size, GL_UNSIGNED_INT,NULL);

   glBindBuffer(GL_ARRAY_BUFFER, TUBE_BASE_VBO);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TUBE_BASE_IVBO);

   glVertexPointer(3, GL_FLOAT, sizeof(VBOVertex), (char*)NULL+0);
   glNormalPointer(GL_FLOAT, sizeof(VBOVertex), (char*)NULL+sizeof(float)*3);
   glColorPointer(4, GL_FLOAT, sizeof(VBOVertex),(char*)NULL+sizeof(float)*6);

   glDrawElements(GL_TRIANGLES, tube_base_indices_size, GL_UNSIGNED_INT,NULL);

//===============================

   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

   glDisableClientState(GL_VERTEX_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);

}

void svArrowGlyph::Generate()
{

}

void svArrowGlyph::Render()
{

}
/*
svScalar svArrowGlyph::Mouse(int x, int y, GLfloat *tm, svScalar currentDistance)
{
cerr<<"start selection"<<endl;
   int count = 0;
   svScalar minDistance = 9e+9;
   for(int i=0;i<seed_num;i++)
   {
       for(int j=0;j<myData->GetSeed(i);j++)
       {
            if(myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
            {
                  svVector3 newGlyph = getTransform(myData->GetGlyph(i,j), tm);
                  svVector3 arrow;
                  arrow[0] = arrow_vertices[count*(ARROWSLICE+1)].pos[0];
                  arrow[1] = arrow_vertices[count*(ARROWSLICE+1)].pos[1];
                  arrow[2] = arrow_vertices[count*(ARROWSLICE+1)].pos[2];
                  svVector3 newArrow = getTransform(arrow, tm);
                  svVector3 projectGlyph = getProject(newGlyph);
                  svVector3 projectArrow = getProject(newArrow);
                  svVector3 projectV = svGetPerpendicularVector2D(projectArrow-projectGlyph);
                  svVector3 newV = getUnProject(projectV);
                  svVector3 newP1 = newGlyph + newV * glyphRadius;

                  count++;
            }
       }
   }
cerr<<"end"<<endl;
   return minDistance;
}

void svArrowGlyph::Mouse(int x, int y, svVector3 eye, float near, GLfloat *tm, svScalar scale, int &layer, int &data)
{
cerr<<"========selection==========="<<endl;
    svVector3 rayPos;
    svVector3 rayDir;

    getRay(x, y, eye, rayDir);
    rayPos = eye;
    svVector3 newRayPos;
    svVector3 newRayDir;

    getNewRay(rayPos, rayDir, tm, newRayPos, newRayDir);

    vector<svVector3> Arrow;
    svVector3 newLb = lbbox;// * scale;
    svVector3 newRb = rbbox;// * scale;
    svVector3 *intersectionP = new svVector3[2];
//    getLineBox(newRayPos, newRayDir, newLb, newRb, intersectionP);
//  cerr<<intersectionP[0][0]<<" "<<intersectionP[0][1]<<" "<<intersectionP[0][2]<<endl;
//  cerr<<intersectionP[1][0]<<" "<<intersectionP[1][1]<<" "<<intersectionP[1][2]<<endl;
    vector<int> layerIndex;
    vector<int> dataIndex;
    vector<svScalar> distance;
    int count =0;
    for(int i=0;i<seed_num;i++)
    {
        for(int j=0;j<myData->GetSeed(i);j++)
        {//cerr<<i<<" "<<j<<endl;
           bool inside = false;
           if(myData->GetVisibleLabel(i,j))
           {
               svVector3 arrow;
               arrow[0] = arrow_vertices[count*(ARROWSLICE+1)].pos[0];
               arrow[1] = arrow_vertices[count*(ARROWSLICE+1)].pos[1];
               arrow[2] = arrow_vertices[count*(ARROWSLICE+1)].pos[2];
              //if(getInsideBox(glyph[i][j]*scale, intersectionP[0], intersectionP[1]))
              if(getInsideCylinder(newRayPos, newRayDir,
                  myData->GetGlyph(i,j), arrow, glyphRadius))
              {
                  svVector3 p = myData->GetGlyph(i,j);// * scale;
                  svVector3 e = arrow;// * scale;
                  svVector3 v = svGetPerpendicularVector(myData->GetDir(i,j));

//                  svVector3 newP = getProject(p);
//                  svVector3 tmpDir = svGetPerpendicularVector2D(newP);
//                  svVector3 v = getUnProject(tmpDir);

                  inside = rayBoxIntersect(newRayPos, newRayDir, p, e, myData->GetDir(i,j), v, glyphRadius);

//                  svVector3 *pp = new svVector3[4];
//                  pp[0] = p - v * glyphRadius;
//                  pp[1] = p + v * glyphRadius;
//                  pp[2] = e + v * glyphRadius;
//                  pp[3] = e - v * glyphRadius;

//                  svScalar tmpD;
//                  svVector3 tmpP;
//                  inside = rayPlaneIntersect(newRayPos, newRayDir, eye, near, dir[i][j], pp, tmpD, tmpP);
                  if(inside)
                  {
                     Arrow.push_back(arrow);
                     layerIndex.push_back(i);
                     dataIndex.push_back(j);
//                     distance.push_back(tmpD);
                  }

//                  delete [] pp;
              }

           }
           count++;
        }
    }
    int minIndex = -1;
    svScalar minD = 9e+9;

    for(int i=0;i<layerIndex.size();i++)
    {
         int ii = layerIndex[i];
         int jj = dataIndex[i];
         svVector3 p = myData->GetGlyph(ii,jj);// * scale;
         svVector3 e = Arrow[i];// * scale;
         svVector3 v = svGetPerpendicularVector(myData->GetDir(ii,jj));

         svScalar d;
         rayBoxIntersectD(newRayPos, newRayDir, p, e, myData->GetDir(ii,jj),
                                  v, glyphRadius, d);

       if(d < minD)
       {
          minD = d;
          minIndex = i;
       }
    }

    if(minIndex>=0)
    {
         layer = layerIndex[minIndex];
         data = dataIndex[minIndex];
    }
    else
    {
         layer = -1;
         data = -1;
    }
//cerr<<minD<<endl;
//if(minIndex>=0)cerr<<layerIndex[minIndex]<<" "<<dataIndex[minIndex]<<endl;
//=========clean up=============
    layerIndex.clear();
    dataIndex.clear();
    distance.clear();
    delete [] intersectionP;
}
*/
/*
void svArrowGlyph::SaveArrowtoOSG(char *file){
        //int size = glyphPos[0].size();
cerr<<file<<endl;        ofstream outfile(file);

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
              outfile<<"                              "<<arrow_indices[i]<<" "<<arrow_indices[i+1]<<" "<<arrow_indices[i+2]<<endl;
        }
        outfile<<"                      }"<<endl;
        outfile<<"              }"<<endl;


        outfile<<"              VertexArray Vec3Array "<<arrow_vertices_size<<endl;
        outfile<<"              {"<<endl;
        for(int i=0;i<arrow_vertices_size;i++)
        {
                outfile<<"                      "
                        <<arrow_vertices[i].pos[0]
                        <<" "<<arrow_vertices[i].pos[1]
                        <<" "<<arrow_vertices[i].pos[2]<<endl;

        }
        outfile<<"              }"<<endl;


        //------------Normal Vector--------------------
        outfile<<"              NormalBinding PER_VERTEX"<<endl;
        outfile<<"              NormalArray Vec3Array "<<arrow_vertices_size<<endl;
        outfile<<"              {"<<endl;
        for(int i=0;i<arrow_vertices_size;i++)
        {
                outfile<<"                      "
                        <<arrow_vertices[i].norm[0]
                        <<" "<<arrow_vertices[i].norm[1]
                        <<" "<<arrow_vertices[i].norm[2]<<endl;

        }
        outfile<<"              }"<<endl;

        //-------------------Color array---------------------------
        outfile<<"              ColorBinding PER_VERTEX"<<endl;
        outfile<<"              ColorArray Vec4Array "<<arrow_vertices_size<<endl;
        outfile<<"              {"<<endl;
        for(int i=0;i<arrow_vertices_size;i++)
        {
                outfile<<"                      "
                        <<arrow_vertices[i].color[0]
                        <<" "<<arrow_vertices[i].color[1]
                        <<" "<<arrow_vertices[i].color[2]
                        <<" "<<arrow_vertices[i].color[3]<<endl;

        }
        outfile<<"              }"<<endl;
        outfile<<"      }"<<endl;
        outfile<<"}"<<endl;
        outfile.close();
}

void svArrowGlyph::SaveTubetoOSG(char *file){
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
              outfile<<"                              "<<tube_indices[i]<<" "<<tube_indices[i+1]<<" "<<tube_indices[i+2]<<" "<<tube_indices[i+3]<<endl;
        }
        outfile<<"                      }"<<endl;
        outfile<<"              }"<<endl;


        outfile<<"              VertexArray Vec3Array "<<tube_vertices_size<<endl;
        outfile<<"              {"<<endl;
cerr<<tube_vertices_size<<endl;
        for(int i=0;i<tube_vertices_size;i++)
        {
                outfile<<"                      "
                        <<tube_vertices[i].pos[0]
                        <<" "<<tube_vertices[i].pos[1]
                        <<" "<<tube_vertices[i].pos[2]<<endl;

        }
        outfile<<"              }"<<endl;


        //------------Normal Vector--------------------
        outfile<<"              NormalBinding PER_VERTEX"<<endl;
        outfile<<"              NormalArray Vec3Array "<<tube_vertices_size<<endl;
        outfile<<"              {"<<endl;
        for(int i=0;i<tube_vertices_size;i++)
        {
                outfile<<"                      "
                        <<-tube_vertices[i].norm[0]
                        <<" "<<-tube_vertices[i].norm[1]
                        <<" "<<-tube_vertices[i].norm[2]<<endl;

        }
        outfile<<"              }"<<endl;

        //-------------------Color array---------------------------
        outfile<<"              ColorBinding PER_VERTEX"<<endl;
        outfile<<"              ColorArray Vec4Array "<<tube_vertices_size<<endl;
        outfile<<"              {"<<endl;
        for(int i=0;i<tube_vertices_size;i++)
        {
                outfile<<"                      "
                        <<tube_vertices[i].color[0]
                        <<" "<<tube_vertices[i].color[1]
                        <<" "<<tube_vertices[i].color[2]
                        <<" "<<tube_vertices[i].color[3]<<endl;

        }
        outfile<<"              }"<<endl;
        outfile<<"      }"<<endl;
        outfile<<"}"<<endl;
        outfile.close();
}
*/
void svArrowGlyph::SaveArrowtoOSG(char *file){
        //int size = glyphPos[0].size();
//cerr<<file<<endl;
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
	int count2 = 0;
    	for(int i =0;i<seed_num;i++){
                for(int j=0;j<myData->splitData[i].size();j++){
                        bool visible = ((*myData->qdotVisible).at(myData->splitData[i][j])
                                  &&(*myData->sampleVisible).at(myData->splitData[i][j]));
			if(visible){
                        	int start1 = count2;
                        	count2++;
                        	for(int t=0;t<ARROWSLICE;t++){
				  	outfile<<"                              "<<
				         	start1<<" ";
                              		if(t==ARROWSLICE-1){
						outfile<<count2<<" "<<count2-(ARROWSLICE-1)<<endl;
                              		}
                              		else{
                                		outfile<<count2<<" "<<count2+1<<endl;
                              		}
					count2++;
                             	}
                        }
                }
   	}
	cout<<arrow_indices_size<<" "<<count2<<endl;
        outfile<<"                      }"<<endl;
        outfile<<"              }"<<endl;


        int size = arrow_indices_size*(ARROWSLICE+1);
        outfile<<"              VertexArray Vec3Array "<<size<<endl;
        outfile<<"              {"<<endl;
	count2 =0;
    	for(int i =0;i<seed_num;i++){
                for(int j=0;j<myData->splitData[i].size();j++){
                        bool visible = ((*myData->qdotVisible).at(myData->splitData[i][j])
                                  &&(*myData->sampleVisible).at(myData->splitData[i][j]));
                        int start1 = count2;
                        count2++;
			if(visible) {
				outfile<<"                      "
					<<arrow_vertices[start1].pos[0]<<" "
					<<arrow_vertices[start1].pos[1]<<" "
					<<arrow_vertices[start1].pos[2]<<endl;
			}
                        for(int t=0;t<ARROWSLICE;t++){
                            if(visible) {
				outfile<<"                      "
					<<arrow_vertices[count2].pos[0]<<" "
					<<arrow_vertices[count2].pos[1]<<" "
					<<arrow_vertices[count2].pos[2]<<endl;
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
	count2 =0;
    	for(int i =0;i<seed_num;i++){
                for(int j=0;j<myData->splitData[i].size();j++){
                        bool visible = ((*myData->qdotVisible).at(myData->splitData[i][j])
                                  &&(*myData->sampleVisible).at(myData->splitData[i][j]));
                        int start1 = count2;
                        count2++;
			if(visible) {
				outfile<<"                      "
					<<arrow_vertices[start1].norm[0]<<" "
					<<arrow_vertices[start1].norm[1]<<" "
					<<arrow_vertices[start1].norm[2]<<endl;
			}
                        for(int t=0;t<ARROWSLICE;t++){
                            if(visible) {
				outfile<<"                      "
					<<arrow_vertices[count2].norm[0]<<" "
					<<arrow_vertices[count2].norm[1]<<" "
					<<arrow_vertices[count2].norm[2]<<endl;
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
	count2 =0;
    	for(int i =0;i<seed_num;i++){
                for(int j=0;j<myData->splitData[i].size();j++){
                        bool visible = ((*myData->qdotVisible).at(myData->splitData[i][j])
                                  &&(*myData->sampleVisible).at(myData->splitData[i][j]));
                        int start1 = count2;
                        count2++;
			if(visible) {
				outfile<<"                      "
					<<arrow_vertices[start1].color[0]<<" "
					<<arrow_vertices[start1].color[1]<<" "
					<<arrow_vertices[start1].color[2]<<" "
					<<arrow_vertices[start1].color[3]<<endl;
			}
                        for(int t=0;t<ARROWSLICE;t++){
                            if(visible) {
				outfile<<"                      "
					<<arrow_vertices[count2].color[0]<<" "
					<<arrow_vertices[count2].color[1]<<" "
					<<arrow_vertices[count2].color[2]<<" "
					<<arrow_vertices[count2].color[3]<<endl;
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

void svArrowGlyph::SaveTubetoOSG(char *file){
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
	int count1=0;
    	for(int i =0;i<seed_num;i++){
                for(int j=0;j<myData->splitData[i].size();j++){
                        bool visible = ((*myData->qdotVisible).at(myData->splitData[i][j])
                                  &&(*myData->sampleVisible).at(myData->splitData[i][j]));

			if(visible){
                        	for(int t=0;t<(CYLINDERSLICE+1)*2;t++){
                            		if(t%2==0&&t<CYLINDERSLICE*2){
                                 		outfile<<"                              "<<
						count1<<" "<<count1+1<<" "<<count1+3
						<<" "<<count1+2<<endl;
                                 	}
					count1++;
                             	}
			}
                 }
   	}
        outfile<<"                      }"<<endl;
        outfile<<"              }"<<endl;


	int size = tube_indices_size*(CYLINDERSLICE+1)*2;
        outfile<<"              VertexArray Vec3Array "<<size<<endl;
        outfile<<"              {"<<endl;
	count1=0;
   	for(int i =0;i<seed_num;i++){
                for(int j=0;j<myData->splitData[i].size();j++){
                        bool visible = ((*myData->qdotVisible).at(myData->splitData[i][j])
                                  &&(*myData->sampleVisible).at(myData->splitData[i][j]));

                        for(int t=0;t<(CYLINDERSLICE+1)*2;t++){
                                 if(visible)//myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
                                 {
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
        outfile<<"              NormalBinding PER_VERTEX"<<endl;
        outfile<<"              NormalArray Vec3Array "<<size<<endl;
        outfile<<"              {"<<endl;
	count1=0;
   	for(int i =0;i<seed_num;i++){
                for(int j=0;j<myData->splitData[i].size();j++){
                        bool visible = ((*myData->qdotVisible).at(myData->splitData[i][j])
                                  &&(*myData->sampleVisible).at(myData->splitData[i][j]));

                        for(int t=0;t<(CYLINDERSLICE+1)*2;t++){
                                 if(visible)//myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
                                 {
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
        outfile<<"              ColorBinding PER_VERTEX"<<endl;
        outfile<<"              ColorArray Vec4Array "<<size<<endl;
        outfile<<"              {"<<endl;
	count1=0;
   	for(int i =0;i<seed_num;i++){
                for(int j=0;j<myData->splitData[i].size();j++){
                        bool visible = ((*myData->qdotVisible).at(myData->splitData[i][j])
                                  &&(*myData->sampleVisible).at(myData->splitData[i][j]));

                        for(int t=0;t<(CYLINDERSLICE+1)*2;t++){
                                 if(visible)//myData->GetVisibleLabel(i,j))//visibleLabel[i][j])
                                 {
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
void svArrowGlyph::cleanup()
{
   if(glIsBuffer(ARROW_VBO))
   {
     glDeleteBuffers(1, &ARROW_VBO);
    glDeleteBuffers(1, &ARROW_IVBO);
    glDeleteBuffers(1, &TUBE_VBO);
    glDeleteBuffers(1, &TUBE_IVBO);
    glDeleteBuffers(1, &ARROW_BASE_VBO);
    glDeleteBuffers(1, &ARROW_BASE_IVBO);
    glDeleteBuffers(1, &TUBE_BASE_VBO);
    glDeleteBuffers(1, &TUBE_BASE_IVBO);
   }
    delete [] arrow_vertices;
      delete [] arrow_base_vertices;
      delete [] arrow_indices;
      delete [] arrow_base_indices;
      delete [] legend_vertices;
    delete [] tube_vertices;
      delete [] tube_base_vertices;
      delete [] tube_indices;
      delete [] tube_base_indices;

}

}

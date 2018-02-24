
/* rendering routine of a vector field using glyphs */

#include "svMesh.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include "svException.h"
#include "svUtil.h"
#include "svColors.h"

using namespace std;

namespace __svl_lib {
svMesh::svMesh(svMeshData *d){
  //nComponent = 0;
  New(d);
}

void svMesh::New(svMeshData *d){
  //cleanup();

  myData = d;
//  vector<int> region = myData->GetUniqueRegion();
  //nComponent = myData->myQDOT->regionSize;//region.size()-1;

  glyph_list = 15;
  display_list=10;
/*  mesh=new svVector3Array[nComponent];
  dir = new svVector3Array[nComponent];
  //colors = new svVector4Array[nComponent];
  glyph = new svVector3Array[nComponent];
  glyphDir = new svVector3Array[nComponent];
  dirDiff = new svScalarArray[nComponent];
  magDiff = new svScalarArray[nComponent];
*/
  maxMag = 0;
  quadSize = 0.4; 
  coneRadius = 0.1;
  coneLength = quadSize/2.;

  for(int i=0;i<myData->glyphs.size();i++){
      for(int j=0;j<myData->glyphs[i].size();j++){
             if(myData->glyphs[i][j].denDiff > maxMag)
                maxMag = myData->glyphs[i][j].denDiff;
      }
  } 
}
void svMesh::GenerateGlyph(ViewProperty &property){
 glLineWidth(4.);
 if(glIsList(glyph_list))
     glDeleteLists(glyph_list, 1);
 glNewList(glyph_list, GL_COMPILE);
 for(int i=0;i<myData->glyphs.size();i++){
  for(int j=0;j<myData->glyphs[i].size();j++){
    svVector3 v = svGetPerpendicularVector(-myData->glyphs[i][j].dir);
    svScalar radius = sqrt(myData->glyphs[i][j].denDiff/maxMag * quadSize * quadSize);
    svVector3 head = myData->glyphs[i][j].pos 
                    + v * radius-myData->glyphs[i][j].dir*radius/5.;
    glColor3f(251./255.,184./255.,103./255.);//91./255.,168./255.,126./255.);
    glBegin(GL_QUADS);
    svVector3 glyphDir = -myData->glyphs[i][j].dir;
    svVector3 glyph = myData->glyphs[i][j].pos;
    glNormal3f(glyphDir[0], glyphDir[1], glyphDir[2]);
    for(int t=0;t<4;t++){
      svVector3 p = svGetRotatePoint(head, glyph,
                          glyphDir, 90. * (float)t);
      glVertex3f(p[0],p[1],p[2]);
    }
    glEnd();
    glColor3f(76./255.,28./255.,95./255.);
    radius = tan(myData->glyphs[i][j].dirDiff/2.) * coneLength;
    svVector3 p = glyph + glyphDir * coneLength;
    int segment;
    float degree = myData->glyphs[i][j].dirDiff/PI * 180.;
    segment = degree/20;
    float sdegree;
    if(segment == 0){
      segment = 2;
    }
    sdegree = degree/(float)segment;
    RenderFan(property, glyph, glyphDir, radius, coneLength, segment, sdegree);
  }
 }
 glEndList();
 glLineWidth(1.);
}

void svMesh::GenerateSurface(){
    //cerr<<display_list<<endl;
    if(glIsList(display_list))
           glDeleteLists(display_list, 1);
    glNewList(display_list, GL_COMPILE);
    for(int i=0;i<myData->facets.size();i++){
       for(int j=0;j<myData->facets[i].size();j++){
           glColor3f(166./255., 206./255., 227./255.);
           glBegin(GL_TRIANGLES);
           glNormal3f(-myData->facets[i][j].norm[0],
                      -myData->facets[i][j].norm[1],
                      -myData->facets[i][j].norm[2]);
           for(int v=0;v<3;v++){
             glVertex3f(myData->facets[i][j].pos[v][0],
                        myData->facets[i][j].pos[v][1],
                        myData->facets[i][j].pos[v][2]);
           }
           glEnd(); 
       }
    }
    glEndList();
}

void svMesh::Render()
{
    glCallList(glyph_list);
    glCallList(display_list);
}

}



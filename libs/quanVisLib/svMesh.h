
/* rendering routine of a vector field using glyphs */

#ifndef __SV_MESH_H
#define __SV_MESH_H


#include "svQDOTData.h"

namespace __svl_lib {

class svMesh {
  public:
    //svMesh(){display_list=10;
    //        mesh=new svVector3Array[1];dir = new svVector3Array[1];
    //         colors = new svVector4Array[1];}
    svMesh(svMeshData *d);

    virtual void New(svMeshData *d);
    //virtual void New(vector<int> region);

    //virtual void SetData(char *file, int index);	
    virtual ~svMesh(){};

    virtual void GenerateGlyph(ViewProperty &property);
    virtual void GenerateSurface();

    virtual void Render();

    
     void SetDisplayList(svInt list){display_list = list;}
  protected:
     void cleanup();

  private:
     //GLuint MESH_BUFFER;
     svMeshData *myData;
     //svVector3Array *glyph;
     //svVector3Array *glyphDir;
     //svScalarArray *dirDiff;
     //svScalarArray *magDiff;

    // int nComponent;//number of components
 
    // vector<float *> meshV;
    // vector<float *> meshN;
    // vector<float *> meshC;

     int glyph_list;
     svScalar quadSize;
     svScalar coneLength;
     svScalar maxMag;
     svScalar coneRadius;
     //vector<GLuint> meshVBO;
     //vector<GLuint> meshIVBO;

     svInt display_list;
     //svVector3Array *mesh;
     //svVector3Array *dir;
     //svVector4Array *colors;
     //vector<int> meshregion;

};

}


#endif // __SV_MESH_H

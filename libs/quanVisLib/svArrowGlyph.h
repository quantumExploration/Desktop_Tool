#ifndef __SV_ARROW_GLYPH_H
#define __SV_ARROW_GLYPH_H

#include "svArray.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
//#include "svGlyph.h"
#include "svQDOTData.h"
#include "svColors.h"

#ifndef ARROWSLICE 
#define ARROWSLICE 4
#endif

#ifndef CYLINDERSLICE
#define CYLINDERSLICE 6
#endif


#ifndef PI
#define PI 3.1415926
#endif

namespace __svl_lib {

class svArrowGlyph{// : public svGlyph{
public:
   svArrowGlyph(svQDOTData* d);//svQDOTData *d);

   virtual ~svArrowGlyph(){cleanup();}
//TODO: a new function to load a new data set
   virtual void Clean();
   virtual void CleanData();
   virtual void CleanIndex(); 
   virtual void Update(); 
   virtual void UpdateData();
   virtual void UpdateIndex();
   //virtual void UpdateIndex(int region);

   void SetTubeRadius(svScalar r){glyphTubeRadius = r;}
   svScalar GetTubeRadius(){return glyphTubeRadius;}

   virtual void UpdateIndexVBO();
   virtual void UpdateArrowVBO();
   virtual void UpdateTubeVBO();

   virtual void GenerateVBO();
   virtual void RenderVBO();
 
   virtual void Generate();
   virtual void Render();

   virtual void SetRadius(svScalar r){glyphRadius = r;}
   virtual void SetColors(){
            svColors *color=new svColors();
            color->GetColors(myData,glyphColors);}
   virtual void SetScale(svScalar s ){glyphScale = s;}

   virtual void SaveTubetoOSG(char *file);
   virtual void SaveArrowtoOSG(char *file);
/*
   virtual svScalar Mouse(int x, int y, GLfloat *tm, svScalar currentDistance);
   virtual void Mouse(int x, int y, svVector3 eye, float near, GLfloat *tm, svScalar scale, int &layer, int &data);
*/
protected: 
   void cleanup();

   svQDOTData * myData;
   svScalar glyphTubeRadius;

   svScalar glyphRadius;
   svVector4Array *glyphColors;
   svScalar glyphScale;

   GLuint ARROW_VBO;
   GLuint ARROW_IVBO;
   GLuint TUBE_VBO;
   GLuint TUBE_IVBO;
   GLuint ARROW_BASE_VBO;
   GLuint ARROW_BASE_IVBO;
   GLuint TUBE_BASE_VBO;  
   GLuint TUBE_BASE_IVBO; 

   VBOVertex *arrow_vertices;
   VBOVertex *arrow_base_vertices;
   int arrow_indices_size;
   int tube_indices_size;
   int arrow_vertices_size;
   int tube_vertices_size;
   int arrow_base_indices_size;
   int arrow_base_vertices_size;
   int tube_base_indices_size; 
   int tube_base_vertices_size;
   VBOVertex *tube_vertices;
   VBOVertex *tube_base_vertices;
   VBOVertex *legend_vertices;
   GLuint *arrow_indices;
   GLuint *tube_indices;
   GLuint *arrow_base_indices;
   GLuint *tube_base_indices;
 
   int seed_num;
};
}

#endif


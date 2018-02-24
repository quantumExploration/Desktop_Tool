#ifndef __SV_CYLINDER_GLYPH_H
#define __SV_CYLINDER_GLYPH_H

#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
#include "svGlyph.h"

#ifndef PI
#define PI 3.1415926
#endif


#ifndef CYLINDERSLICE 
#define CYLINDERSLICE 10
#endif 

namespace __svl_lib {

typedef struct CylinderLegendProperty{
  svInt vSegment;
  svInt hSegment;
  svVector4 color;
  svVector3 vPos;//vertical
  svVector3 vDir;
  svVector3 hPos;
  svVector3 hDir;
  svScalar vUnit;
  svScalar hUnit;
} CylinderLegendProperty;

typedef struct CylinderProperty{
  svInt SEGMENT_NUM;
  svScalar radius;
  svVector4 legendcolor;
  svScalar legend;
  svScalar scaley;
} CylinderProperty;

class svCylinderGlyph : public svGlyph{

public:
   svCylinderGlyph(svVectorField *f);

   virtual ~svCylinderGlyph();

   virtual void Clean();
   virtual void CleanData();
   virtual void CleanIndex();

   virtual void Update();
   virtual void UpdateData();
   virtual void UpdateIndex();

   void SetTubeRadius(svScalar r){glyphTubeRadius = r;}
   svScalar GetTubeRadius(){return glyphTubeRadius;}

   virtual void UpdateIndexVBO();
   virtual void UpdateTubeVBO();

   virtual void GenerateVBO();
   virtual void RenderVBO();

   virtual void Generate(){};
   virtual void Render(){};

   virtual void SetProperty(CylinderProperty &cylinderproperty){cproperty = cylinderproperty;} 
   virtual void SetLegendProperty(CylinderLegendProperty &legendproperty){lproperty = legendproperty;}

//================================================================
   virtual void Draw_InnerLegend();
   virtual void Draw_WidthLegend();

   svVector3Array *getcylinderEnd1(){return cylinderEnd1;}
   svVector3Array *getcylinderEnd2(){return cylinderEnd2;}
   svVector3Array *getcylinderEnd3(){return cylinderEnd3;}
   svVector3Array *getcylinderEnd4(){return cylinderEnd4;}

   svVector3 GetEnd(int seed, int index){svScalar s = 2; return (cylinderEnd3[seed][index] + cylinderEnd4[seed][index])/s;}
   svVector3 getcylinderEnd1(int seed, int index){return cylinderEnd1[seed][index];}
   svVector3 getcylinderEnd2(int seed, int index){return cylinderEnd2[seed][index];}
   svVector3 getcylinderEnd3(int seed, int index){return cylinderEnd3[seed][index];}
   svVector3 getcylinderEnd4(int seed, int index){return cylinderEnd4[seed][index];}

protected:
   void cleanup();

   CylinderLegendProperty lproperty;
   CylinderProperty cproperty;

   svScalar glyphTubeRadius;

   GLuint TUBE_VBO;
   GLuint TUBE_IVBO;
   GLuint TUBE_BASE_VBO;
   GLuint TUBE_BASE_IVBO;

   int tube_indices_size;
   int tube_vertices_size;
   int tube_base_indices_size;
   int tube_base_vertices_size;

   VBOVertex *tube_vertices;
   VBOVertex *tube_base_vertices;
   VBOVertex *legend_vertices;

   int *tube_indices;
   int *tube_base_indices;

//the plane facing to users (for selection)
   svVector3Array *cylinderEnd1;
   svVector3Array *cylinderEnd2;
   svVector3Array *cylinderEnd3;
   svVector3Array *cylinderEnd4;
};
}

#endif


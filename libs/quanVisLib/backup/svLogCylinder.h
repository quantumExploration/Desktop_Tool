/* rendering routine of a vector field using glyphs */

#ifndef __SV_LOG_CYLINDER_H
#define __SV_LOG_CYLINDER_H

#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
#include "svCylinderGlyph.h"
#include "svUtil.h"
#ifndef PI
#define PI 3.1415926
#endif


#ifndef CYLINDERSLICE 
#define CYLINDERSLICE 6
#endif 

namespace __svl_lib {


class svLogCylinder : public svCylinderGlyph{

public:
   svLogCylinder(svVectorField *f);

   virtual ~svLogCylinder(){};

   virtual void GenerateTubes();
   virtual void GenerateIndex();

    virtual void RenderSample();

   virtual void Render();
   virtual void Generate(){}

   virtual void Generate(DirectProperty &property,ViewProperty &viewproperty);
   virtual void Generate(DirectProperty &property,ViewProperty &viewproperty, int index);

};


}

#endif

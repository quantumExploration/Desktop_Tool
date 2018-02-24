/* rendering routine of a vector field using glyphs */

#ifndef __SV_LLO_CYLINDER_H
#define __SV_LLO_CYLINDER_H

#include "svVectorField.h"
#include "svCylinderGlyph.h"


namespace __svl_lib {


class svLLOCylinder : public svCylinderGlyph{

public:
   svLLOCylinder(svVectorField *f);

   virtual ~svLLOCylinder(){};


   virtual void GenerateTubes();
   virtual void GenerateIndex();

    virtual void RenderSample();

    virtual void GenerateLegend();
    virtual void RenderLegend();

    virtual void SetES(svScalar s){expScale = s;}
private:
   svScalar expScale;

};


}

#endif

/* rendering routine of a vector field using glyphs */

#ifndef __SV_LC_CYLINDER_H
#define __SV_LC_CYLINDER_H

#include "svVectorField.h"
#include "svCylinderGlyph.h"


namespace __svl_lib {


class svLCCylinder : public svCylinderGlyph{

public:
   svLCCylinder(svVectorField *f);

   virtual ~svLCCylinder(){};


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

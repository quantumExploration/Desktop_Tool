/* rendering routine of a vector field using glyphs */

#ifndef __SV_LLP_CYLINDER_H
#define __SV_LLP_CYLINDER_H

#include "svVectorField.h"
#include "svCylinderGlyph.h"


namespace __svl_lib {


class svLLPCylinder : public svCylinderGlyph{

public:
   svLLPCylinder(svVectorField *f);

   virtual ~svLLPCylinder(){};

   virtual void UpdateLLPData();
   virtual void UpdateLLPIndex();

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

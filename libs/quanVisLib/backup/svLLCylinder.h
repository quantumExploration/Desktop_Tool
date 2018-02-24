/* rendering routine of a vector field using glyphs */

#ifndef __SV_LL_CYLINDER_H
#define __SV_LL_CYLINDER_H

#include "svVectorField.h"
#include "svCylinderGlyph.h"


namespace __svl_lib {


class svLLCylinder : public svCylinderGlyph{

public:
   svLLCylinder(svVectorField *f);

   virtual ~svLLCylinder(){delete [] legend_vertices_h;};

   virtual void UpdateLLLegend();

   virtual void GenerateTubes();
   virtual void GenerateIndex();

    virtual void RenderSample();

    virtual void GenerateLegend();
    virtual void RenderLegend();

    virtual void SetES(svScalar s){expScale = s;}
private:
   svScalar expScale;
VBOVertex *legend_vertices_h;
};


}

#endif

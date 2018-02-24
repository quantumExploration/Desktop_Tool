/* rendering routine of a vector field using glyphs */


#ifndef __SV_LT_CYLINDER_H
#define __SV_LT_CYLINDER_H

#include "svVectorField.h"
#include "svCylinderGlyph.h"


namespace __svl_lib {

class svLTCylinder : public svCylinderGlyph{

public:
   svLTCylinder(svVectorField *f);

   virtual ~svLTCylinder(){};

   virtual void UpdateLTData();
   virtual void UpdateLTIndex();

   virtual void GenerateTubes();
   virtual void GenerateIndex();

    virtual void RenderSample();

    virtual void GenerateLegend();
    virtual void RenderLegend();

    virtual void SetES(svScalar s){expScale = s;}
private:
   svScalar expScale;
   svInt cylinder_segment;
   svInt texture_index;
};


}

#endif

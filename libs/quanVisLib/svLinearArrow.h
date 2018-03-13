
/* rendering routine of a vector field using glyphs */

#ifndef __SV_LINEAR_ARROW_H
#define __SV_LINEAR_ARROW_H

//#include "svArray.h"
//#include "svVectorField.h"
//#include "svParticle.h"
//#include "svPrimitive.h"
//#include "svLut.h"
//#include "svGlyph.h"

#include "svArrowGlyph.h"

#ifndef PI
#define PI 3.1415926
#endif

namespace __svl_lib {

class svLinearArrow : public svArrowGlyph {
  public:
    //svLCArrow(svChar *inf);
    svLinearArrow(svQDOTData* d);//svQDOTData *d);//svVectorField *f);
	
    virtual ~svLinearArrow(){}
    // void SetNumPower(svInt power){numPower = power;}
  
//    virtual void Render();
    virtual void SetColors(svColors *color);

    virtual void GenerateArrows();
    virtual void GenerateTubes();
   // virtual void GenerateLegend();
    virtual void GenerateIndex();
   // virtual void GenerateIndex(int region);
    //virtual void GenerateVBO();

    //virtual void RenderVBO();
//    virtual void RenderLegend();
//    virtual void RenderSample();

 //   virtual void GenerateSpring();

   // virtual void Generate();
   // virtual void GenerateTexture();
   // virtual void GenerateSample(int frequency);
//(int contour, svScalar z1, svScalar z2, int z, int frequency);

  private:

 //   svScalar   glyphSize;
//    svScalar   glyphScale;
//    svScalar   glyphRadius;
   //svInt numPower;
  protected:
   // virtual void BuildDisplayListFromStore();
};

}


#endif // __SV_LC_ARROW_H

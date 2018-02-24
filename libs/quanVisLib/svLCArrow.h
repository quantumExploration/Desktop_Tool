
/* rendering routine of a vector field using glyphs */

#ifndef __SV_LC_ARROW_H
#define __SV_LC_ARROW_H

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

class svLCArrow : public svArrowGlyph {
  public:
    //svLCArrow(svChar *inf);
    svLCArrow(svQDOTData* d);//svQDOTData *d);//svVectorField *f);
	
    virtual ~svLCArrow(){}
    // void SetNumPower(svInt power){numPower = power;}
    void SetColors(svColors *color);  
//    virtual void Render();
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
    virtual void SetES(svScalar s){expScale = s;}
    virtual void SetCS(svScalar s){coeScale = s;}

  private:

    void RenderStore() const; 
 //   svScalar   glyphSize;
//    svScalar   glyphScale;
//    svScalar   glyphRadius;
   svScalar expScale;
   svScalar coeScale;
   //svInt numPower;
  protected:
   // virtual void BuildDisplayListFromStore();
};

}


#endif // __SV_LC_ARROW_H

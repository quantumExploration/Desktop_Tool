
/* rendering routine of a vector field using glyphs */

#ifndef __SV_LT_ARROW_H
#define __SV_LT_ARROW_H

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

#define REPEAT_TEXTURE 3

namespace __svl_lib {

class svLTArrow : public svArrowGlyph {
  public:
    svLTArrow(svChar *inf);
    svLTArrow(svQDOTData *d);//svVectorField *f);
	
    virtual ~svLTArrow(){cleancolor();};
    // void SetNumPower(svInt power){numPower = power;}

    virtual void SetColors(svColors *color);
    virtual void UpdateData();
    virtual void UpdateIndex();
//    virtual void UpdateIndex(int region);  

    virtual void GenerateArrows();
    virtual void GenerateTubes();
//    virtual void GenerateLegend();
    virtual void GenerateIndex();
//    virtual void GenerateIndex(int reigon);

//    virtual void RenderLegend();
//    virtual void RenderSample();

    virtual void SetES(svScalar s){expScale = s;}
    virtual void SetCS(svScalar s){coeScale = s;}

    virtual void SaveTubetoOSG(char *file);
    virtual void SaveArrowtoOSG(char *file);
  private:

   void cleancolor();
   svScalar expScale;
   svScalar coeScale;
   int numPower;
   int cylinder_segment;
   svVector4Array *secondColors;
  protected:
};

}


#endif // __SV_LT_ARROW_H

#ifndef __SV_SUMMARY_GLYPH_H
#define __SV_SUMMARY_GLYPH_H
#include <string.h>
#include "svArrowGlyph.h"

/*

#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
*/
#ifndef PI
#define PI 3.1415926
#endif

/*
svRawSummaryData
svContourSummaryData
one summary glyph for each splitdata[i]
*/

namespace __svl_lib {

class svSummaryGlyph : public svArrowGlyph {
  public:
    svSummaryGlyph(svQDOTData *d);
    virtual ~svSummaryGlyph(){}
    //svSummaryGlyph();

   // void SetNumPower(svInt power){numPower = power;}

   virtual void GenerateList();
   virtual void Render();

    virtual void SetColors(svColors *color);
    //virtual void SetScaling(svInt s){scaling =s;}

    virtual void Generate(float alpha);
    virtual void Render();
    virtual void RenderFont();
    virtual void RenderColor();


protected:
    void cleanup();
    void RenderAnnulus(svVector3 pos, svVector3 vel,
                        svScalar left,svScalar x, svScalar H, svScalar r);
private:
    int summary_list;//display list

    //svVector3Array summaryPos;
    //svVector3Array summaryDir;
    //svScalarArray  summaryDen;
    //svIntArray     summaryLabel;
    //svIntArray     summaryCount;
    //svVector4Array summaryColor;
    //svVector4Array summaryDenColor;
    //svScalarArray summaryMaxDen;

    //svInt scaling;
    //svInt numPower;

    //svScalarArray *visibleDistribute;
    //svScalarArray *denDistribute;
};

}

#endif // __SV_SUMMARY_GLYPH_H

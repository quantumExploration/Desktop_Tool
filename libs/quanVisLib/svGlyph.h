
#ifndef __SV_GLYPH_H
#define __SV_GLYPH_H

#include "svUtil.h"
#include "svArray.h"
#include "svVectorField.h"
#include "svParticle.h"
#include "svPrimitive.h"
#include "svLut.h"
#include "svColors.h"
#include "svList.h"
#include "svQDOTData.h"
#include "svColors.h"

#define DEFAULT_GLYPH_HEIGHT 0.08
#define DEFAULT_GLYPH_FREQUENCY 1
#define DEFAULT_GLYPH_SECTION_NUM 5
#define DEFAULT_GLYPH_SCALE  50000
#define DEFAULT_GLYPH_SIZE  1
#define DEFAULT_GLYPH_RADIUS 300
#define DEFAULT_DISPLAYLIST 5

#ifndef ARROWSLICE 
#define ARROWSLICE 4
#endif

#ifndef CYLINDERSLICE
#define CYLINDERSLICE 6
#endif

#ifndef SPHERESLICE
#define SPHERESLICE 6
#endif

#ifndef SPHERESTACK
#define SPHERESTACK 6
#endif



namespace __svl_lib {


typedef struct DirectProperty{
  svScalar UnitHeight;
  svScalar width;
  svScalar shift;
}DirectProperty;


typedef struct SplitVectorsProperty{
  svScalar shiftexp;
  svScalar expHeight;
  svScalar coeHeight;
  svScalar expWidth;
  svScalar coeWidth;
  svVector4 coeColor;
  svVector4 expColor;
} SplitVectorsProperty;


class svGlyph : public svPrimitive
{
 public:
  svGlyph(vector< vector<Spin *> > myData);

  virtual ~svGlyph();
  void   clean(); // clean memory

// Generate
  virtual void Generate(){};

// Rendering
  virtual void Render(){};
  virtual void RenderSample(){};
  virtual void DrawGrid(svVector3 startPos, svVector3 dir1, svVector3 dir2,
                       svScalar stepD1, svScalar stepD2,
                       svInt stepN1, svInt stepN2);
 // virtual void DrawSilkPlane(svVector3 planeDir);

//color=============
  virtual void SetColors(svColors *color);
/*
  virtual void DisableColor();
  virtual void SetSolidColor();
  virtual void SetColor(svVector4 color);
  virtual void SetColorByCluster();
  virtual void SetColor(int index1, int index2, svVector4 color1, svVector4 color2);
  virtual void UpdateColor();
  virtual void SetColorByCluster(svIntArray index, svVector4 c);
  virtual void SetColorByCluster(svIntArray cluster);
  virtual void SetColorByClusterMag();
  virtual void SetColorByPower();
  virtual void SetColorBySymmetryType();
*/
  virtual void SetScaling(svInt s){scaling =s;}
  virtual void SetNumPower(svInt power){numPower = power;}

  virtual void SetBBox();

   svVector3 GetLb(){return lbbox;}
   svVector3 GetRb(){return rbbox;}
   virtual void SetAlpha(svScalar a){alpha =a;}
   virtual void SetRadius(svScalar radius){glyphRadius = radius;}
   virtual void SetScale(svScalar scale){glyphScale = scale;}
   virtual svScalar GetRadius(){return glyphRadius;}
   virtual svScalar GetScale(){return glyphScale;}
   svVector4Array *GetColors(){return glyphColors;}
 private:

 protected:
//  svQDOTData *myData;
  svColors *myColor;
  vector< vector<Spin *> > myData;  

  svInt glyphSize;
  svScalar glyphScale;
  svScalar glyphRadius;

  svVector4Array *glyphColors;
  svScalarArray  *glyphWidth;
                             
  svScalar alpha;
  svInt scaling;
  svInt numPower;
};

}

#endif // __SV_GLYPH_H

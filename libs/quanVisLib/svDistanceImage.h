
#ifndef __SV_DISTANCE_IMAGE_H
#define __SV_DISTANCE_IMAGE_H

#include "svImage.h"
#include "svDistance.h"

namespace __svl_lib {


class svDistanceImage : public svImage 
{
 public:
  svDistanceImage();
  virtual ~svDistanceImage();

  virtual void ResetData(int seed);

  virtual void GenerateDistance(DistanceProperty &property);

  virtual void SaveDistancetoFile(char *file);

 private:
  svScalarArray *dLength;

 protected:
  virtual void cleandistance();
};

}

#endif // __SV_DISTANCE_IMAGE_H

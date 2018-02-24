
#ifndef __SV_ENTROPY_IMAGE_H
#define __SV_ENTROPY_IMAGE_H

#include "svImage.h"

namespace __svl_lib {

class svEntropyImage : public svImage 
{
 public:
  svEntropyImage();
  virtual ~svEntropyImage();

  virtual void resetData(int seed);
  virtual void generateEntropy(svScalar length,
                               svScalar width);
  virtual void setColorsbyEntropy(); 
 
  virtual void SaveDistancetoFile(char *file);

 private:
  svScalarArray *entropy;
  svScalar maxEntropy;
  svScalar minEntropy;

 protected:
  virtual void cleanentropy();
};

}

#endif // __SV_ENTROPY_IMAGE_H

#ifndef SV_MOUSE_INTERACTION
#define SV_MOUSE_INTERACTION

#include "svMouseGroup.h"


namespace __svl_lib{
class svMouseInteraction{
public:
  void mouse(int button, int state, int x, int y);
  void motion(int x, int y);
  void movement(int x ,int y);

  bool success;
  svMouseGroup *group;
//  MouseEventProperty *current;
};
}
#endif

#include "dataApi.h"
#include "type.h"
#include "paser.h"

class dataUpdater{
public:
  dataUpdater(dataApi *data, Paser *paser){myData = data;this->paser=paser;}
  ~dataUpdater(){};

  void resetData();
  void updateData(char *inputfile);
  void updateContour();
  void updateCluster(visibilityProperty &vproperty);
  void updateVisibility(visibilityProperty &vproperty);
private:
  dataApi *myData;
  Paser *paser;
};





#include "dataUpdater.h"
void dataUpdater::resetData(){
  myData->resetData();
}
void dataUpdater::updateData(char *inputfile){
  paser->ReadFiles(inputfile);
  myData->init(paser->GetRawDir(), paser->GetRawFile(), paser->GetStoreDir(),
              paser->GetPlaneCenter(), paser->GetPlaneVector(),
              paser->GetPlaneDistance(), paser->GetFormat());
  svVector3 c = myData->getPlaneC();
  paser->SetSeedNum(myData->getSeed());
  paser->SetPlane(myData->getPlaneD(),myData->getPlaneC());
  char *str = new char[400];
  sprintf(str, "%s/%s/density%s", paser->GetStoreDir(), paser->GetRawFile(), paser->GetPlaneName());
  paser->ProcessContour();
  paser->SetContourValues(myData->getContourValues(str));
  paser->ProcessClusterLayers();
  paser->ResetBool();
  delete [] str;
  myData->setRawData(paser->GetStoreDir(), paser->GetRawFile(),
                  paser->GetSymmetryProperty());
  //myData->getBoundary(lb, rb);
  myData->setContourTree(paser->GetContourTreeProperty(),
                        paser->GetContourProperty());
}
void dataUpdater::updateContour(){
  myData->setContourData(paser->GetContourProperty());
}
void dataUpdater::updateCluster(visibilityProperty &vproperty){
  myData->setCluster(paser->GetMagRange00(),paser->GetMagRange10(),
                      paser->GetMagRange01(),paser->GetMagRange11(),
                      paser->GetStep1Property(), paser->GetStep2Property(),
                      paser->GetSymmetryProperty(), vproperty.symmetrytype);
  myData->setContourCluster(paser->GetMagRange00(),paser->GetMagRange10(),
                      paser->GetMagRange01(),paser->GetMagRange11(),
                      paser->GetStep1Property(), paser->GetStep2Property());
}
void dataUpdater::updateVisibility(visibilityProperty &vproperty){
  myData->resetVisible();
  myData->resetContourVisible();
  myData->setVisible(vproperty.symmetrytype, vproperty.visiblelayer, 
                     vproperty.frequency, vproperty.min_mag, vproperty.max_mag,
                     vproperty.lb, vproperty.rb, 
                     vproperty.min_xangle, vproperty.min_xangle,
                     vproperty.min_yangle, vproperty.max_yangle,
                     vproperty.min_zangle, vproperty.max_zangle,
                     vproperty.samplesize);
  myData->setContourVisible(vproperty.visiblelayer,
                     vproperty.frequency, vproperty.min_mag, vproperty.max_mag,
                     vproperty.lb, vproperty.rb,
                     vproperty.min_xangle, vproperty.min_xangle,
                     vproperty.min_yangle, vproperty.max_yangle,
                     vproperty.min_zangle, vproperty.max_zangle,
                     vproperty.samplesize);
}

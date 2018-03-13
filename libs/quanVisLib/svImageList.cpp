#include "svImageList.h"

namespace __svl_lib{
  svImageList::svImageList(svRawSliceData *data)
  {
    myData = NULL;
    New(data);
  }
  void svImageList::New(svRawSliceData *data)
  {
    if(myData!=NULL){cleanup();}
    myData = data;
    imageList.resize(myData->splitData.size());
    float origin_sidex =  (float)(myData->imageColumn * myData->iminD[0]);//min(myData->iminD[0],myData->iminD[1]));
    float origin_sidey =  (float)(myData->imageRow * myData->iminD[1]);//min(myData->iminD[0],myData->iminD[1]));
    for(int i=0;i<myData->splitData.size();i++)
    {
      imageList[i] = new svHybridImage();
      imageList[i]->myData = myData;
      imageList[i]->sidex = origin_sidex;
      imageList[i]->sidey = origin_sidey;
      imageList[i]->iminD = myData->iminD;
      imageList[i]->row = myData->imageRow;
      imageList[i]->column = myData->imageColumn;
     // imageList[i]->pixelSize = min(myData->iminD[0], myData->iminD[1]);
      imageList[i]->sliceIndex = i;
    }
    svVector3 dir = myData->sliceDir;
    GetRotateAngle(dir, rotate_x, rotate_z);
    rotate_x = rotate_x/PI * 180.;
    rotate_z = rotate_z/PI * 180.;
    groupList.resize(myData->splitData.size());
    groupSize = myData->splitData.size();
  }
  void svImageList::Reshape(int segment, int width, int left, int top)
  {
    this->width = width;
    this->left = left;
    this->top = top;
    float sidex = (float)width/(float)segment;
    float origin_sidex =  (float)(myData->imageColumn * myData->iminD[0]);//min(myData->iminD[0],myData->iminD[1]));
    float scale = sidex/origin_sidex;
    float sidey = sidex * (float)(myData->imageRow*myData->iminD[1])/(float)(myData->imageColumn*myData->iminD[0]);
//    float sidey = sidex * (float)(myData->imageColumn*myData->iminD[0])/(float)(myData->imageRow*myData->iminD[1]);
    for(int i=0;i<imageList.size();i++){
      imageList[i]->scale = scale;
      imageList[i]->sidex = sidex;
      imageList[i]->sidey = sidey;
      imageList[i]->tranx = left + i%segment;
      imageList[i]->trany = top - i/segment - sidey;
    }
  }
  void svImageList::Reshape(int left, int top, int width)
  {
    this->left = left;
    this->top = top;
    this->width = width;
    SetLocations();
  }

  void svImageList::AddHybridImage(svContourData *myContour,string dir, float cutoff){
    //cerr<<"myslice "<<imageList.size()<<endl;
    for(int i=0;i<imageList.size();i++){
      string ifname1 = dir + "/imagecolor.txt";
      string ifname2 = dir + "/contourcolor.txt";
      string ofname = dir + "/hybridimage.txt";
      imageList[i]->myContour = myContour;
      imageList[i]->GenerateHybridImage(ifname1, ifname2, cutoff, ofname);//, i);
//      imageList[i]->GenerateDisplay(i+10,i+imageList.size()+10);
    }
  }
  void svImageList::GenerateDisplay()
  {
    
    for(int i=0;i<imageList.size();i++){
       //cout<<imageList[i]->sidex<<" "<<imageList[i]->sidey<<endl;
       imageList[i]->GenerateDisplay(i+10,i+imageList.size()+10);
    }
  }
  void svImageList::SetColors(svColors *colors){
    svVector4Array *pixelcolor = new svVector4Array[myData->sliceData.size()];
    svVector4 white(1,1,1,1);
    for(int i=0;i<myData->sliceData.size();i++){
      for(int j=0;j<myData->sliceData[i].size();j++){
        pixelcolor[i].add(white);
      }
    }
    switch (imageType) {
      case magnitude_image: colors->SetColorType(c_magnitude);break;
      case cluster_image: colors->SetColorType(c_cluster);break;
    }
    colors->GetColors(myData, pixelcolor);
    for(int i=0;i<myData->sliceData.size();i++){
      for(int j=0;j<myData->sliceData[i].size();j++){
        int index = myData->sliceData[i][j].torindex;
        myData->sliceData[i][j].icolor = pixelcolor[i][index];
      }
    }
    for(int i=0;i<myData->sliceData.size();i++)
      pixelcolor[i].free();
    delete [] pixelcolor;
  }

  void svImageList::GenerateGroupingByColor(float percentage){
    cerr<<"Group images ..."<<endl;
    for(std::list< vector<int> >::iterator it=groupList.begin();
          it!=groupList.end();++it){
          (*it).clear();
    }
    groupList.clear();
    bool *traverse = new bool[imageList.size()];
    for(int i=0;i<imageList.size();i++){
      traverse[i] = false;
    }
    for(int i=0;i<imageList.size();i++){
      if(!traverse[i]){
        //cerr<<i<<endl;
        vector<int> sameList;
        //#pragma omp parallel for
        for(int j=0;j<imageList.size();j++){
          if(traverse[j] == false){
            //cerr<<j<<" ";
            bool same = GenerateGroupingByColor(i,j,percentage);
            if(same){
              sameList.push_back(j);
            }
          }
        }
        for(int j=0;j<sameList.size();j++){
          traverse[sameList[j]] = true;
        }
        sort(sameList.begin(), sameList.end());
        groupList.push_back(sameList);
        sameList.clear();
      }
    }
    delete [] traverse;
    SetLocations();
  }

  bool svImageList::GenerateGroupingByColor(int image1, int image2, float percentage){
    int count = 0;
    float whole = 0;
    for(int i=0;i<myData->imageRow;i++){
      for(int j=0;j<myData->imageColumn;j++){
        int in = i * myData->imageColumn + j;
        int index1 = myData->imageData[image1][in];
        int index2 = myData->imageData[image2][in];
        if(index1<0 && index2<0){
          //count++;
        }
        else if(index1>=0 && index2>=0){
          svVector4 color1 = myData->sliceData[image1][index1].icolor;
          svVector4 color2 = myData->sliceData[image2][index2].icolor;
          if(isEqual(color1[0], color2[0], false)
          && isEqual(color1[1], color2[1], false)
          && isEqual(color1[2], color2[2], false)){
            count++;
          }
          whole++;
        }
      }
    }
    float per = (float)count/whole;//(float)(myData->imageRow*myData->imageColumn);
  //  cerr<<per<<endl;
    if(per > percentage) return true;
    else return false;
  }

  void svImageList::SetLocations(){
    int segment = groupList.size();
    float sidex = (float)width/(float)segment;
    if(sidex > 40 ) sidex = 40;//hard code!!!
    float origin_sidex =  (float)(myData->imageColumn * myData->iminD[0]);//min(myData->iminD[0],myData->iminD[1]));
    float scale = sidex/origin_sidex;
    for(int i=0;i<imageList.size();i++){
      imageList[i]->scale = scale;
    }
    int x = left;
    float sidey = sidex * (float)(myData->imageRow*myData->iminD[1])/(float)(myData->imageColumn*myData->iminD[0]);
    int y = top - sidey;
    for(std::list< vector<int> >::iterator it=groupList.begin();
          it!=groupList.end();++it){
      for(int i=0;i<(*it).size();i++){
        int index = (*it)[i];
        imageList[index]->scale = scale;
        imageList[index]->sidex = sidex;
        imageList[index]->sidey = sidey;
        imageList[index]->tranx = x;
        imageList[index]->trany = y;
        //cerr<<top<<" "<<x<<" "<<y<<" "<<imageList[index]->scale<<endl;
        y = y - sidey;
      }
      x = x + sidex;
      y = top - sidey;
    }
  }

  void svImageList::Render2D(){
    for(int i=0;i<imageList.size();i++)
    { 
      glPushMatrix();
      glTranslatef(imageList[i]->tranx, imageList[i]->trany, 0);
      glScalef(imageList[i]->scale, imageList[i]->scale, 1);
      imageList[i]->Render2D();
      glPopMatrix();
     // imageList[i]->isSelected = myState->rP->splitVisible[i];
      if(imageList[i]->isSelected || imageList[i]->isHighlighted)
      {
         glPushMatrix();
         glTranslatef(imageList[i]->tranx, imageList[i]->trany, 0);
         glLineWidth(4.);
         glColor3f(253./255., 174./255., 97./255.);
    	 glBegin(GL_LINE_LOOP);
      	 glVertex2f(0,0);
   	 glVertex2f(imageList[i]->sidex, 0);
   	 glVertex2f(imageList[i]->sidex, imageList[i]->sidey);
   	 glVertex2f(0,imageList[i]->sidey);
  	 glEnd();
         glLineWidth(1.);
         glPopMatrix();
      }
    }
  }

  void svImageList::Render3D(){
    for(int i=0;i<imageList.size();i++)
    {
       if(imageList[i]->isSelected){
         //cout<<i<<endl;
         int jj = myData->imageData[i][0];
         svVector3 pos = myData->sliceOrigin[i];
         //cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<endl;
//(*(myData->splitData[i][myData->sliceData[i][jj].torindex])).pos;
       //  cout<<pos[0]<<" "<<pos[1]<<" "<<pos[2]<<endl;
         glPushMatrix();
         glTranslatef(pos[0], pos[1], pos[2]);
         //glTranslatef(imageList[i]->sidex/2., imageList[i]->sidey/2.,0);
         glRotatef(rotate_z,0,0,1);
         glRotatef(rotate_x,1,0,0);
         //cout<<rotate_z<<" "<<rotate_x<<endl;
         //glTranslatef(-imageList[i]->sidex/2., -imageList[i]->sidey/2.,0);
         imageList[i]->Render3D();
         glPopMatrix();
       }
    }
  }

  void svImageList::UpdateState(){
   for(int i=0;i<imageList.size();i++)
   {
      if(imageList[i]->isSelected){// || myState->rP->splitVisible[i]){
        myState->rP->sliceVisible[i] = true;
      }
      else
      {
         myState->rP->sliceVisible[i]=false;
      }
    }
    myState->updateVisible = true;
  }

}

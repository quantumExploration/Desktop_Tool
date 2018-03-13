#include "svImageMouse.h"
#include <algorithm>
using namespace std;

namespace __svl_lib{

void svImageSelect::Reset()
{
  for(int i=0;i<imageList->imageList.size();i++)
  {
    imageList->imageList[i]->isSelectable = true;
    imageList->imageList[i]->isSelected = false;
    imageList->imageList[i]->isHighlighted = false;
  }
  boundary2D[0][0]=9e+9;
  boundary2D[0][1]=9e+9;
  boundary2D[1][0]=-9e+9;
  boundary2D[1][1]=-9e+9;

  for(int i=0;i<imageList->imageList.size();i++)
  {
    float tranx = imageList->imageList[i]->tranx;
    float sidex = imageList->imageList[i]->sidex;
    float trany = imageList->imageList[i]->trany;
    float sidey = imageList->imageList[i]->sidey;
    if(boundary2D[0][0]>tranx)
        boundary2D[0][0] = tranx;
    if(boundary2D[1][0]<sidex + tranx)
        boundary2D[1][0] = sidex + tranx;
    if(boundary2D[0][1]>trany)
        boundary2D[0][1] = trany;
    if(boundary2D[1][1]<sidey + trany)
        boundary2D[1][1] = sidey + trany;
  }
  selectpixel[0]=-1;
  selectpixel[1]=-1;
  selectpixel[2]=-1;
  selectlayer.clear();
  selectlabel = -1;
  selectedlayer.clear();
//  for(int i=0;i<imageList->myData->splitData.size();i++)
//     selectedlayer.push_back(i);
  selectedlabel.clear();
//  for(int i=-100;i<100;i++) selectedlabel.push_back(i);
  selectedcolor.clear();
}

void svImageSelect::Select(){
  if(mouse_state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
  {
    success = false;
    bool local_success=false;
    switch (selecttype){
      case one_layer://ImageSelection::one_layer:
      {
        //selectlayer.clear();
        /*for(int i=0;i<imageList->imageList.size();i++)
        {
          local_success=SelectLayer(imageList->imageList[i])?true:local_success;
          if(local_success)
          {
            selectlayer.push_back(i);
            std::vector<int>::iterator it;
            it = find (selectedlayer.begin(), selectedlayer.end(), i);
            if (it == selectedlayer.end())
            {
              selectedlayer.push_back(i);
            }
          }
        }*/
        boundary2D[0][0]=9e+9;
        boundary2D[0][1]=9e+9;
        boundary2D[1][0]=-9e+9;
        boundary2D[1][1]=-9e+9;
        for(int i=0;i<imageList->imageList.size();i++)
        {
          float tranx = imageList->imageList[i]->tranx;
          float sidex = imageList->imageList[i]->sidex;
          float trany = imageList->imageList[i]->trany;
          float sidey = imageList->imageList[i]->sidey;
          if(boundary2D[0][0]>tranx)
              boundary2D[0][0] = tranx;
          if(boundary2D[1][0]<sidex + tranx)
             boundary2D[1][0] = sidex + tranx;
          if(boundary2D[0][1]>trany)
              boundary2D[0][1] = trany;
          if(boundary2D[1][1]<sidey + trany)
              boundary2D[1][1] = sidey + trany;
        }
        local_success = SelectLayer(false);
        //if(local_success) isMove = true;
        break;
      }
      case one_3d://ImageSelection::one_3d:
      {
        selectlayer.clear();
        local_success = SelectLayer(true);
        isMove = false;
        break;
      }
      case one_pixel://ImageSelection::one_pixel:
      {
        selectpixel[0]=-1;
        selectpixel[1]=-1;
        selectpixel[2]=-1;
        /*for(int i=0;i<imageList->imageList.size();i++)
        {
          local_success=SelectPixel(imageList->imageList[i]);
          isMove = false;
          if(local_success) break;
        }*/
        local_success=SelectPixel();
        isMove = false;
        break;
      }
      case one_label://ImageSelection::one_label:
      {
        selectlabel = -1;
        /*
        for(int i=0;i<imageList->imageList.size();i++)
        {
          local_success=SelectLabel(imageList->imageList[i])?true:local_success;
          if(local_success)
          {
            std::vector<int>::iterator it;
            it = find (selectedlabel.begin(), selectedlabel.end(), selectlabel);
            if (it == selectedlabel.end() && selectlable>=-1)
            {
              selectedlabel.push_back(selectlabel);
            }
            break;
          }
        }*/
        local_success=SelectLabel();
        isMove = false;
        break;
      }
    }
  success = local_success;
 }
}

bool svImageSelect::SelectLayer(bool addselected)//svImage *image)
{
  isMove = false;
  for(int i=0;i<imageList->imageList.size();i++)
  {
    svImage *image = imageList->imageList[i];
    if(image->isSelectable)
    {
      float tranx = image->tranx;
      float trany = image->trany;
      float sidex = image->sidex;
      float sidey = image->sidey;
      if(mousex>tranx && mousex<tranx+sidex
      && mousey>trany && mousey<trany+sidey)
      {
        image->isHighlighted = true;
        if(addselected)
        {
          if(image->isSelected)
          {
            image->isSelected = false;
            std::vector<int>::iterator it;
            it = find (selectedlayer.begin(), selectedlayer.end(), i);
            if (it != selectedlayer.end())
            {
              selectedlayer.erase(selectedlayer.begin()+i);//erase(i);
            }
          }
          else
          {
            image->isSelected = true;
            std::vector<int>::iterator it;
            it = find (selectedlayer.begin(), selectedlayer.end(), i);
            if (it != selectedlayer.end())
            {
              selectedlayer.push_back(i);
            }
            selectlayer.push_back(i);
          } 
          isMove = false;        
        }
        else
        {
          isMove = true;
          selectlayer.push_back(i); 
        }
        return true;
      }
      else
      {
        //image->
        //isSelected = false;
        //image->isHighlighted = false;
      }
    }
  }
  return false;
}

bool svImageSelect::SelectPixel()//svImage *image)
{
  int index=-1;
  bool local_success = false;
  for(int i=0;i<imageList->imageList.size();i++)
  {
    svImage *image = imageList->imageList[i];
    if(image->isSelectable)
    {
      float tranx = image->tranx;
      float trany = image->trany;
      float sidex = image->sidex;
      float sidey = image->sidey;
      float scale = image->scale;
      if(mousex>image->tranx && mousex<image->tranx+image->sidex
      && mousey>image->trany && mousey<image->trany+image->sidey)
      {
        index = image->sliceIndex;
        selectpixel[2]=index;//selectlayer = index;
      }
      if(index<0)
      {
        continue;
      }

      float x = 0;
      float y = 0;
      svVector3 iminD = image->myData->iminD;
   //   svScalar minD = min(image->myData->iminD[0], image->myData->iminD[1]);
      for(int r=0;r<image->myData->imageRow;r++){
          for(int c=0;c<image->myData->imageColumn;c++){
             if(mousex>x*scale + tranx && mousex < (x+iminD[0]) * scale + tranx
             && mousey>y*scale + trany && mousey < (y+iminD[1]) * scale + trany)
             {
               selectpixel[0] = r;
               selectpixel[1] = c;
               local_success = true;
               break;
             }
             x += iminD[0];
          }
          if(local_success)
          {
            break;
          }
          x = 0;
          y += iminD[1];
      }
      if(local_success)break;
    }
  }
  return local_success;
}

bool svImageSelect::SelectLabel()///svImage *image)
{
  bool local_success = false;
  for(int i=0;i<imageList->imageList.size();i++)
  {
    svImage *image = imageList->imageList[i];
    int index=-1;
    float scale = image->scale;
    float tranx = image->tranx;
    float trany = image->trany;
    float sidex = image->sidex;
    float sidey = image->sidey;
    if(image->isSelectable)
    {
      if(mousex>image->tranx && mousex<image->tranx+image->sidex
      && mousey>image->trany && mousey<image->trany+image->sidey)
      {
        index = image->sliceIndex;
      }
      if(index<0)
        continue;

      float x = 0;
      float y = 0;
      svVector3 iminD = image->myData->iminD;
//      svScalar minD = min(image->myData->iminD[0], image->myData->iminD[1]);
      for(int r=0;r<image->myData->imageRow;r++){
          for(int c=0;c<image->myData->imageColumn;c++){
             int j = r * image->myData->imageColumn + c;
             j = image->myData->imageData[image->sliceIndex][j];
             if(j<0)continue;
             if(mousex>x*scale + tranx && mousex < (x+iminD[0]) * scale + tranx
             && mousey>y*scale + trany && mousey < (y+iminD[1]) * scale + trany)
             {
               selectlabel = image->myData->sliceData[index][j].ilabel;
               int ii=0;
               for(ii=0;ii<selectedlabel.size();ii++)
               {
                  if(selectedlabel[ii] == selectlabel){break;}
               }
               if(ii==selectedlabel.size()) selectedlayer.push_back(selectlabel);
               else selectedlayer.erase(selectedlayer.begin()+ii);
/*
	       selectcolor = image->myData->sliceData[index][j].icolor;
               int c= 0;int size = selectedcolor.size();
               for(c=0;c<selectedcolor.size();c++)
               {
                     if(isEqual(selectcolor[0], selectedcolor[c][0],false)
                     && isEqual(selectcolor[1], selectedcolor[c][1],false)
                     && isEqual(selectcolor[2], selectedcolor[c][2],false))
                     {
                          selectedcolor.erase(selectedcolor.begin()+c);break;
                     }
               }
               if(c==size)selectedcolor.push_back(selectcolor);
*/
               local_success = true;
               break;
             }
             x += iminD[0];
          }
          if(local_success)break;
          x = 0;
          y += iminD[1];
      }
      if(local_success)break;
    }
  }
  return local_success;
}

void svImageMotion::Motion(){
  if(selectEvent->selecttype == one_layer){// && selectEvent->isMove){
    int numOfLayers = selectEvent->selectlayer.size();
    boundary2D[0] = selectEvent->boundary2D[0];
    boundary2D[1] = selectEvent->boundary2D[1];
    switch (numOfLayers) {
      case 0:
      {
        break;
      }
      case 1:
      {
        MoveOneLayer();
        break;
      }
      default:
      {
      //  MoveMultipleLayers();
        break;
      }
    }
  }
}

void svImageMotion::MoveOneLayer()
{
  //if(mousex > boundary2D[0][0] && mousex < boundary2D[1][0]
  //&& mousey > boundary2D[0][1] && mousey < boundary2D[1][1])
  {
    //re-arrange 2d image
    Drag2DinGrid();
  }
  //else
  {
    //drag to 3D
    //Dragto3D();
  }
}

void svImageMotion::MoveMultipleLayers()
{
  if(!(mousex > boundary2D[0][0] && mousex < boundary2D[0][1]
  && mousey > boundary2D[1][0] && mousey < boundary2D[1][1]))
  {
    Dragto3D();
  }
}

void svImageMotion::Drag2DinGrid()
{
  svImageList *imageList = selectEvent->imageList;

  imageList->isGrid = true;
  float sidex = imageList->imageList[0]->sidex;
  float sidey = imageList->imageList[0]->sidey;
  int column = (mousex - boundary2D[0][0])/sidex;
  int row = -(mousey - boundary2D[1][1])/sidey;
  if(column>=0 && row>=0){
    SetLocations(column, row);
  }
}

void svImageMotion::Dragto3D()
{
  svImageList *imageList = selectEvent->imageList;

  for(int i=0;i<selectEvent->selectlayer.size();i++)
  {
    int index= selectEvent->selectlayer[i];
    imageList->imageList[index]->is3D = true;
  }
}

void svImageMotion::SetLocations(int column, int row)
{
  //cout<<column<<" "<<row<<endl;
  svImageList *imageList = selectEvent->imageList;

  int layer_index = selectEvent->selectlayer[0];
  for(list< vector<int> >::iterator it=imageList->groupList.begin();
    it!=imageList->groupList.end();++it)
  {
    bool findit = false;
    for(int i=0;i<(*it).size();i++){
       if((*it)[i] == layer_index){
         (*it).erase((*it).begin() + i); findit = true;
         break;
       }
    }
    if(findit && (*it).size()==0)
    {
       imageList->groupList.erase(it); 
    }
    if(findit) break;
  }
  if(column>=imageList->groupList.size()){
    vector<int> tmpgroup;
    tmpgroup.push_back(layer_index);
    imageList->groupList.push_back(tmpgroup);
    tmpgroup.clear();
  }
  else{
    list< vector<int> >::iterator it = imageList->groupList.begin();
     for(int i=0;i<column;i++)it++;// + column;
    (*it).insert((*it).begin()+(row>(*it).size()?(*it).size():row), layer_index);
  }
  imageList->SetLocations();
}
/*
void svImageMove::Drag2D()
{
  int column, row;
  int index = selectEvent->selectLayer[0];
  float distancex = start_x - x;//imageList->imageList[index]->tranx;
  float distancey = start_y - y;//imageList->imageList[index]->trany;
  float tranx = imageList->imageList[index]->tranx + distancex;
  float trany = imageList->imageList[index]->trany + distancey;
  float sidex = imageList->imageList[index]->sidex;
  float sidey = imageList->imageList[index]->sidey;
  svScalar cut[4];
  if(tranx < bounary2D[0][0])
  {
    cut[0]=0;cut[1]=0;cut[2]=1;cut[3]=0;
    column = -1;
    row = 0;
  }//insert left
  else if(tranx + sidex > boundary2D[0][1])
  {
    cut[0]=0;cut[1]=0;cut[2]=0;cut[3]=1;
    column = imageList->groupList.size();
    row = 0;
  }//insert right
  else
  {
    column = (tranx - boundary2D[0][0])/sidex;
    if(column<0)
    {
      cut[0] = (tranx - boundary2D[0][0])/sidex;
      cut[1] = 1 - cut[0];
    }
    else{
      cut[1] = (tranx - column * sidex)/sidex;
      cut[0] = 1 - cut[1];
    }
    row = -(trany- boundary2D[1][1])/sidey;
    if(row<0)
    {
      cut[2] = (boundary2D[1][1] - trany)/sidey;
      cut[3] = 1- cut[2];
    }// insert top
    else
    {
      cut[3] = ( trany - row * trany - boundary2D[1][1])/sidey;
      cue[2] = 1- cut[3];
    }
  }
  Drag2DLayout(cut, column ,row);
}

void svImage::Drag2DLayout(svScalar *cut, int column, int row, int layerindex)
{
  if((cut[2] > cut[0] && cut[2] > cut[1])
  ||  (cut[3] > cut[0] && cut[3] > cut[1]))
  {
    //new column
    vector<int> tempgroup;
    tempgroup.push_back(layerindex);
    if(column < groupList.size())
    {
      groupList.insert(groupList.begin()+(column+1), tmpgroup);
    }
  }
  else
  {

  }
}
*/
void svImageRelease:: Release()
{
  svImageList *imageList = selectEvent->imageList;

  UpdateState();
  imageList->isGrid = false;
  for(int i=0;i<imageList->imageList.size();i++)
  {
    imageList->imageList[i]->isSelectable = true;
    imageList->imageList[i]->isHighlighted = false;
    //imageList->isSelected = false;
  }
  //TODO: UPDATE STATE
  selectEvent->selectlayer.clear();
  selectEvent->imageList->UpdateState();
  UpdateState();
  //selectEvent->selectpixel[0]=-1;
  //selectEvent->selectpixel[1]=-1;
  //selectEvent->selectlabel = -2;
}

void svImageRelease::UpdateState()
{
  svImageList *imageList = selectEvent->imageList;

  switch (selectEvent->selecttype)
  {
    case (one_3d):
    {
//      state->UpdateSplitVisible(selectEvent->selectedlayer);
      break;
    }
    case (one_pixel)://updatt contour values
    {
      int layerindex = selectEvent->selectpixel[2];//selectEvent->selectlayer;
      svImage *image = imageList->imageList[layerindex];
      int index = selectEvent->selectpixel[0] * image->myData->imageColumn
                + selectEvent->selectpixel[1];
      index = image->myData->imageData[layerindex][index];
      if(index<0)break;//continue;
      index = image->myData->sliceData[layerindex][index].torindex;
      svScalar den = (*(image->myData->splitData[layerindex][index])).den;
      state->UpdateContour(layerindex, den);
      break;
    }
    case (one_label):
    {
      for(int i=0;i<imageList->imageList.size();i++)
      {
         svImage *image = imageList->imageList[i];//layerindex];
         for(int j=0;j<image->myData->sliceData[i].size();j++)
         {
            bool exist=false;
            for(int s =0;s<selectEvent->selectedlabel.size();s++)
            {
                  if(image->myData->sliceData[i][j].ilabel==selectEvent->selectedlabel[s])
                     exist = true;
            }
            int index = image->myData->sliceData[i][j].torindex;
            if(exist && (*image->myData->qdotVisible).at(image->myData->splitData[i][index]) )
              (*image->myData->qdotVisible).at(image->myData->splitData[i][index])=true;
         }
      }
      ////state->UpdateSplitVisible(selectEvent->selectedlabel);
      break;
    }
  }
}

void svImageMove::Move()
{
  movelayer = -1;
  movepixel[0] = -1;
  movepixel[1] = -1;
  movelabel = -2;
  switch (selectEvent->selecttype){
    case one_layer:
    {
      MoveLayer();
      break;
    }
    case one_3d:
    {
      MoveLayer();
      break;
    }
    case one_pixel:
    {
      MovePixel();
      break;
    }
    case one_label:
    {
      MoveLabel();
      break;
    }
  }
}

void svImageMove::MoveLayer()
{
  svImageList *imageList = selectEvent->imageList;
  for(int i=0;i<imageList->imageList.size();i++)
  {
    svImage *image = imageList->imageList[i];
    float tranx = image->tranx;
    float trany = image->trany;
    float sidex = image->sidex;
    float sidey = image->sidey;
    float scale = image->scale;
    if(image->isSelectable)
    {
      if(mousex>tranx && mousex<tranx+sidex
      && mousey>trany && mousey<trany+sidey)
      {
        image->isHighlighted = true;
        movelayer = i;
      }
      else{
        image->isHighlighted = false;
      }
    }
  }
}

void svImageMove::MovePixel()
{
  svImageList *imageList = selectEvent->imageList;
  for(int i=0;i<imageList->imageList.size();i++)
  {
    int index=-1;
    bool local_success = false;
    svImage *image = imageList->imageList[i];
    float tranx = image->tranx;
    float trany = image->trany;
    float sidex = image->sidex;
    float sidey = image->sidey;
    float scale = image->scale;
    if(image->isSelectable)
    {
      if(mousex>image->tranx && mousex<image->tranx+image->sidex
      && mousey>image->trany && mousey<image->trany+image->sidey)
      {
        index = image->sliceIndex;
      }
      if(index<0)
        break;

      float x = 0;
      float y = 0;
      svVector3 iminD = image->myData->iminD;
//      svScalar minD = min(image->myData->iminD[0], image->myData->iminD[1]);
      for(int r=0;r<image->myData->imageRow;r++){
          for(int c=0;c<image->myData->imageColumn;c++){
             if(mousex>x*scale + tranx && mousex < (x+iminD[0]) * scale + tranx
             && mousey>y*scale + trany && mousey < (y+iminD[1]) * scale + trany)
             {
               movepixel[0] = r;
               movepixel[1] = c;
               local_success = true;
               break;
             }
             x += iminD[0];
          }
          if(local_success)break;
          x = 0;
          y += iminD[1];
      }
    }
  }
}

void svImageMove::MoveLabel()
{
  svImageList *imageList = selectEvent->imageList;
  for(int i=0;i<imageList->imageList.size();i++)
  {
    int index=-1;
    bool local_success = false;
    svImage *image = imageList->imageList[i];
    float tranx = image->tranx;
    float trany = image->trany;
    float sidex = image->sidex;
    float sidey = image->sidey;
    float scale = image->scale;
    if(image->isSelectable)
    {
      if(mousex>tranx && mousex<tranx+sidex
      && mousey>trany && mousey<trany+sidey)
      {
        image->isHighlighted = true;
      }
      else{
        image->isHighlighted = false;
      }
/*
      if(mousex>image->tranx && mousex<image->tranx+image->sidex
      && mousey>image->trany && mousey<image->trany+image->sidey)
      {
        index = image->sliceIndex;
      }
      if(index<0) break;

      float x = 0;
      float y = 0;
      svScalar minD = min(image->myData->iminD[0], image->myData->iminD[1]);
      for(int r=0;r<image->myData->imageRow;r++){
          for(int c=0;c<image->myData->imageColumn;c++){
             int j = r * image->myData->imageColumn + c;
             j = image->myData->imageData[image->sliceIndex][j];
             if(j<0)continue;
             if(mousex>x*scale + tranx && mousex < (x+minD) * scale + tranx
             && mousey>y*scale + trany && mousey < (y+minD) * scale + trany)
             {
               movelabel = image->myData->sliceData[image->sliceIndex][j].ilabel;
              // movecolor = image->myData->sliceData[image->sliceIndex][j].icolor;
               local_success = true;
             }
             x += minD;
          }
          //if(local_success)break;
          x = 0;
          y += minD;
      }
*/
    }
  }
}


}

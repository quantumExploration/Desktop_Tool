#include "svImageMouse.h"

namespace __svl_lib{

void svImageSelect:Reset()
{
  for(int i=0;i<imageList->imageList.size();i++)
  {
    imageList->imageList[i]->isSelectable = true;
    imageList->imageList[i]->isSelected = false;
    imageList->imageList[i]->isHighlighted = false;

  }
  selectpixel[0]=-1;
  selectpixel[1]=-1;
  selectlayer.clear();
  selectlable = -1;
  selectedlayer.clear();
  selectedlabel.clear();
}

void svImageSelect::Select(){
  if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)
  {
    success = false;
    bool local_success=false;
    switch (selecttype){
      case ImageSelection::one_layer:
      {
        selectlayer.clear();
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
        local_success = SelectLayer(false);
        //if(local_success) isMove = true;
        break;
      }
      case ImageSelection::one_3d:
      {
        selectlayer.clear();
        local_success = SelectLayer(true);
        isMove = false;
        break;
      }
      case ImageSelection::one_pixel:
      {
        selectpixel[0]=-1;
        selectpixel[1]=-1;
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
      case ImageSelection::one_label:
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
        if(image->isSelected){
          image->isSelected = false;
          isMove = false;
          if(addselected)
          {
            std::vector<int>::iterator it;
            it = find (selectedlayer.begin(), selectedlayer.end(), i);
            if (it != selectedlayer.end())
            {
              selectedlayer.erase(i);
            }
          }
        }
        else{
          image->isSelected = true;
          image->isHighlighted = true;
          if(addselected)
          {
            std::vector<int>::iterator it;
            it = find (selectedlayer.begin(), selectedlayer.end(), i);
            if (it != selectedlayer.end())
            {
              selectedlayer.push_back(i);
            }
            selectlayer.push_back(i);
          }
          isMove = true;
        }
        return true;
      }
      else
      {
        //image->isSelected = false;
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
      if(mousex>image->tranx && mousex<image->tranx+image->sidex
      && mousey>image->trany && mousey<image->trany+image->sidey)
      {
        selectlayer = index;
        index = image->sliceIndex;
      }
      if(index<0)
      {
        continue;
      }

      float x = 0;
      float y = 0;
      svScalar minD = min(image->myData->iminD[0], image->myData->iminD[1]);
      for(int r=0;r<image->myData->imageRow;r++){
          for(int c=0;c<image->myData->imageColumn;c++){
             if(mousex>x*scale + tranx && mousex < (x+minD) * scale + tranx
             && mousey>y*scale + trany && mousey < (y+minD) * scale + trany)
             {
               selectpixel[0] = r;
               selectpixel[1] = c;
               local_success = true;
               break;
             }
             x += minD;
          }
          if(local_success)
          {
            break;
          }
          x = 0;
          y += minD;
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
      svScalar minD = min(image->myData->iminD[0], image->myData->iminD[1]);
      for(int r=0;r<image->myData->imageRow;r++){
          for(int c=0;c<image->myData->imageColumn;c++){
             int j = r * image->myData->imageColumn + c;
             j = image->myData->imageData[myData[index]][j];
             if(mousex>x*scale + tranx && mousex < (x+minD) * scale + tranx
             && mousey>y*scale + trany && mousey < (y+minD) * scale + trany)
             {
               selectlabel = image->myData->sliceData[index][j].ilabel;
               vector<int>::iterator it;
               it = find (selectedlabel.begin(), selectedlabel.end(), i);
               if (it == selectedlayer.end())
               {
                 selectedlayer.push_back(selectlabel);
               }
               else
               {
                  selectedlayer.erase(it);
               }
               local_success = true;
               break;
             }
             x += minD;
          }
          if(local_success)break;
          x = 0;
          y += minD;
      }
      if(local_success)break;
    }
  }
  return local_success;
}

void svImageMove::Move(){
  if(selectEvent->selecttype == one_layer && selectEvent->isMove){
    int numOfLayers = selectEvent->selectlayer.size();
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

void svImageMove::MoveOneLayer()
{
  if(mousex > boundary2D[0][0] && mousex < boundary2D[0][1]
  && mousey > boundary2D[1][0] && mousey < boundary2D[1][1])
  {
    //re-arrange 2d image
    Drag2DinGrid();
  }
  else
  {
    //drag to 3D
    //Dragto3D();
  }
}

void svImageMove::MoveMultipleLayers()
{
  if(!(mousex > boundary2D[0][0] && mousex < boundary2D[0][1]
  && mousey > boundary2D[1][0] && mousey < boundary2D[1][1]))
  {
    Dragto3D();
  }
}

void svImageMove::Drag2DinGrid()
{
  imageList->isGrid = true;
  float sidex = imageList->imageList[0]->sidex;
  float sidey = imageList->imageList[0]->sidey;
  int column = (mousex - boundary2D[0][0])/sidex;
  int row = -(mousey - bounary2D[1][1])/sidey;
  if(column>=0 && row>=0){
    SetLocations(column, row);
  }
}

void svImageMove::Dragto3D()
{
  for(int i=0;i<selectEvent->selectlayer.size();i++)
  {
    int index= selectEvent->selectlayer[i];
    imageList->imageList[index]->is3D = true;
  }
}

void svImageMove::SetLocations(int column, int row)
{
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
     if(findit)break;
  }
  if(column>=imageList->groupList.size()){
    vector<int> tmpgroup;
    tmpgroup.push_back(layer_index);
    groupList.push_back(tmpgroup);
    tmpgroup.clear();
  }
  else{
    list< vector<int> >::iterator it = imageList->groupList.begin() + column;
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
  UpdateState();
  imageList->isGrid = false;
  for(int i=0;i<imageList->imageList.size();i++)
  {
    imageList->isSelectable = true;
    imageList->isHighlighted = false;
    //imageList->isSelected = false;
  }
  //TODO: UPDATE STATE
  //selectEvent->selectlayer.clear();
  //selectEvent->selectpixel[0]=-1;
  //selectEvent->selectpixel[1]=-1;
  //selectEvent->selectlabel = -2;
}

void svImageRelease::UpdateState()
{
  switch (selectEvent->selecttype)
  {
    case (ImageSelection::one_3d):
    {
      state->UpdateVisible(selectEvent->selectedlayer);
      break;
    }
    case (ImageSelection::one_pixel)://updatt contour values
    {
      int layerindex = selectEvent->selectlayer;
      svImage *image = imageList->imageList[layerindex];
      int index = selectEvent->selectpixel[0] * image->myData->imageColumn
                + selectEvent->selectpixel[1];
      index = image->myData->imageData[layerindex][index];
      index = image->myData->sliceData[layerindex][index].torindex;
      svScalar den = image->myData->splitData[layerindex][index];
      state->UpateContour(layerindex, den);
      break;
    }
    case (ImageSelection::one_label):
    {
      state->UpdateVisible(selectEvent->selectedlabel);
      break;
    }
  }
}

void svImageMotion::Motion()
{
  motionlayer = -1;
  motionpixel[0] = -1;
  motionpixel[1] = -1;
  motionlabel = -2;
  switch (selectEvent->selectype){
    case ImageSelection::one_layer:
    {
      MotionLayer();
      break;
    }
    case ImageSelection::one_pixel:
    {
      MotionPixel();
      break;
    }
    case ImageSelection::one_label:
    {
      MotionLabel();
      break;
    }
  }
}

void svImageMotion::MotionLayer()
{
  for(int i=0;i<imageList->imageList.size();i++)
  {
    svImage *image = imageList->imageList[i];
    float tranx = image->tranx;
    float trany = image->trany;
    float sidex = image->sidex;
    float sidey = image->sidey;
    if(image->isSelectable)
    {
      if(mousex>tranx && mousex<tranx+sidex
      && mousey>trany && mousey<trany+sidey)
      {
        image->isHighlighted = true;
        motionlayer = i;
      }
      else{
        image->isHighlighted = false;
      }
    }
  }
}

void svImageMotion::MotionPixel()
{
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
      svScalar minD = min(image->myData->iminD[0], image->myData->iminD[1]);
      for(int r=0;r<image->myData->imageRow;r++){
          for(int c=0;c<image->myData->imageColumn;c++){
             if(mousex>x*scale + tranx && mousex < (x+minD) * scale + tranx
             && mousey>y*scale + trany && mousey < (y+minD) * scale + trany)
             {
               image->motionpixel[0] = r;
               image->motionpixel[1] = c;
               local_success = true;
               break;
             }
             x += minD;
          }
          if(local_success)break;
          x = 0;
          y += minD;
      }
    }
  }
}

void svImageMotion::MotionLabel()
{
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
      if(index<0) break;

      float x = 0;
      float y = 0;
      svScalar minD = min(image->myData->iminD[0], image->myData->iminD[1]);
      for(int r=0;r<image->myData->imageRow;r++){
          for(int c=0;c<image->myData->imageColumn;c++){
             int j = r * image->myData->imageColumn + c;
             j = image->myData->imageData[myData[image->sliceIndex]][j];
             if(mousex>x*scale + tranx && mousex < (x+minD) * scale + tranx
             && mousey>y*scale + trany && mousey < (y+minD) * scale + trany)
             {
               motionlabel = image->myData->sliceData[image->sliceIndex][j].ilabel;
               local_success = true;
               break;
             }
             x += minD;
          }
          if(local_success)break;
          x = 0;
          y += minD;
      }
    }
  }
}


}

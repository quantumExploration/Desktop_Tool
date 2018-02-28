

#ifndef __SV_WIDGET_H
#define __SV_WIDGET_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <string.h>
#include "svQDOTData.h"
#include "svColors.h"

namespace __svl_lib {

class svWidget {
  public:
    svWidget(svRawSliceData *data);
    ~svWidget(){cleanup();}
    void Reshape(float tranx, float trany,
              float scalex, float scaley,
              int w, int h)
    {
      this->tranx = tranx;
      this->trany = trany;
      this->scalex = scalex;
      this->scaley = scaley;
      float left = w - tranx;
      float size = left/(boxside*4+boxside * myData->splitData.size());
      this->scalex = scalex > size ? size : scalex;  
      cerr<<size<<" "<<scalex<<endl;
    }
    void Init(svRawSliceData *data);
    void Init(svContourData *myContour){};
    void Render(svColors *myColor)
    {
      glPushMatrix();
      glTranslatef(tranx, trany, 0);
      glScalef(scalex, scaley, 1);
      RenderWidgets();
      if(myData!=NULL)RenderMagHistogram(myColor);
      glPopMatrix();
    }
    void RenderWidgets();
    void RenderMagHistogram(svColors *myColor);
    void Repeat(bool showbox);
    void SetIndex(int zmin, int zmax, int notshowz);

    //void Mouse(svScalar tranx, svScalar trany,
    //               svScalar scalex, svScalar scaley,
    //              int x, int y);
    //void Movement(int x, int y);
    //void Move(int x, int y);
    /*void SetHistoValues(svScalarArray *histo)
    {
          tophisto = -1;
          for(int i=0;i<4;i++)
                 histovalues[i].free();
          for(int i=0;i<4;i++)
          {
              for(int j=0;j<histo[i].size();j++)
              {
                 histovalues[i].add(histo[i][j]);

              }
          }
          for(int j=0;j<histo[0].size();j++)
          {
               svScalar count = histo[0][j] + histo[1][j]
                              + histo[2][j] + histo[3][j];
               if(tophisto< count) tophisto = count;
          }
    }*/
    /*void SetValues(svScalarArray v, bool ifmin)
    {
      topmax = -1;
      topvalues.free();
      for(int i=0;i<v.size();i++)
      {
           if(v[i] > topmax) topmax = v[i];
      }
      for(int i=0;i<v.size();i++)
      {
           topvalues.add(v[i]);
      }
    }*/

    //void MoveLeft();
    //void MoveRight();

    //virtual void Render();

    //svVector3 *value){for(int i=0;i<3;i++)values[i]=value[i];}
    //void SetVisible();

    //svIntArray GetVisible(){return layer;}

    //void GetIndex(svInt *index){for(int i=0;i<3;i++)index[i]= layerindex[i];}
    //void Reset();
    //bool isSelect();

    int GetZmin(){return layerindex[0];}
    int GetZmax(){return layerindex[1];}
    int GetNonz(){return layerindex[2];}

//---------------------Interaction parameters======================//
    void SetValues();
    svBool tselect[3];
    bool bselect;
    svInt layerindex[3];
    float tranx;
    float trany;
    float scalex;
    float scaley;

     State *state;
  protected:
    virtual void UpdateState();
    virtual void SetHistoValues();
    virtual void cleanup();
  private:
     svScalar boxside;
     svVector3 box[3];
     svVector3 line[2];
     bool showbox;
     svScalar triangleside;
     svVector3 triangle[3][3];

     svInt level;
//     int mousex;
//     int mousey;
     vector<bool> layer;
     svVector3 values[3];

     //svScalar topmax;
     //svScalarArray topvalues;
     svScalarArray *histovalues;
     //svScalarArray contourexp;
     svScalar tophisto;
     svRawSliceData *myData;
     svContourData *myContour;
};

}


#endif // __SV_WIDGET_H

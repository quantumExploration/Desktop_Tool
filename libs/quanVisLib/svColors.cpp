#include "svColors.h"
#include <algorithm>
#include <fstream>
namespace __svl_lib {

double redtoBlue2[2][3]={
{252,141,89},
{145,191,219}
};

double redtoBlue3[3][3]=
{
{252,141,89},
{255,255,191},
{145,191,219}
};
double redtoBlue4[4][3]=
{
{215,25,28},
{253,174,97},
{171,217,233},
{44,123,182}
};
double redtoBlue5[5][3]=
{
{215,25,28},
{253,174,97},
{255,255,191},
{171,217,233},
{44,123,182}
};
double redtoBlue6[6][3]=
{
{215,48,39},
{252,141,89},
{254,224,144},
{224,243,248},
{145,191,219},
{69,117,180}
};
double redtoBlue7[7][3]={
{215,48,39},
{252,141,89},
{254,224,144},
{255,255,191},
{224,243,248},
{145,191,219},
{69,117,180}
};
double redtoBlue8[8][3]={
{215,48,39},
{244,109,67},
{253,174,97},
{254,224,144},
{224,243,248},
{171,217,233},
{116,173,209},
{69,117,180}
};
double redtoBlue9[9][3]={
{215,48,39},
{244,109,67},
{253,174,97},
{254,224,144},
{255,255,191},
{224,243,248},
{171,217,233},
{116,173,209},
{69,117,180}
};
double redtoBlue10[10][3]={
{165,0,38},
{215,48,39},
{244,109,67},
{253,174,97},
{254,224,144},
{224,243,248},
{171,217,233},
{116,173,209},
{69,117,180},
{49,54,149}
};
double redtoBlue11[11][3]={
{165,0,38},
{215,48,39},
{244,109,67},
{253,174,97},
{254,224,144},
{255,255,191},
{224,243,248},
{171,217,233},
{116,173,209},
{69,117,180},
{49,54,149}
};
double redtoBlue12[12][3]={
{165,0,38},
{215,48,39},
{244,109,67},
{253,174,97},
{254,224,144},
{255,255,191},
{224,243,248},
{171,217,233},
{116,173,209},
{69,117,180},
{49,54,149},
{49,54,89}
};

double redtoBlue13[13][3]={
{89,0,38},
{165,0,38},
{215,48,39},
{244,109,67},
{253,174,97},
{254,224,144},
{255,255,191},
{224,243,248},
{171,217,233},
{116,173,209},
{69,117,180},
{49,54,149},
{49,54,89}
};


svColors::svColors(){
  divergingColor = new svVector4Array[DIVERGING_SIZE];
  divergingColorBlind = new svVector4Array[DIVERGING_SIZE];
  alpha = 1;
  blind_safe = false;
  color_type = c_magnitude;

  initDivergingColor();
  initDiscreteColor();
}

void svColors::initDivergingColor(){
   svVector4 white(1,1,1,1); white[3] = alpha;
   for(int j=0;j<DIVERGING_SIZE;j++){
    if(j==0) divergingColor[j].add(white);
    else if(j<=12){
     switch (j) {
      case 12: {for(int i=0;i<j;i++){
                 svVector4 colorarray(1,1,1,1);
                 colorarray[0] = redtoBlue13[11-i][0];
                 colorarray[1] = redtoBlue13[11-i][1];
                 colorarray[2] = redtoBlue13[11-i][2];
                 divergingColor[j].add(colorarray);}
                 break;}
      case 11: {for(int i=0;i<j;i++){
                 svVector4 colorarray(1,1,1,1);
                 colorarray[0] = redtoBlue12[10-i][0];
                 colorarray[1] = redtoBlue12[10-i][1];
                 colorarray[2] = redtoBlue12[10-i][2];
                 divergingColor[j].add(colorarray);}
                 break;}
      case 10: {for(int i=0;i<j;i++){
                 svVector4 colorarray(1,1,1,1);
                 colorarray[0] = redtoBlue11[9-i][0];
                 colorarray[1] = redtoBlue11[9-i][1];
                 colorarray[2] = redtoBlue11[9-i][2];
                 divergingColor[j].add(colorarray);}
                 break;}
      case 9: {for(int i=0;i<j;i++){
                 svVector4 colorarray(1,1,1,1);
                 colorarray[0] = redtoBlue10[8-i][0];
                 colorarray[1] = redtoBlue10[8-i][1];
                 colorarray[2] = redtoBlue10[8-i][2];
                 divergingColor[j].add(colorarray);}
                 break;}
      case 8: {for(int i=0;i<j;i++){
                 svVector4 colorarray(1,1,1,1);
                 colorarray[0] = redtoBlue9[7-i][0];
                 colorarray[1] = redtoBlue9[7-i][1];
                 colorarray[2] = redtoBlue9[7-i][2];
                 divergingColor[j].add(colorarray);}
                 break;}
      case 7: {for(int i=0;i<j;i++){
                 svVector4 colorarray(1,1,1,1);
                 colorarray[0] = redtoBlue8[6-i][0];
                 colorarray[1] = redtoBlue8[6-i][1];
                 colorarray[2] = redtoBlue8[6-i][2];
                 divergingColor[j].add(colorarray);}
                 break;}
      case 6: {for(int i=0;i<j;i++){
                 svVector4 colorarray(1,1,1,1);
                 colorarray[0] = redtoBlue7[5-i][0];
                 colorarray[1] = redtoBlue7[5-i][1];
                 colorarray[2] = redtoBlue7[5-i][2];
                 divergingColor[j].add(colorarray);}
                 break;}
      case 5: {for(int i=0;i<j;i++){
                 svVector4 colorarray(1,1,1,1);
                 colorarray[0] = redtoBlue6[4-i][0];
                 colorarray[1] = redtoBlue6[4-i][1];
                 colorarray[2] = redtoBlue6[4-i][2];
                 divergingColor[j].add(colorarray);}
                 break;}
      case 4: {for(int i=0;i<j;i++){
                 svVector4 colorarray(1,1,1,1);
                 colorarray[0] = redtoBlue5[3-i][0];
                 colorarray[1] = redtoBlue5[3-i][1];
                 colorarray[2] = redtoBlue5[3-i][2];
                 divergingColor[j].add(colorarray);}
                 break;}
      case 3: {for(int i=0;i<j;i++){
                 svVector4 colorarray(1,1,1,1);
                 colorarray[0] = redtoBlue4[2-i][0];
                 colorarray[1] = redtoBlue4[2-i][1];
                 colorarray[2] = redtoBlue4[2-i][2];
                 divergingColor[j].add(colorarray);}
                 break;}
      case 2: {for(int i=0;i<j;i++){
                 svVector4 colorarray(1,1,1,1);
                 colorarray[0] = redtoBlue3[1-i][0];
                 colorarray[1] = redtoBlue3[1-i][1];
                 colorarray[2] = redtoBlue3[1-i][2];
                 divergingColor[j].add(colorarray);}
                 break;}
      case 1: {for(int i=0;i<j;i++){
                 svVector4 colorarray(1,1,1,1);
                 colorarray[0] = redtoBlue2[i][0];
                 colorarray[1] = redtoBlue2[i][1];
                 colorarray[2] = redtoBlue2[i][2];
                 divergingColor[j].add(colorarray);}
                 break;}
      }
    }
    else{
       CIEColor *cie = new CIEColor();
       for(int i=j-1;i>=0;i--){
           double t = (double)i/(double)(j-1);
           double s= 1;//0.6;
           double b=0.25;
           double nprime = 1. - 0.06 * (11.-(double)j/2.-1);
           double c =0.5;//0.88;
           double w = 0.5;//;
           double m =0.5;
           double h1=10;
           double h2=250;
           if(nprime < 0.88) c = nprime;
           vector<double> cc = cie ->DiverginghueSequential(t, s, b, c, h1, h2, w, m);
           svVector4 dc;
           dc[0] = cc[0]*255.;
           dc[1] = cc[1]*255.;
           dc[2] = cc[2]*255.;
           dc[3] = alpha;
           divergingColor[j].add(dc);
           double R,G,B;
           ColorBlindSafe(cc[0], cc[1], cc[2], R, G, B);
           dc[0] = R;
           dc[1] = G;
           dc[2] = B;
           dc[3] = alpha;
           divergingColorBlind[j].add(dc);
       }
    }
  }
}
void svColors::initDiscreteColor(){
    string infName(SRC_DIR);
    infName = infName + "/libs/quanVisLib/colorfiles/discretecolors";
    cerr<<infName<<endl;
    ifstream infile(infName.c_str());
    int colorSize;
    infile>>colorSize;
    for(int i=0;i<colorSize;i++){
      double r,g,b;
      infile>>r>>g>>b;
      svVector4 c;
      c[0]=r/255.;c[1]=g/255.;c[2]=b/255.;
      c[3]=1;
      discreteColor.add(c);
      double R, G,B;
      ColorBlindSafe(r,g,b, R,G,B);
      c[0] = R/255.;
      c[1] = G/255.;
      c[2] = B/255.;
      c[3] = alpha;
      discreteColorBlind.add(c);
    }
    infile.close();
}
void svColors::GetColors(svQDOTData * data, svVector4Array *color,
                         svVector4Array *secondColor){
 switch (color_type) {
   case c_cluster:{
    GetDiscreteColors(blind_safe, data, color);
    break;
   }
   case c_magnitude:{
    GetDivergingColors(blind_safe, data, color);
    break;
   }
   case c_texture_cluster:{
     if(secondColor!=NULL)
      GetDiscreteColors(blind_safe, data, color, secondColor);
     else
      GetDiscreteColors(blind_safe, data, color);
    break;
   }
 }
}

void svColors::SetAlpha(float alpha){
  this->alpha = alpha;
}

void svColors::GetDivergingColors(int level,
                    bool blind_safe, svVector4Array *color)
{
  int divergingIndex = level;//maxExp - minExp+1;
  if(!blind_safe){
    for(int i=0;i<level;i++){
          color[i][0] = divergingColor[divergingIndex][i][0]/255.;
          color[i][1] = divergingColor[divergingIndex][i][1]/255.;
          color[i][2] = divergingColor[divergingIndex][i][2]/255.;
          color[i][3] = alpha;//0.85;
	}
  }
  else{
    for(int i=0;i<level;i++){
          color[i][0] = divergingColorBlind[divergingIndex][i][0]/255.;
          color[i][1] = divergingColorBlind[divergingIndex][i][1]/255.;
          color[i][2] = divergingColorBlind[divergingIndex][i][2]/255.;
          color[i][3] = alpha;//0.85;
	}
  }
}

void svColors::GetDivergingColors(bool blind_safe,
                 svQDOTData* data, svVector4Array *color){
  svScalar maxExp = data->maxExp;//-9e+9;
  svScalar minExp = data->minExp;//9e+9;
  int divergingIndex = maxExp - minExp+1;
  if(!blind_safe){
    for(int i=0;i<(data->splitData).size();i++){
       for(int j=0;j<data->splitData[i].size();j++){
          Spin d = *(((data->splitData)[i])[j]);
          int den = d.exp - minExp;//cerr<<12-den+step<<endl;
          if(d.coe<1e-8) continue;
          color[i][j][0] = divergingColor[divergingIndex][den][0]/255.;
          color[i][j][1] = divergingColor[divergingIndex][den][1]/255.;
          color[i][j][2] = divergingColor[divergingIndex][den][2]/255.;
          color[i][j][3] = alpha;//0.85;
      }
    }
  }
  else{
    for(int i=0;i<(data->splitData).size();i++){
       for(int j=0;j<data->splitData[i].size();j++){
          Spin d = *(((data->splitData)[i])[j]);
          int den = d.exp - minExp;//cerr<<12-den+step<<endl;
          if(d.coe<1e-8) continue;
          color[i][j][0] = divergingColorBlind[divergingIndex][den][0]/255.;
          color[i][j][1] = divergingColorBlind[divergingIndex][den][1]/255.;
          color[i][j][2] = divergingColorBlind[divergingIndex][den][2]/255.;
          color[i][j][3] = alpha;//0.85;
      }
    }
  }
}

void svColors::GetDiscreteColors(bool blind_safe,
                 svQDOTData* data, svVector4Array *color){
//  if(data->data_structure_type == d_cluster){
     svVector4 white(1,1,1,1);
     for(int i=0;i<data->splitData.size();i++){
          for(int j=0;j<data->splitData[i].size();j++){
              int colorIndex = (*data->clusterList).at(data->splitData[i][j]);
              if(colorIndex<0) color[i][j] = white;
              //cerr<<colorIndex<<" "<<discreteColor.size()<<endl;
              if(colorIndex >= 0){
                if(blind_safe)
                 color[i][j] = discreteColorBlind[colorIndex];
                else
                 color[i][j] = discreteColor[colorIndex];
              }
          }
     }
    // cerr<<"discrete colors"<<endl;
/*  }
  else{
     for(int i=0;i<data->splitData.size();i++){
          for(int j=0;j<data->splitData[i].size();j++){
              int colorIndex = data->myCluster->clusterList.at(data->splitData[i][j]);
              if(colorIndex >= 0){
                if(blind_safe)
                 color[i][j] = discreteColorBlind[colorIndex];
                else
                 color[i][j] = discreteColor[colorIndex];
              }
          }
     }
  }
*/
}

void svColors::GetDiscreteColors(bool blind_safe, svQDOTData *data,
                                 svVector4Array *firstColor,
                                 svVector4Array *secondColor){
  //if(data->data_structure_type == d_cluster){
     for(int i=0;i<data->splitData.size();i++){
          for(int j=0;j<data->splitData[i].size();j++){
              int colorIndex = (*data->clusterList).at(data->splitData[i][j]);
              if(colorIndex >= 0){
                if(blind_safe)
                 firstColor[i][j] = discreteColorBlind[colorIndex];
                else
                 firstColor[i][j] = discreteColor[colorIndex];
                double l, a, b;
                RGB2LAB((double)firstColor[i][j][0],
                        (double)firstColor[i][j][1],
                        (double)firstColor[i][j][2],
                        l, a, b);
                l = 95;//(l+10)>99?90:(l+20);
                double R,G,B;
                LAB2RGB(l,a,b,R,G,B);
                secondColor[i][j][0] = R;
                secondColor[i][j][1] = G;
                secondColor[i][j][2] = B;
              }
          }
     }
/*  }
  else{
     for(int i=0;i<data->splitData.size();i++){
          for(int j=0;j<data->splitData[i].size();j++){
              int colorIndex = data->myCluser->clusterList.at(data->splitData[i][j]);
              if(colorIndex >= 0){
                if(blind_safe)
                 firstColor[i][j] = discreteColorBlind[colorIndex];
                else
                 firstColor[i][j] = discreteColor[colorIndex];
                double l, a, b;
                RGB2LAB((double)firstColor[i][j][0],
                        (double)firstColor[i][j][1],
                        (double)firstColor[i][j][2],
                        l, a, b);
                l = 90;
                double R,G,B;
                LAB2RGB(l,a,b,R,G,B);
                secondColor[i][j][0] = R;
                secondColor[i][j][1] = G;
                secondColor[i][j][2] = B;
              }
          }
     }
  }
*/
}

void svColors::RGB2LAB(double R, double G, double B, double &l, double &a, double &b)
{

        if(fabs(R)<0.00001
                && fabs(G)<0.00001
                && fabs(B) <0.00001)
        {
                l = 0;
                a = 0;
                b = 0;
        }
        else
        {
                double var_R = R;
                double var_G = G;
                double var_B = B;

                if ( var_R > 0.04045 ) var_R = pow( ( ( var_R + 0.055 ) / 1.055 ) , 2.4);
                else                   var_R = var_R / 12.92;
                if ( var_G > 0.04045 ) var_G = pow(( ( var_G + 0.055 ) / 1.055 ) , 2.4);
                else                   var_G = var_G / 12.92;
                if ( var_B > 0.04045 ) var_B = pow(( ( var_B + 0.055 ) / 1.055 ) , 2.4);
                else                   var_B = var_B / 12.92;

                var_R = var_R * 100;
                var_G = var_G * 100;
                var_B = var_B * 100;
                double X = var_R * 0.4124 + var_G * 0.3576 + var_B * 0.1805;
                double Y = var_R * 0.2126 + var_G * 0.7152 + var_B * 0.0722;
                double Z = var_R * 0.0193 + var_G * 0.1192 + var_B * 0.9505;

                double ref_X =  95.047  ;
                double ref_Y = 100.000;
                double ref_Z = 108.883;

                double var_X = X / ref_X;          //ref_X =  95.047   Observer= 2°, Illuminant= D65
                double var_Y = Y / ref_Y;         //ref_Y = 100.000
                double var_Z = Z / ref_Z;          //ref_Z = 108.883

                if ( var_X > 0.008856 ) var_X = pow(var_X , ( 1./3. ));
                else                    var_X = ( 7.787 * var_X ) + ( 16. / 116. );
                if ( var_Y > 0.008856 ) var_Y = pow(var_Y , ( 1./3. ));
                else                    var_Y = ( 7.787 * var_Y ) + ( 16. / 116. );
                if ( var_Z > 0.008856 ) var_Z = pow(var_Z , ( 1./3. ));
                else                    var_Z = ( 7.787 * var_Z ) + ( 16. / 116. );

                l = ( 116 * var_Y ) - 16;
                a = 500 * ( var_X - var_Y );
                b = 200 * ( var_Y - var_Z );

        }

}
void svColors::LAB2RGB(double l, double a, double b, double &R, double &G, double &B)
{
        double var_Y = ( l + 16 ) / 116;
        double var_X = a / 500 + var_Y;
        double var_Z = var_Y - b / 200;

        if ( pow(var_Y,3.) > 0.008856 ) var_Y = pow(var_Y,3.);
        else                      var_Y = ( var_Y - 16. / 116. ) / 7.787;
        if ( pow(var_X,3.) > 0.008856 ) var_X = pow(var_X, 3.);
        else                      var_X = ( var_X - 16. / 116. ) / 7.787;
        if ( pow(var_Z,3.) > 0.008856 ) var_Z = pow(var_Z,3.);
        else                      var_Z = ( var_Z - 16. / 116. ) / 7.787;

        double ref_X =  95.047;
        double ref_Y = 100.000;
        double ref_Z = 108.883;
        double X = ref_X * var_X;     //ref_X =  95.047     Observer= 2°, Illuminant= D65
        double Y = ref_Y * var_Y;     //ref_Y = 100.000
        double Z = ref_Z * var_Z;     //ref_Z = 108.883



        var_X = X / 100;        //X from 0 to  95.047      (Observer = 2°, Illuminant = D65)
        var_Y = Y / 100;        //Y from 0 to 100.000
        var_Z = Z / 100;       //Z from 0 to 108.883

        double var_R = var_X *  3.2406 + var_Y * (-1.5372) + var_Z * (-0.4986);
        double var_G = var_X * (-0.9689) + var_Y *  1.8758 + var_Z *  0.0415;
        double var_B = var_X *  0.0557 + var_Y * (-0.2040) + var_Z *  1.0570;
        if ( var_R > 0.0031308 ) var_R = 1.055 * pow( var_R , ( 1. / 2.4 ) ) - 0.055;
        else                     var_R = 12.92 * var_R;
        if ( var_G > 0.0031308 ) var_G = 1.055 * pow( var_G , ( 1. / 2.4 ) ) - 0.055;
        else                     var_G = 12.92 * var_G;
        if ( var_B > 0.0031308 ) var_B = 1.055 * pow( var_B , ( 1. / 2.4 ) ) - 0.055;
        else                     var_B = 12.92 * var_B;

        R = var_R;
        G = var_G;
        B = var_B;
}


void svColors::ColorBlindSafe(double r, double g, double b, double &r2, double &g2, double &b2)
{
        double colorl, colora, colorb;

        RGB2LAB(r,g,b,colorl,colora,colorb);


        double phimaxl = 3.1415926/4.;
        double phimaxr = 3.1415926/4.;

        double phi = 0;
        double phi_max;
        double gama;
        double tmpa = 0;
        double tmpb = 0;
        double theta;

        tmpa = colora;
        tmpb = colorb;

        if(tmpa >=0 && tmpb >=0)
        {
                gama = 0.5;
                phi_max = 3.1415926/4.;

                double c = tmpa * tmpa + tmpb * tmpb;
                c = sqrt(c);

                theta = cos(tmpa/c);
        }
        else if(tmpa >0 && tmpb <0)
        {
                gama = 2.;
                phi_max = 3.1415926/4.;

                double c = tmpa * tmpa + tmpb * tmpb;
                c = sqrt(c);

                theta =  -cos(tmpa/c);
        }
        else if(tmpa <0 && tmpb >0)
        {
                gama = 0.5;
                phi_max = (3.1415926*3.)/4.;

                double c = tmpa * tmpa + tmpb * tmpb;
                c = sqrt(c);

                theta =  (3.1415926/2. - cos(fabs(tmpa)/c)) + 3.1415926/2.;
        }
        else if(tmpa <0 && tmpb <0)
        {
                gama = 2.;
                phi_max = (3.1415926*3.)/4.;

                double c = tmpa * tmpa + tmpb * tmpb;
                c = sqrt(c);

                theta =  cos(fabs(tmpa)/c) + 3.1415926;
        }
        phi = phi_max * (1 - pow(fabs(theta)/(3.1415926/2),gama));
        colora = tmpa * cos(phi) - tmpb * sin(phi);
        colorb = tmpa * sin(phi) + tmpb * cos(phi);


        LAB2RGB(colorl, colora, colorb, r2,g2,b2);

        if(r2<0) r2=0;
        if(g2<0) g2=0;
        if(b2<0) b2=0;
        if(r2>1) r2=1;
        if(g2>1) g2=1;
        if(b2>1) b2=1;

}
/*
void svColors::RenderDivergingLegend(double power)
{
        glPushMatrix();
        glScalef(10,20,1);

        char str[50];
        float x = 1;
        float y = 1;
        float size = 1;
        float m_length_in = 1;

        glColor3f(0,0,0);

        double index;
        int numPower = divergingColor.size();


        sprintf(str, "Magnitude");
         glRasterPos2f(x ,(numPower+2)*y*(size) );
                        for(int j=0;j<strlen(str);j++)
                             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);
     for(int i=numPower-1;i>=0;i--)

        {
            glColor3f(divergingColor [i][0],
                                    divergingColor[i][1],
                                    divergingColor[i][2]);

                        glBegin(GL_QUADS);
                        glVertex2f(x,y);
                        glVertex2f(x+size,y);
                        glVertex2f(x+size,y+size);
                        glVertex2f(x,y+size);
                        glEnd();

        glColor3f(0,0,0);

                        sprintf(str,"1e%0.0f",power+i);
                        glRasterPos2f(x + size*1.1, y );
                        for(int j=0;j<strlen(str);j++)
                             glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[j]);

                        y = y + size;

        }
  glPopMatrix();

}
*/
void svColors::clean(){
   discreteColor.free();
   discreteColorBlind.free();
   for(int i=0;i<DIVERGING_SIZE;i++){
        divergingColor[i].free();
        divergingColorBlind[i].free();
   }
   delete [] divergingColor;
   delete [] divergingColorBlind;
}

}

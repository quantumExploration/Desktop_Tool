#ifndef __SV_COLORS_H
#define __SV_COLORS_H

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "svQDOTData.h"
#include "CIEColor.h"
#include <string.h>

namespace __svl_lib {

//#define CONTINUECOLOR 0
//#define DISCRETECOLOR 1

#define DISCRECT_SIZE 80
#define DIVERGING_SIZE 30

class svColors{
public:
	svColors();
	~svColors();

        void initDivergingColor();
        void initDiscreteColor();

        void SetAlpha(float alpha);
        void SetColorBlind(bool safe){blind_safe = safe;}
        void SetColorType(int type){color_type = type;}
        void GetColors(svQDOTData * data, svVector4Array *color,
                       svVector4Array *secondColor=NULL);
				void GetDivergingColors(int level, bool blind_safe,svVector4Array *color);
        //void GetColors(svQDOTData * data, svVector4Array *color){}
        void RenderDivergingLegend(vector<string> text){}

protected:
        void RGB2LAB(double R, double G, double B, double &l, double &a, double &b);
        void ColorBlindSafe(double r, double g, double b, double &r2, double &g2, double &b2);
        void HSVtoRGB( double &r, double &g, double &b, double h, double s, double v );
        void LAB2RGB(double l, double a, double b, double &R, double &G, double &B);
        void GetDivergingColors(bool blind_safe,svQDOTData *data, svVector4Array *color);
        void GetDiscreteColors(bool blind_safe,svQDOTData *data, svVector4Array *color);
        void GetDiscreteColors(bool blind_safe, svQDOTData *data, svVector4Array *color, svVector4Array *secondColor);
        void clean();

        int color_type;
        bool blind_safe;
        svScalar alpha;
        svVector4Array * divergingColor;
        svVector4Array * divergingColorBlind;
        svVector4Array discreteColor;
        svVector4Array discreteColorBlind;

};
}


#endif // __SV_COLORS_H

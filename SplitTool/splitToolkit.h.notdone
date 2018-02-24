#include "dual_depth_peeling.h"

#include <GL/glext.h>
#ifndef  GL_RG32F
#define GL_RG32F 0x8230
#endif

#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <string.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string.h>


#include "svQDOT.h"
#include "svDirectArrow.h"
#include "svSplitArrow.h"
#include "svSummaryGlyph.h"
#include "svUtil.h"
#include "ivTrackball.h"
#include "ivview.h"
#include "MGL.h"
#include "svOutline.h"
#include "svMesh.h"
#include "svConfig.h"
#include "svWidget.h"
#include "svCCL.h"
#include "svIcons.h"
#include "svColors.h"
#include "svContourImageDrawing.h"
#include "parser.h"
#include "svQDOTSlider.h"

#include <GL/glui.h>

#define ENCODE 0
#define SOLID_COLOR 1
#define SYMMETRY_COLOR 2

#define SPLITVECTORS_SCALE 0.5
#define LINEAR_SCALE  50000
#define ARROW_SCALE 0.1
#define SUMMARY_ARROW_SCALE 0.75
#define TUBE_SCALE 0.05
#define SUMMARY_TUBE_SCALE 0.325

#define SYMMETRY_CHECK 1
#define SYMMETRY_UNCHECK 0
#define SYMMETRY_UP 0
#define SYMMETRY_DOWN 1
#define SYMMETRY_UP_SELECT 2
#define SYMMETRY_DOWN_SELECT 3
#define SYMMETRY_TYPE_NUM 8
#define SYMMETRY_STATUS_NUM 4
#define CLUSTER_DIMENSION 7

#define VISUAL_TYPE 3
#define LINEAR 0
#define LC 1
#define LT 2
#define LINEAR_TUBE 3
#define LLO 4
#define LC_TUBE 5

#define SYMMETRY_ID 1
#define OVERVIEW_ID 2
#define LENGTH_ID 3
#define ARROW_ID 4
#define TUBE_ID 5
#define FREQ_ID 6
#define MESH_ID 7
#define MESH_TYPE_ID 8
#define MESH_VIS_ID 9
#define MAG_ID 10
#define LAYER_NUM_ID 11
#define LAYER_ID 12
#define UPDATE_ID 14
#define ENCODE_ID 13
#define ALPHA_ID 15
#define LAYER_REPEAT_ID 16
#define LENGTH_VIS_ID 17
#define PICTURE_ID 18
#define PICTURE_TEXT_ID 19
#define COLORBY_ID 20
#define COLORBY_CLUSTER_ID 21
#define COLORBY_SYMMETRY_ID 22
#define ENCODE_LINEAR_ID 23
#define ENCODE_SPLIT_ID 24
#define CONFIG_ID 25
#define DATA_CONTOUR_ID 26
#define DATA_RAW_ID 27
#define CONTOUR_TREE_ID 28
#define DATA_ID 29
#define CONTOUR_GR_ID 30
#define CONTOUR_GR_EDIT_ID 31
#define CONFIG_TEXT_ID 32
#define CONTOUR_BACK_ID 33
#define CUTOFF_ID 34
#define MAG_WIDGET_ID 35
#define VEL_WIDGET_ID 36
#define SELECT_MAP_ID 37

#define ENCODE_LINEAR_TUBE_ID 123

#define IMAGE_WIDTH 1900
#define IMAGE_HEIGHT 1000

void  getTime(char *buffer)
{
         time_t rawtime;
         struct tm * timeinfo;

                        time (&rawtime);
                        timeinfo = localtime (&rawtime);

                        sprintf(buffer,"%s", asctime(timeinfo));

}


void CaptureScreen(char *file, int startw, int starth, int w, int h)
{
  char *image_buf = new char [w*h*3];
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadPixels(startw, starth, w, h, GL_RGB, GL_UNSIGNED_BYTE, image_buf);

  FILE* fp;
  if (NULL != (fp = fopen(file, "wb"))){
    fprintf(fp, "P6 %d %d 255\n", w, h);
    for (int i=h-1; i >= 0; --i)
    {
       fwrite(image_buf+3*i*w, sizeof(unsigned char), 3*w, fp);
    }
    fclose(fp);
  }
  delete [] image_buf;
}

//=========Camera=================
int ACSIZE=4;
typedef struct
{
        GLfloat x, y;
} jitter_point;

#define MAX_SAMPLES  66

/* 8 jitter points */
jitter_point j8[] =
{
        {-0.334818,  0.435331},
        { 0.286438, -0.393495},
        { 0.459462,  0.141540},
        {-0.414498, -0.192829},
        {-0.183790,  0.082102},
        {-0.079263, -0.317383},
        { 0.102254,  0.299133},
        { 0.164216, -0.054399}
};

void accFrustum(GLdouble left, GLdouble right, GLdouble bottom,
   GLdouble top, GLdouble near, GLdouble far, GLdouble pixdx,
   GLdouble pixdy, GLdouble eyedx, GLdouble eyedy, GLdouble focus);
void accPerspective(GLdouble fovy, GLdouble aspect,
   GLdouble near, GLdouble far, GLdouble pixdx, GLdouble pixdy,
   GLdouble eyedx, GLdouble eyedy, GLdouble focus);
void Reshape(int w, int h);


//=========Light==================
void ArrowLight();
void InitLight();

//=========Render Routine=========
void Init();
void DrawSymmetryButtons(void);
void Render2DContour();
void accDisplay(void);
void InitDualPeelingRenderTargets();
void DeleteDualPeelingRenderTargets();
void BuildShaders(string SHADER_PATH);
void MakeFullScreenQuad();
void RenderDualPeeling();
void InitGL(string shader);

//=========Update=================
void Update2DColor();
void UpdateColor();
void UpdateCluster();
void UpdateRender();
void UpdateContour();
void UpdateImageBack();
void UpdateImage();
void UpdateSymmetry();
void UpdateSymmetryMesh();
void UpdateVisible();
void UpdateSingleContour();
void Update();

//=========GLUI===================
void idle();
void control_cb(int control);
void ComputeGLUIFactor();
void glui_display();

//=========Init===================
void InitField();
void ReadConfig(char *inputfile);
void LocateTexture();
void LoadTexture();
void initContours();
void initRaw();
void init(char *configfname);


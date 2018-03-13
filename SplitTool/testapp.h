//--------------------------------------------------------------------------------------
// Order Independent Transparency with Dual Depth Peeling
//
// Author: Louis Bavoil
// Email: sdkfeedback@nvidia.com
//
// Depth peeling is traditionally used to perform order independent transparency (OIT)
// with N geometry passes for N transparency layers. Dual depth peeling enables peeling
// N transparency layers in N/2+1 passes, by peeling from the front and the back
// simultaneously using a min-max depth buffer. This sample performs either normal or
// dual depth peeling and blends on the fly.
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#pragma warning( disable : 4996 )

//henan
//#include <nvModel.h>

#include "GLSLProgramObject.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <assert.h>
#include <string>
#include <cstring>
#include "svConfig.h"
#include "ivTrackball.h"
#include "svQDOTData.h"
#include "svMesh.h"
#include "svSlice.h"
#include "svLCArrow.h"
#include "svLinearArrow.h"
#include "svLTArrow.h"
#include "svImageList.h"
#include "svSymmetryButtons.h"
#include "svMouseInteraction.h"
#include "svOutline.h"

#include <GL/glui.h>

using namespace __svl_lib;

#define LINEAR_SCALE 1000000

#define MAX_DEPTH 1.0
int g_numPasses = 4;
int g_imageWidth = 1920;//500;//1024;
int g_imageHeight = 1080;//500;//768;
GLuint g_quadDisplayList;
GLuint g_vboId;
GLuint g_eboId;
bool g_useOQ = false;
GLuint g_queryId;
GLSLProgramObject g_shaderDualInit;
GLSLProgramObject g_shaderDualPeel;
GLSLProgramObject g_shaderDualBlend;
GLSLProgramObject g_shaderDualFinal;
float g_opacity = 1;
unsigned g_numGeoPasses = 0;
float g_white[3] = {0.75,0.75,0.75};
float g_black[3] = {0.0};
float *g_backgroundColor = g_white;
GLuint g_dualBackBlenderFboId;
GLuint g_dualPeelingSingleFboId;
GLuint g_dualDepthTexId[2];
GLuint g_dualFrontBlenderTexId[2];
GLuint g_dualBackTempTexId[2];
GLuint g_dualBackBlenderTexId;
GLenum g_drawBuffers[] = {GL_COLOR_ATTACHMENT0_EXT,
						   GL_COLOR_ATTACHMENT1_EXT,
						   GL_COLOR_ATTACHMENT2_EXT,
						   GL_COLOR_ATTACHMENT3_EXT,
						   GL_COLOR_ATTACHMENT4_EXT,
						   GL_COLOR_ATTACHMENT5_EXT,
						   GL_COLOR_ATTACHMENT6_EXT
};
#define CHECK_GL_ERRORS {}
float alpha = 0.6;
void MakeFullScreenQuad();
void InitDualPeelingRenderTargets();
void DeleteDualPeelingRenderTargets();
void BuildShaders(string SHADER_PATH);
void InitGL(string shader);
void RenderDualPeeling();

//svVector3 eye;//(0,0,100);
//GLfloat m[16];
ViewProperty viewproperty;
Trackball trackball;

svMesh *meshRender;
svLCArrow *lc;
svLTArrow *lt;
svLinearArrow *linear;
svLCArrow *lc_contour;
svLTArrow *lt_contour;
svLinearArrow *linear_contour;
svColors *myColor;
svImageList *mySlice;
svQDOT *rawData;
svRawSliceData *sliceData;
svRawSymmetryData *symmetryData;
svRawClusterData *clusterData;
svContourData *contourData;
svContourClusterData *contourClusterData;
svMeshData *meshData;
svMeshData *meshCluster;
svMesh *meshClusterRender;
svWidget *widget;
svSlice *slice;
svROISlider *slider;
svOutline *outline;
svSymmetryButtons *button;
State *myState;
svMouseInteraction *mouseHandle;
svMouseGroup *mouseGroup;

//==========GLUI===========//
int window;
GLUI *glui;
//data panel
GLUI_StaticText *glui_data;
//slice panel
GLUI_Checkbox *glui_show_slice;
GLUI_TextBox *glui_slice_box;
string sliceText;
GLUI_EditText *(glui_slice_pos[3]);
GLUI_EditText *(glui_slice_dir[3]);
GLUI_EditText *(glui_slice_d);
//contour panel
GLUI_Checkbox *glui_show_contour;
GLUI_TextBox *glui_contour_box;
string contourText;
GLUI_RadioGroup *glui_contour_type;
GLUI_EditText *glui_contour_value;
//symmetry panel
GLUI_Checkbox *glui_show_symmetry;
GLUI_TextBox *glui_symmetry_box;
string symmetryText;
GLUI_EditText *(glui_symmetry_pos[3]);
GLUI_EditText *(glui_symmetry_dir[3]);
//cluster panel
GLUI_Checkbox *glui_cluster_split;
GLUI_TextBox *glui_cluster_box1;
string clusterText1;
GLUI_EditText *glui_cluster_num;
GLUI_EditText *(glui_cluster_weight[7]);
GLUI_Checkbox *glui_roi_cluster;
GLUI_TextBox *glui_cluster_box2;
string clusterText2;
GLUI_EditText *glui_roi_cluster_num;
GLUI_EditText *(glui_roi_cluster_weight[7]);
//update
GLUI_Button *glui_apply;
GLUI_Button *glui_update;
GLUI_Button *glui_vis_update;
//ROI panel
//image panel
GLUI_Checkbox *glui_show_image;
GLUI_RadioGroup *glui_image_type;
GLUI_Scrollbar *glui_image_percentage_s;
GLUI_EditText *glui_image_percentage_t;
GLUI_RadioGroup *glui_image_interaction;
//widget panel
GLUI_Checkbox *glui_show_widget;
GLUI_Checkbox *glui_widget_repeat;
//symmetry button
GLUI_Checkbox *glui_show_button;
//slider
GLUI_Checkbox *glui_show_slider;
//3D panel
GLUI_Checkbox *glui_show_glyph;
GLUI_RadioGroup *glui_glyph_type;
GLUI_RadioGroup *glui_color_type;
GLUI_Checkbox *glui_show_qdot;
GLUI_Checkbox *glui_show_csurface;//cluster surface
GLUI_Scrollbar *glui_glyph_length;
GLUI_Scrollbar *glui_glyph_width;
//Sampling panel
GLUI_Checkbox *glui_keep_cluster;
GLUI_Checkbox *glui_keep_qdot;
GLUI_Scrollbar *glui_sample;
GLUI_EditText *glui_sample_box;
GLUI_Button *glui_screenshot;

enum GLUI_ID{
  GLUI_SLICE_POS_ID0,
  GLUI_SLICE_POS_ID1,
  GLUI_SLICE_POS_ID2,
  GLUI_SLICE_DIR_ID0,
  GLUI_SLICE_DIR_ID1,
  GLUI_SLICE_DIR_ID2,
  GLUI_SLICE_DISTANCE_ID,
  GLUI_SLICE_TEXT_ID,
  GLUI_SHOW_CONTOUR_ID,
  GLUI_SHOW_SLICE_ID,
  GLUI_SHOW_SYMMETRY_ID,
  GLUI_CONTOUR_TEXT_ID,
  GLUI_CONTOUR_TYPE_ID,
  GLUI_CONTOUR_VALUE_ID,
  GLUI_SYM_TEXT_ID,
  GLUI_SYM_POS_ID0,
  GLUI_SYM_POS_ID1,
  GLUI_SYM_POS_ID2,
  GLUI_SYM_DIR_ID0,
  GLUI_SYM_DIR_ID1,
  GLUI_SYM_DIR_ID2,
  GLUI_SYM_X_ID0,
  GLUI_SYM_X_ID1,
  GLUI_SYM_X_ID2,
  GLUI_SYM_Y_ID0,
  GLUI_SYM_Y_ID1,
  GLUI_SYM_Y_ID2,
  GLUI_CLUSTER_TEXT_ID0,
  GLUI_CLUSTER_TEXT_ID1,
  GLUI_CLUSTER_SPLIT_ID,
  GLUI_CLUSTER_NUM_ID,
  GLUI_CLUSTER_WEIGHT_ID0,
  GLUI_CLUSTER_WEIGHT_ID1,
  GLUI_CLUSTER_WEIGHT_ID2,
  GLUI_CLUSTER_WEIGHT_ID3,
  GLUI_CLUSTER_WEIGHT_ID4,
  GLUI_CLUSTER_WEIGHT_ID5,
  GLUI_CLUSTER_WEIGHT_ID6,
  GLUI_ROI_CLUSTER_ID,
  GLUI_ROI_CLUSTER_NUM_ID,
  GLUI_ROI_CLUSTER_WEIGHT_ID0,
  GLUI_ROI_CLUSTER_WEIGHT_ID1,
  GLUI_ROI_CLUSTER_WEIGHT_ID2,
  GLUI_ROI_CLUSTER_WEIGHT_ID3,
  GLUI_ROI_CLUSTER_WEIGHT_ID4,
  GLUI_ROI_CLUSTER_WEIGHT_ID5,
  GLUI_ROI_CLUSTER_WEIGHT_ID6,
  GLUI_UPDATE_ID,
  GLUI_APPLY_ID,
  GLUI_SHOW_IMAGE_ID,
  GLUI_IMAGE_TYPE_ID,
  GLUI_IMAGE_INTERACTION_ID,
  GLUI_IMAGE_PERCENTAGE_T_ID,
  GLUI_SHOW_WIDGET_ID,
  GLUI_WIDGET_REPEAT_ID,
  GLUI_SHOW_BUTTON_ID,
  GLUI_SHOW_SLIDER_ID,
  GLUI_SHOW_GLYPH_ID,
  GLUI_GLYPH_TYPE_ID,
  GLUI_GLYPH_COLOR_ID,
  GLUI_GLYPH_WIDTH_ID,
  GLUI_GLYPH_LENGTH_ID,
  GLUI_SHOW_QDOT_ID,
  GLUI_SHOW_CSURFACE_ID,
  GLUI_SAMPLE_ID,
  GLUI_VIS_UPDATE_ID,
  GLUI_SCREENSHOT_ID
};
///===========GLUI done=============//


//only for adding contours in contour tree;
typedef struct contourInfo
{
//  int contourstep;
//  bool contourratio;
  float value;
  int type;
  int showcontour;
} contourInfo;

enum glyphtype
{
  linearencode,
  splitvectorsencode
};

int outline_display_list;

typedef struct glyphInfo
{
  int glyph;
  int type;
  int color;
  float arrowradius;
  float tuberadius;
  float length;
  int encodetype;
  int sample; 
  int zmin;
  int zmax;
} glyphInfo;

//replace 
typedef struct enableInfo
{
  bool enableMesh;
  bool enableClusterBoundary;
} enableInfo;

enum GUITYPE
{
  gui_widget,
  gui_button,
  gui_slice,
  gui_slider,
  gui_none
};

typedef struct guiInfo
{
  int left;
  int guitype;
  int slice;
  int slider;
  int button;
  int widget;
  int widgetrepeat;
} guiInfo;

typedef struct imageInfo
{
  float percentage;
  int imageMouseType;
  int imageType;//magnitdue_image, cluster_image
} imageInfo;

typedef struct dataInfo
{
  int type;
  string dir;
  string tmpdir;
  string name;
} dataInfo;

svScalar sliceside;
typedef struct sliceInfo
{
  int showslice;
  svVector3 pos;
  svVector3 dir;
  svScalar distance;
}sliceInfo;

typedef struct symInfo
{
  int showsymmetry;
  svVector3 pos;
  svVector3 dir;
  svVector3 x;  
  svVector3 y;
}symInfo;

typedef struct clusterInfo
{
  int split;
  int num;
  float weight[7];
  int roinum;
  int roicluster;
  float roiweight[7];  
}clusterInfo;

int enableQDOT = 0;
int enableSurface;

ROIProperty *roip1;
ROIProperty *roip2;

bool updateGlyph = false;
bool updateColor = false;
bool updateImage = false;

int testcluster;

void    GetSliceText();
void   GetSymmetryText();
void   GetContourText();
void   GetClusterText();


void   SetSymmetryState();
void  SetClusterState();
void  SetContourState();

void SetSliceText();
void GetSliceText();

void UpdateContourCluster();
void UpdateRender();
void UpdateSlice();
void UpdateSymmetry();
void UpdateCluster();
void UpdateContour();
void ResetROI();
void Update();
void control_cb(int control);
void glui_display();

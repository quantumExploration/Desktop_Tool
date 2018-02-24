#include "dual_depth_peeling.h"
#include <GL/glext.h>
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

#include "dataApi.h"
#include "parser.h"
#include "svLTArrow.h"
#include "svLCArrow.h"
#include "svMesh.h"
#include "svOutline.h"
#include "svContourImageDrawing.h"

using namespace __svl_lib;

#define CHECK_GL_ERRORS {}


enum RenderType{
   RENDER_TYPE_LC = 1; //color + magnitude
   RENDER_TYPE_LT = 2; //texture + cluster
   RENDER_TYPE_MIX = 3; //texture + color
   RENDER_TYPE_CLUSTER1 = 4;//uniform; color+texture
   RENDER_TYPE_CLUSTER2 = 0;//linear; color+texture
};

GLenum g_drawBuffers[] = {GL_COLOR_ATTACHMENT0_EXT,
                                                   GL_COLOR_ATTACHMENT1_EXT,
                                                   GL_COLOR_ATTACHMENT2_EXT,
                                                   GL_COLOR_ATTACHMENT3_EXT,
                                                   GL_COLOR_ATTACHMENT4_EXT,
                                                   GL_COLOR_ATTACHMENT5_EXT,
                                                   GL_COLOR_ATTACHMENT6_EXT
};

class renderApi{
public:
  renderApi(){initParameter();}
  ~renderApi(){cleanup();}

  void setPaser(Paser *paser){this->paser = paser;}

  void initParameter();
  void init(dataApi *raw, dataApi *data);

  void generateColor();
  void generateShape();
  void updateVBO();
  void updateVisible();

  void render();
  void Reshape(int w, int h);

/*=============lighting==============*/
  void ArrowLight();
  void InitLight();
/*=============lighting end==========*/

/*===========glyph==================*/
  void UpdateVis();
  void UpdateRender();
  void UpdateVisible();
/*============glyph end==============*/

/*===========hyperslice==============*/
  void Reshape2DContour();
  void UpdateSingleContour();
  void Update2DContour();
  void Update2DColor();
  void Update2D();
  void imageMovement();
/*===========hyperslice end============*/

/*===========depth dual peeing========*/
  void InitDualPeelingRenderTargets();
  void InitGL(string shader);
  void RenderDualPeeling();
  void DeleteDualPeelingRenderTargets();
/*===========depth end===============*/

/*==========accumulation=============*/
  void InitAccumulationRenderTargets();
  void DeleteAccumulationRenderTargets();
  void RenderAverageColors();
/*=========accumulation end=========*/

protected: 
  void MakeFullScreenQuad();
  void BuildShaders(string SHADER_PATH);
  void DestroyShaders();

  void cleanup();
private:
  int renderType;

  int image_width;
  int image_height; 
  
  float alpha;//0.75;
  vVector4 blackcolor;
  svScalar magthreshold;
  svInt contour_layer;
  svInt contour_index;
  svScalar contour_mag;
  vector<int> symmetrytype;

//image parameters
  svInt volumebound[2]=;
  svInt volumestep;
  svInt vbound;
  float imageCutoff;
  int image_lbx;
  int image_lby;
  int image_rbx;
  int image_rby;
  bool image_inside;

  Paser *paser;
  svQDOTData *field;
  svLTArrow *lt;
  svLCArrow *lc; 
  svMesh *mesh;
  svOutline *outline;
  svContourImageDrawing *image;

//peeling
int g_numPasses;
int g_imageWidth;
int g_imageHeight;
nv::Model *g_model;
GLuint g_quadDisplayList;
GLuint g_vboId;
GLuint g_eboId;
bool g_useOQ;//true;//true;
GLuint g_queryId;
static nv::SDKPath sdkPath;
GLSLProgramObject g_shaderDualInit;
GLSLProgramObject g_shaderDualPeel;
GLSLProgramObject g_shaderDualBlend;
GLSLProgramObject g_shaderDualFinal;
GLSLProgramObject g_shaderAverageInit;
GLSLProgramObject g_shaderAverageFinal;
float g_opacity;
bool g_showOsd;
bool g_bShowUI;
unsigned g_numGeoPasses;
int g_rotating;
int g_panning;
int g_scaling;
int g_oldX, g_oldY;
int g_newX, g_newY;
float g_bbScale;
nv::vec3f g_bbTrans;
nv::vec2f g_rot;
nv::vec3f g_pos;
float g_white[3];
float g_black[3];
float g_backgroundColor[3];
GLuint g_dualBackBlenderFboId;
GLuint g_dualPeelingSingleFboId;
GLuint g_dualDepthTexId[2];
GLuint g_dualFrontBlenderTexId[2];
GLuint g_dualBackTempTexId[2];
GLuint g_dualBackBlenderTexId;
GLuint g_frontFboId[2];
GLuint g_frontDepthTexId[2];
GLuint g_frontColorTexId[2];
GLuint g_frontColorBlenderTexId;
GLuint g_frontColorBlenderFboId;
GLuint g_accumulationTexId[2];
GLuint g_accumulationFboId;

};


};



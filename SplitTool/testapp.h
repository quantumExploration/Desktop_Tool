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
#include <string.h>
#include "svConfig.h"
#include "ivTrackball.h"
#include "svQDOTData.h"
#include "svMesh.h"
#include "svLCArrow.h"
#include "svLTArrow.h"
#include "svImageList.h"
#include "svWidget.h"

using namespace __svl_lib;

#define MAX_DEPTH 1.0
int g_numPasses = 4;
int g_imageWidth = 500;//1024;
int g_imageHeight = 500;//768;
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
svColors *myColor;
svImageList *mySlice;
svQDOT *rawData;
svRawSliceData *sliceData;
svRawSymmetryData *symmetryData;
svRawClusterData *clusterData;
svContourData *contourData;
svContourClusterData *contourClusterData;
svMeshData *meshData;
svWidget *widget;
State *myState;

//only for adding contours in contour tree;
typedef struct contourInfo
{
  int contourstep;
  bool contourratio;
} contourInfo;

typedef struct glyphInfo
{
  float arrowradius;
  float tuberadius;
  int encodetype;
  int sample; 
  int zmin;
  int zmax;
} glyphInfo;

typedef struct enableInfo
{
  bool enableMesh;
  bool enableClusterBoundary;
} enableInfo;


typedef struct guiInfo
{
  int left;
} guiInfo;

#pragma warning( disable : 4996 )

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
#include "svConfig.h"
#include "ivTrackball.h"
#include "svLTArrow.h"
#include "svLCArrow.h"
#include "parser.h"
#include "dataApi.h"
#include "svMesh.h"
#include "svOutline.h"
#include "svContourImageDrawing.h"

using namespace __svl_lib;

#define FOVY 30.0
#define ZNEAR 0.1//0.0001
#define ZFAR 10.0
#define FPS_TIME_WINDOW 1
#define MAX_DEPTH 1.0

int image_height =1000;
int image_width = 1000;
svVector3 eye;//(0,0,100);


GLfloat m[16];
int image_lbx;
int image_lby;
int image_rbx;
int image_rby;
bool image_inside = false;
int movement_x=0;
int movement_y=0;
int movement_stop_x=0;
int movement_stop_y=0;
int movement_times=-1;


ViewProperty viewproperty;

Paser *paser;
svVector3 lb, rb;

Trackball trackball;
svLTArrow *lt;
svLCArrow *lc;
svMesh *mesh;
svOutline *outline;
svContourImageDrawing *image;


dataApi *raw;
dataApi *contour;

float alphainside = 0.75;//0.5;
float alphaoutside = 0.75;//0.1;
float alpha = 0.75;
svVector4 blackcolor(0,0,0,0.0);
svInt volumebound[2]={0,100};
svInt volumestep = 5;
svInt vbound = 0;
svScalar magthreshold = 0;
svInt contour_layer;
svInt contour_index;
svScalar contour_mag;
vector<int> symmetrytype;
float imageCutoff = 10.;



void readConfig(char *inputfile){
  paser->ReadFiles(inputfile);
  raw->init(paser->GetRawDir(), paser->GetRawFile(), paser->GetStoreDir(),
              paser->GetPlaneCenter(), paser->GetPlaneVector(),
              paser->GetPlaneDistance(), paser->GetFormat());
cerr<<"raw"<<endl;
  contour->init(paser->GetRawDir(), paser->GetRawFile(), paser->GetStoreDir(),
              paser->GetPlaneCenter(), paser->GetPlaneVector(),
              paser->GetPlaneDistance(), paser->GetFormat());
cerr<<"contour"<<endl;
  svVector3 c = raw->getPlaneC();
  paser->SetSeedNum(raw->getSeed());
  paser->SetPlane(raw->getPlaneD(),raw->getPlaneC());
  char *str = new char[400];
  sprintf(str, "%s/%s/density%s", paser->GetStoreDir(), paser->GetRawFile(), paser->GetPlaneName());
cerr<<str<<endl;
  paser->ProcessContour();
cerr<<1<<endl;
  paser->SetContourValues(raw->getContourValues(str));
cerr<<2<<endl;
  paser->ProcessClusterLayers();
  paser->ResetBool();
  delete [] str;
}

void init(){
  raw = new dataApi();
  contour = new dataApi();  
  paser = new Paser();
  outline = new svOutline();
}


void setData(){
cerr<<"setdata"<<endl;
  //raw->init(paser->GetRawDir(), paser->GetRawFile(), paser->GetStoreDir(),
  //         paser->GetPlaneCenter(),
  //        paser->GetPlaneVector(),
 //         paser->GetPlaneDistance(),
 //         paser->GetFormat());
 // contour->init(paser->GetRawDir(), paser->GetRawFile(), paser->GetStoreDir(),
   //        paser->GetPlaneCenter(),
     //     paser->GetPlaneVector(),
       //   paser->GetPlaneDistance(),
         // paser->GetFormat());
  contour->setContourData(paser->GetContourProperty());
cerr<<"contour"<<endl;                      //    paser->GetSymmetryProperty());
  raw->setRawData(paser->GetStoreDir(), paser->GetRawFile(),
                  paser->GetSymmetryProperty());
//  contour->setCluster(paser->GetMagRange00(),paser->GetMagRange10(),
  //                    paser->GetMagRange01(),paser->GetMagRange11()
    //                  paser->GetStep1Property(), paser->GetStep2Property(),
      //                paser->GetSymmetryProperty(), symmetrytype);
  raw->setCluster(paser->GetMagRange00(),paser->GetMagRange10(),
                      paser->GetMagRange01(),paser->GetMagRange11(),
                      paser->GetStep1Property(), paser->GetStep2Property(),
                      paser->GetSymmetryProperty(), symmetrytype);
//  contour->setContourVariables(raw->getCluster(), raw->getAtom(),
//                              raw->getRegion(), raw->getPos());
//  raw->testvis();
  contour->setContourCluster(paser->GetMagRange00(),paser->GetMagRange10(),
                      paser->GetMagRange01(),paser->GetMagRange11(),
                      paser->GetStep1Property(), paser->GetStep2Property());

    raw->getBoundary(lb,rb);

}

void Reshape2DContour()
{
   int rx, ry, rw, rh;
   rx = 0;
   ry = 0;
   rw = image_width;
   rh = image_height;
   //GLUI_Master.get_viewport_area(&rx, &ry, &rw, &rh);
   rx = image_width - 300;
   rw = 300.;
   image->Reshape(50,rw,rh,rx,ry);//hard code!!!!!!!!!
   image->SetMinD();
   image->SetContourPosition();
   image->getImageSize(image_lbx, image_lby, image_rbx, image_rby);
}

void UpdateContour(){
   image->SetSideX(50.);
   image->ResetContourTree();cerr<<"1"<<endl;
   image->New();cerr<<"1"<<endl;
   image->GeneratePixels();cerr<<"1"<<endl;
   image->SetBound(volumebound[0], volumebound[1], volumestep);cerr<<"1"<<endl;
   image->GenerateContourTree(paser->GetContourTreeProperty());cerr<<"2"<<endl;
   image->GenerateContours(paser->GetContourTreeDir(),
                             paser->GetPlaneName());cerr<<"3"<<endl;
   image->GenerateSelectedContours(paser->GetContourProperty());cerr<<"4"<<endl;
   image->GenerateVisibility(vbound, magthreshold);cerr<<"5"<<endl;
   Reshape2DContour();cerr<<"6"<<endl;
   image->SetContourPosition();cerr<<"7"<<endl;

   char *str = new char[400];
   sprintf(str, "%s/%s/2dcontour.txt", paser->GetStoreDir(),paser->GetRawFile());
   image->SaveContourtoFile(str);cerr<<"8"<<endl;
  delete [] str;

  Reshape2DContour();
}

void Update2DColor()
{
  char *str = new char[400];
  sprintf(str, "%s/%s/2dcolor.txt", paser->GetStoreDir(),paser->GetRawFile());
  cerr<<str<<endl;
  if(paser->GetIsContour())//hard code!!
  image->SaveColortoFile(str, lc->GetColors());
  else
  image->SaveColortoFile(str, lt->GetColors());
  delete [] str;
}

 
void UpdateImage()
{
  char *str = new char[400];
  sprintf(str, "%s/%s/2dimage.txt", paser->GetStoreDir(),paser->GetRawFile());

  char *str1 = new char[400];
  sprintf(str1, "%s/%s/2dcolor.txt", paser->GetStoreDir(),paser->GetRawFile());
  char *str2 = new char[400];
  sprintf(str2, "%s/%s/2dcontour.txt", paser->GetStoreDir(),paser->GetRawFile());
  image->GenerateColors(str1, str2, imageCutoff, str);
  image->SetColors(str);
  delete [] str1;
  delete [] str2;
  delete [] str;
}

void UpdateVis(){
  lt->UpdateIndex();cerr<<"update index"<<endl;
  lt->GenerateIndex();cerr<<"index"<<endl;
  lt->UpdateIndexVBO();cerr<<"ivbo"<<endl;
}

void UpdateRender(){
  lt->UpdateData();
  lt->UpdateColor();
  lt->SetColorByClusterMag();
  lt->UpdateIndex();cerr<<"update index"<<endl;
  lt->GenerateTubes();cerr<<"tube"<<endl;
  lt->GenerateArrows();cerr<<"arrow"<<endl;
  lt->GenerateLegend();cerr<<"legend"<<endl;
  lt->GenerateIndex();cerr<<"index"<<endl;
  lt->UpdateTubeVBO();cerr<<"tvbo"<<endl;
  lt->UpdateArrowVBO();cerr<<"avbo"<<endl;
  lt->UpdateIndexVBO();cerr<<"ivbo"<<endl;

}
void UpdateVisible(){
//  contour->testvis(contour_layer, contour_index, lb, rb);cerr<<contour_layer<<" "<<contour_index<<endl;
  raw->testvis(contour_layer, contour_index, lb, rb);cerr<<contour_layer<<" "<<contour_index<<endl;
  UpdateVis();
//  Update2DColor();
//  UpdateImage();
}
void UpdateSingleContour(){
  image->GenerateContour(paser->GetContourTreeDir(),
                            paser->GetPlaneName());
   char *str = new char[400];
   sprintf(str, "%s/%s/2dcontour.txt", paser->GetStoreDir(),paser->GetRawFile());
   image->SaveContourtoFile(str);cerr<<"8"<<endl;
  delete [] str;
  UpdateImage();
cerr<<"image"<<endl;
  contour->setSingleContour(paser->GetContourTreeDir(),
                            paser->GetPlaneName(),
                            paser->GetContourProperty(),
                            contour_layer, contour_mag);
cerr<<"contour"<<endl;
  image->SetContourPosition();
cerr<<"position"<<endl;
  contour->resetVisible();
cerr<<"visible"<<endl;
  contour->setContourCluster(paser->GetMagRange00(),paser->GetMagRange10(),
                      paser->GetMagRange01(),paser->GetMagRange11(),
                      paser->GetStep1Property(), paser->GetStep2Property());
cerr<<"cluster"<<endl;
  UpdateRender();
}



void ArrowLight()
{
  GLfloat mat_ambient[] = {1,1,1,1};
  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, .8};
  GLfloat mat_specular[] = { 0.2,0.2,0.2,0.2 };
  GLfloat mat_shininess[] = { 0.4 };
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void InitLight()
{
  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8,1};
  GLfloat mat_specular[] = { 0.2,0.2,0.2,0.2 };
  GLfloat light_position[] = {eye[0], eye[1], eye[2],0};//100,100,200, 0 };
  GLfloat white_light[] = { 0.8, 0.8, 0.8, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

  GLfloat lmodel_ambient[] = {0.2,0.2,0.2,1.0};
  GLfloat local_view[] = {0.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);


  GLfloat light1_position[] = {100,100,200,1};//eye[0], eye[1], eye[2],0};// 0,0,100,0};
  GLfloat light1_ambient[] = {0.2,0.2,0.2,1};
  GLfloat light1_diffuse[] = {0.5,0.5,0.5,1.0};
  GLfloat light1_specular[] = {0.5,0.5,0.5,1.0};
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR,light1_specular);

  ArrowLight();
}

//this is silver material
/*void InitLight(){
  GLfloat mat_diffuse[] = { 0.5, 0.5, 0.5, 0.4};
  GLfloat mat_specular[] = { 1,1,1,1};
  GLfloat mat_shininess[] = { 0.4 };
  GLfloat light_position[] = { 50,50,50, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
}
*/
//====================
int g_numPasses = 4;
int g_imageWidth = 1024;
int g_imageHeight = 768;

nv::Model *g_model;
GLuint g_quadDisplayList;
GLuint g_vboId;
GLuint g_eboId;

bool g_useOQ = false;//true;//true;
GLuint g_queryId;

static nv::SDKPath sdkPath;

GLSLProgramObject g_shaderDualInit;
GLSLProgramObject g_shaderDualPeel;
GLSLProgramObject g_shaderDualBlend;
GLSLProgramObject g_shaderDualFinal;

GLSLProgramObject g_shaderFrontInit;
GLSLProgramObject g_shaderFrontPeel;
GLSLProgramObject g_shaderFrontBlend;
GLSLProgramObject g_shaderFrontFinal;

GLSLProgramObject g_shaderAverageInit;
GLSLProgramObject g_shaderAverageFinal;

GLSLProgramObject g_shaderWeightedSumInit;
GLSLProgramObject g_shaderWeightedSumFinal;

float g_opacity = 0.6;
char g_mode = DUAL_PEELING_MODE;
bool g_showOsd = true;
bool g_bShowUI = true;
unsigned g_numGeoPasses = 0;

int g_rotating = 0;
int g_panning = 0;
int g_scaling = 0;
int g_oldX, g_oldY;
int g_newX, g_newY;
float g_bbScale = 1.0;
nv::vec3f g_bbTrans(0.0, 0.0, 0.0);
nv::vec2f g_rot(0.0, 45.0);
nv::vec3f g_pos(0.0, 0.0, 2.0);

float g_white[3] = {1.0,1.0,1.0};
float g_black[3] = {0.0};
float *g_backgroundColor = g_white;

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

GLenum g_drawBuffers[] = {GL_COLOR_ATTACHMENT0_EXT,
						   GL_COLOR_ATTACHMENT1_EXT,
						   GL_COLOR_ATTACHMENT2_EXT,
						   GL_COLOR_ATTACHMENT3_EXT,
						   GL_COLOR_ATTACHMENT4_EXT,
						   GL_COLOR_ATTACHMENT5_EXT,
						   GL_COLOR_ATTACHMENT6_EXT
};

#if 0
#define CHECK_GL_ERRORS  \
{ \
    GLenum err = glGetError(); \
    if (err) \
        printf( "Error %x at line %d\n", err, __LINE__); \
}
#else
#define CHECK_GL_ERRORS {}
#endif

void InitDualPeelingRenderTargets()
{
	glGenTextures(2, g_dualDepthTexId);
	glGenTextures(2, g_dualFrontBlenderTexId);
	glGenTextures(2, g_dualBackTempTexId);
	glGenFramebuffersEXT(1, &g_dualPeelingSingleFboId);
	for (int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//henan
		//glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_FLOAT_RG32_NV, g_imageWidth, g_imageHeight,
		 glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RG32F, g_imageWidth, g_imageHeight,
					 0, GL_RGB, GL_FLOAT, 0);

		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, g_imageWidth, g_imageHeight,
					 0, GL_RGBA, GL_FLOAT, 0);

		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[i]);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, g_imageWidth, g_imageHeight,
					 0, GL_RGBA, GL_FLOAT, 0);
	}

	glGenTextures(1, &g_dualBackBlenderTexId);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB, g_imageWidth, g_imageHeight,
				 0, GL_RGB, GL_FLOAT, 0);

	glGenFramebuffersEXT(1, &g_dualBackBlenderFboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualBackBlenderFboId);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
							  GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId, 0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingSingleFboId);

	int j = 0;
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[j], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[j], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT2_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[j], 0);

	j = 1;
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT3_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualDepthTexId[j], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT4_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualFrontBlenderTexId[j], 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT5_EXT,
								  GL_TEXTURE_RECTANGLE_ARB, g_dualBackTempTexId[j], 0);

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT6_EXT,
							  GL_TEXTURE_RECTANGLE_ARB, g_dualBackBlenderTexId, 0);

	CHECK_GL_ERRORS;
}

void InitAccumulationRenderTargets()
{
	glGenTextures(2, g_accumulationTexId);

	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_accumulationTexId[0]);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA16F_ARB,
				 g_imageWidth, g_imageHeight, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, g_accumulationTexId[1]);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//henan
	//glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_FLOAT_R32_NV,
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_R32F,
				 g_imageWidth, g_imageHeight, 0, GL_RGBA, GL_FLOAT, NULL);

	glGenFramebuffersEXT(1, &g_accumulationFboId);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_accumulationFboId);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
							  GL_TEXTURE_RECTANGLE_ARB, g_accumulationTexId[0], 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT,
							  GL_TEXTURE_RECTANGLE_ARB, g_accumulationTexId[1], 0);

	CHECK_GL_ERRORS;
}
//--------------------------------------------------------------------------
void DeleteDualPeelingRenderTargets()
{
	glDeleteFramebuffersEXT(1, &g_dualBackBlenderFboId);
	glDeleteFramebuffersEXT(1, &g_dualPeelingSingleFboId);
	glDeleteTextures(2, g_dualDepthTexId);
	glDeleteTextures(2, g_dualFrontBlenderTexId);
	glDeleteTextures(2, g_dualBackTempTexId);
	glDeleteTextures(1, &g_dualBackBlenderTexId);
}

//--------------------------------------------------------------------------
void MakeFullScreenQuad()
{
	g_quadDisplayList = glGenLists(1);
	glNewList(g_quadDisplayList, GL_COMPILE);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 1.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	{
		glVertex2f(0.0, 0.0); 
		glVertex2f(1.0, 0.0);
		glVertex2f(1.0, 1.0);
		glVertex2f(0.0, 1.0);
	}
	glEnd();
	glPopMatrix();

	glEndList();
}



//--------------------------------------------------------------------------
void BuildShaders(string SHADER_PATH)
{
        printf("\nloading shaders...\n");

        g_shaderDualInit.attachVertexShader(SHADER_PATH+"dual_peeling_init_vertex.glsl");
        g_shaderDualInit.attachFragmentShader(SHADER_PATH+"dual_peeling_init_fragment.glsl");
        g_shaderDualInit.link();

        g_shaderDualPeel.attachVertexShader(SHADER_PATH+"shade_vertex.glsl");
        g_shaderDualPeel.attachVertexShader(SHADER_PATH+"dual_peeling_peel_vertex.glsl");
        g_shaderDualPeel.attachFragmentShader(SHADER_PATH+"shade_fragment.glsl");
        g_shaderDualPeel.attachFragmentShader(SHADER_PATH+"dual_peeling_peel_fragment.glsl");
        g_shaderDualPeel.link();

        g_shaderDualBlend.attachVertexShader(SHADER_PATH+"dual_peeling_blend_vertex.glsl");
        g_shaderDualBlend.attachFragmentShader(SHADER_PATH+"dual_peeling_blend_fragment.glsl");
        g_shaderDualBlend.link();

        g_shaderDualFinal.attachVertexShader(SHADER_PATH+"dual_peeling_final_vertex.glsl");
        g_shaderDualFinal.attachFragmentShader(SHADER_PATH+"dual_peeling_final_fragment.glsl");
        g_shaderDualFinal.link();

	g_shaderAverageInit.attachVertexShader(SHADER_PATH+"shade_vertex.glsl");
	g_shaderAverageInit.attachVertexShader(SHADER_PATH+"wavg_init_vertex.glsl");
	g_shaderAverageInit.attachFragmentShader(SHADER_PATH+"shade_fragment.glsl");
	g_shaderAverageInit.attachFragmentShader(SHADER_PATH+"wavg_init_fragment.glsl");
	g_shaderAverageInit.link();

	g_shaderAverageFinal.attachVertexShader(SHADER_PATH+"wavg_final_vertex.glsl");
	g_shaderAverageFinal.attachFragmentShader(SHADER_PATH+"wavg_final_fragment.glsl");
	g_shaderAverageFinal.link();

}

//--------------------------------------------------------------------------
void DestroyShaders()
{
	g_shaderDualInit.destroy();
	g_shaderDualPeel.destroy();
	g_shaderDualBlend.destroy();
	g_shaderDualFinal.destroy();
}


void DeleteAccumulationRenderTargets()
{
	glDeleteFramebuffersEXT(1, &g_accumulationFboId);
	glDeleteTextures(2, g_accumulationTexId);
}
//--------------------------------------------------------------------------
void InitGL(string shader)
{ 
  /*GLfloat mat_diffuse[] = { 0.8, .0, 0.0, .4};
  GLfloat mat_specular[] = { 1, 1, 1, 1 };
  GLfloat mat_shininess[] = { 2.0 };
  GLfloat light_position[] = { 50,50,50, 0.0 };
  GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
*/
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);

	// Allocate render targets first
	InitDualPeelingRenderTargets();
       // InitAccumulationRenderTargets();
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	BuildShaders(shader);
	MakeFullScreenQuad();

//	glDisable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
//	glDisable(GL_NORMALIZE);

	glGenQueries(1, &g_queryId);
}

//--------------------------------------------------------------------------
void RenderDualPeeling()
{

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	// ---------------------------------------------------------------------
	// 1. Initialize Min-Max Depth Buffer
	// ---------------------------------------------------------------------

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingSingleFboId);

	// Render targets 1 and 2 store the front and back colors
	// Clear to 0.0 and use MAX blending to filter written color
	// At most one front color and one back color can be written every pass
	glDrawBuffers(2, &g_drawBuffers[1]);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Render target 0 stores (-minDepth, maxDepth, alphaMultiplier)
	glDrawBuffer(g_drawBuffers[0]);
	glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendEquationEXT(GL_MAX_EXT);

	g_shaderDualInit.bind();
        glPushMatrix();
        glMultMatrixf(m);
     //lt->RenderVBO();
      mesh->Render();
        glPopMatrix();
	g_shaderDualInit.unbind();

	CHECK_GL_ERRORS;

	// ---------------------------------------------------------------------
	// 2. Dual Depth Peeling + Blending
	// ---------------------------------------------------------------------

	// Since we cannot blend the back colors in the geometry passes,
	// we use another render target to do the alpha blending
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualBackBlenderFboId);
	glDrawBuffer(g_drawBuffers[6]);
	glClearColor(g_backgroundColor[0], g_backgroundColor[1], g_backgroundColor[2], 0);
	glClear(GL_COLOR_BUFFER_BIT);

	int currId = 0;

	for (int pass = 1; g_useOQ || pass < g_numPasses; pass++) {
		currId = pass % 2;
		int prevId = 1 - currId;
		int bufId = currId * 3;
		
		//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingFboId[currId]);

		glDrawBuffers(2, &g_drawBuffers[bufId+1]);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawBuffer(g_drawBuffers[bufId+0]);
		glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		// Render target 0: RG32F MAX blending
		// Render target 1: RGBA MAX blending
		// Render target 2: RGBA MAX blending
		glDrawBuffers(3, &g_drawBuffers[bufId+0]);
		glBlendEquationEXT(GL_MAX_EXT);

		g_shaderDualPeel.bind();
		g_shaderDualPeel.bindTextureRECT("DepthBlenderTex", g_dualDepthTexId[prevId], 0);
		g_shaderDualPeel.bindTextureRECT("FrontBlenderTex", g_dualFrontBlenderTexId[prevId], 1);
                glPushMatrix();
                glMultMatrixf(m);
                float surfaceFlag = 1;
                g_shaderDualPeel.setUniform("Alpha", (float*)&alphainside, 1);
                g_shaderDualPeel.setUniform("Surface", (float*)&surfaceFlag, 1);
                //lt->RenderVBO();
                mesh->Render();
                glPopMatrix();
		g_shaderDualPeel.unbind();

		CHECK_GL_ERRORS;

		// Full screen pass to alpha-blend the back color
		glDrawBuffer(g_drawBuffers[6]);

		glBlendEquationEXT(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (g_useOQ) {
			glBeginQuery(GL_SAMPLES_PASSED_ARB, g_queryId);
		}

		g_shaderDualBlend.bind();
		g_shaderDualBlend.bindTextureRECT("TempTex", g_dualBackTempTexId[currId], 0);
		glCallList(g_quadDisplayList);
		g_shaderDualBlend.unbind();

		CHECK_GL_ERRORS;

//		if (g_useOQ) {
			glEndQuery(GL_SAMPLES_PASSED_ARB);
			GLuint sample_count;
			glGetQueryObjectuiv(g_queryId, GL_QUERY_RESULT_ARB, &sample_count);
			if (sample_count == 0) {
				break;
			}
//		}
	}

	glDisable(GL_BLEND);

	// ---------------------------------------------------------------------
	// 3. Final Pass
	// ---------------------------------------------------------------------

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);

	g_shaderDualFinal.bind();
	g_shaderDualFinal.bindTextureRECT("DepthBlenderTex", g_dualDepthTexId[currId], 0);
	g_shaderDualFinal.bindTextureRECT("FrontBlenderTex", g_dualFrontBlenderTexId[currId], 1);
	g_shaderDualFinal.bindTextureRECT("BackBlenderTex", g_dualBackBlenderTexId, 2);
	glCallList(g_quadDisplayList);
	g_shaderDualFinal.unbind();

	CHECK_GL_ERRORS;


}




void RenderAverageColors()
{
	glDisable(GL_DEPTH_TEST);

	// ---------------------------------------------------------------------
	// 1. Accumulate Colors and Depth Complexity
	// ---------------------------------------------------------------------

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_accumulationFboId);
	glDrawBuffers(2, g_drawBuffers);

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glBlendEquationEXT(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_BLEND);

	g_shaderAverageInit.bind();
	//g_shaderAverageInit.setUniform("Alpha", (float*)&g_opacity, 1);
        glPushMatrix();
        glMultMatrixf(m);
//		g_shaderAverageInit.setUniform("Alpha", (float*)&alphainside, 1);
        g_shaderAverageInit.setUniform("Alpha", (float*)&alphaoutside, 1);
        glEnable(GL_LIGHTING);  
        //lt->RenderVBO();
         mesh->Render();
        glDisable(GL_LIGHTING);
        outline->DrawAxis(lb,rb);
        //outline->DrawXYZFont(lb,rb, viewproperty);
        glPopMatrix();

	g_shaderAverageInit.unbind();

	glDisable(GL_BLEND);

	CHECK_GL_ERRORS;

	// ---------------------------------------------------------------------
	// 2. Approximate Blending
	// ---------------------------------------------------------------------

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glDrawBuffer(GL_BACK);

	g_shaderAverageFinal.bind();
	g_shaderAverageFinal.setUniform("BackgroundColor", g_backgroundColor, 3);
	g_shaderAverageFinal.bindTextureRECT("ColorTex0", g_accumulationTexId[0], 0);
	g_shaderAverageFinal.bindTextureRECT("ColorTex1", g_accumulationTexId[1], 1);
	glCallList(g_quadDisplayList);
	g_shaderAverageFinal.unbind();

	CHECK_GL_ERRORS;
}


//--------------------------------------------------------------------------
void imageMovement(){ 
            if(image_inside)    
             image->Mouse(movement_x, image_height-movement_y,
                              contour_layer, contour_index, contour_mag);
}
void display()
{
        trackball.getMatrix().getValue(m);

	g_numGeoPasses = 0;	
        RenderDualPeeling();

   //     RenderAverageColors();
//================================================
        for(int i=0;i<16;i++)viewproperty.tm[i]=m[i];
        glGetDoublev (GL_MODELVIEW_MATRIX, viewproperty.mvmatrix);
        glGetDoublev (GL_PROJECTION_MATRIX, viewproperty.projmatrix);
        glGetIntegerv( GL_VIEWPORT, viewproperty.viewport );

        glDisable(GL_CULL_FACE);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0,(GLfloat) viewproperty.viewport[2], 0.0, (GLfloat) viewproperty.viewport[3]);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_LIGHTING);

        glColor3f(1,1,1);
        glDisable(GL_DEPTH_TEST);

        imageMovement();
        //image->RenderBoundary();
        //image->RenderColors();
        //image->RenderPixel();

        glMatrixMode(GL_PROJECTION);
        glLoadMatrixd(viewproperty.projmatrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixd(viewproperty.mvmatrix);


	glutSwapBuffers();
        glFlush();
}

//--------------------------------------------------------------------------
void reshape(int w, int h)
{

   glClearColor(0., 0., 0., 1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glViewport(0, 0, (GLsizei) w, (GLsizei) h);

   image_width = w;
   image_height = h;

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   float aspect =  float(w)/float(h);
   float GLfloat_fov=60;
    gluPerspective(GLfloat_fov,
                        aspect,
                        0.1,//   view_info.hither,
                        1000);//  view_info.yon);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(eye[0],eye[1],eye[2], 0,0,0,0,1,0);

//  mesh->GenerateWiresolid(" /home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/tmp/spin_proj_norm_vs_pos_p_v_z_10z_newcolumn/mesh1");


	if (g_imageWidth != w || g_imageHeight != h)
	{
		g_imageWidth = w;
		g_imageHeight = h;

		DeleteDualPeelingRenderTargets();
		InitDualPeelingRenderTargets();

//		DeleteAccumulationRenderTargets();
//		InitAccumulationRenderTargets();
	}


   Reshape2DContour();
}

void mouse(int button, int state, int x, int y){
        long s=0x00000000;
    s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
    s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
    s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;


    int key_state = glutGetModifiers();
    s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
//    s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
        s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

     if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
     { 
     /*    image->Mouse(x, image_height-y, contour_layer, contour_index, contour_mag);
         if(contour_layer>=0 && contour_index>=0){
           if(paser->GetIsContour())UpdateSingleContour();
           else UpdateVisible();
         }
       */  
     } 

        if (s & Trackball::BUTTON_DOWN){
          trackball.mouseDown(s, x, y);
          g_numPasses = 2;
          g_useOQ = false;
          glEnable(GL_CULL_FACE);
        }

        if (s & Trackball::BUTTON_UP){
          trackball.mouseUp(s, x, y);
          g_numPasses = 4;
          //g_useOQ = true;
            mesh->GenerateGlyphList(viewproperty);
          glDisable(GL_CULL_FACE);
        }
        glutPostRedisplay();
}

void motion(int x, int y){
        trackball.mouseMotion(x, y);
        glutPostRedisplay();
}

void movement(int x, int y){
     /*movement_x = x;
     movement_y = y;

     //cout<<"x "<<x<<" "<<image_lbx<<" "<<image_rbx<<" "<<endl;
    //cout<<"y "<<image_height-y<<" "<<image_lby<<" "<<image_rby<<" "<<endl;
     if(x >= image_lbx
            && x <= image_rbx
            && image_height-y >= image_lby
            && image_height - y <=image_rby )
          {
             movement_times = 0;
             image_inside = true;
             image->Move(x, image_height -y);
            // image->getImageSize(image_lbx, image_lby, image_rbx, image_rby);
          }
          else
          {
             image_inside = false;
             image->Move();
            // image->getImageSize(image_lbx, image_lby, image_rbx, image_rby);
          }
        glutPostRedisplay();
*/
}

void key(unsigned char key, int x, int y){
        switch (key) {
        case '\033':
        case 'q':
        case 'Q':
                exit(0);
                break;
        case 'h':
        case 'H':
               //contour->resetVisible();
               raw->resetVisible();
               UpdateVis();
               trackball.reset();
               break;
        case 'a':
               alphaoutside+=0.05;
               break;
        case 'A':
               alphaoutside-=0.05;
               break;
        }
        glutPostRedisplay();

}

//--------------------------------------------------------------------------
int main(int argc, char *argv[])
{  
  eye[0]=0;
  eye[1]=0;
  eye[2]=100;

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(g_imageWidth, g_imageHeight);
	glutInit(&argc, argv);
	glutCreateWindow("Dual Depth Peeling");

	if (glewInit() != GLEW_OK)
	{
		printf("glewInit failed. Exiting...\n");
		exit(1);
	}

    if (!glewIsSupported( "GL_VERSION_2_0 "
                          "GL_ARB_texture_rectangle "
						  "GL_ARB_texture_float "
						  "GL_NV_float_buffer "
                          "GL_NV_depth_buffer_float "))
    {
        printf("Unable to load the necessary extensions\n");
		printf("This sample requires:\n");
		printf("OpenGL version 2.0\n");
		printf("GL_ARB_texture_rectangle\n");
		printf("GL_ARB_texture_float\n");
		printf("GL_NV_float_buffer\n");
		printf("GL_NV_depth_buffer_float\n");
		printf("Exiting...\n");
        exit(1);
    }
  init();
  readConfig(strdup(argv[1]));
  setData();

  image = new svContourImageDrawing(raw->getData());

  cerr<<"Render routine..."<<endl;
  if(!paser->GetIsContour())
     lt = new svLTArrow(raw->getData());
  else
  {//cout<<"Contour"<<endl;
   lt = new svLTArrow(contour->getData());
  } 
  lt->SetColorByClusterMag();//(blackcolor);
  lt->UpdateData();cerr<<"update data"<<endl;
  lt->SetES(raw->getScaling());
  lt->SetNumPower(raw->getNumPower());
  lt->SetScaling(raw->getScaling());
  lt->UpdateIndex();cerr<<"update index"<<endl;
  lt->SetTubeRadius(0.05);cerr<<"radius"<<endl;
  lt->SetRadius(0.1);
  lt->SetScale(0.05);
  lt->GenerateTubes();cerr<<"tube"<<endl;
  lt->GenerateArrows();cerr<<"arrow"<<endl;
  lt->GenerateLegend();cerr<<"legend"<<endl;
  lt->GenerateIndex();cerr<<"index"<<endl;
  lt->UpdateTubeVBO();cerr<<"tvbo"<<endl;
  lt->UpdateArrowVBO();cerr<<"avbo"<<endl;
  lt->UpdateIndexVBO();cerr<<"ivbo"<<endl;

  lc = new svLCArrow(raw->getData());
  lc->SetES(raw->getScaling());
  lc->SetNumPower(raw->getNumPower());
  lc->SetScaling(raw->getScaling());
  lc ->SetColorByCluster();

  UpdateContour();
  Update2DColor();
  UpdateImage();

cerr<<"---done"<<endl;
 
/*  lc->UpdateData();cerr<<"update data"<<endl;
  lc->UpdateIndex(2);cerr<<"update index"<<endl;
  lc->SetTubeRadius(0.05);cerr<<"radius"<<endl;
  lc->SetRadius(0.1);
  lc->SetScale(0.05);
  lc->GenerateTubes();cerr<<"tube"<<endl;
  lc->GenerateArrows();cerr<<"arrow"<<endl;
  lc->GenerateIndex(2);cerr<<"index"<<endl;
  lc->UpdateTubeVBO();cerr<<"tvbo"<<endl;
  lc->UpdateArrowVBO();cerr<<"avbo"<<endl;
  lc->UpdateIndexVBO();cerr<<"ivbo"<<endl;

*/
  svVector3 center(0,0,0);
  trackball.setEye(eye);
  trackball.setFocus(center);
  trackball.setWindowSize(image_width, image_height);
  viewproperty.eye = eye;
  trackball.getMatrix().getValue(m);
  for(int i=0;i<16;i++)viewproperty.tm[i]=m[i];

  InitLight();
  char *str = new char[200];
  sprintf(str,"%s/libs/dual_depth_peeling/shaders/",SRC_DIR);
  mesh = new svMesh(raw->getData());
  mesh->SetData("/home/davinci/Documents/henan/NIST/NIST_SPLIT/SPLIT_VIS2/tmp/spin_proj_norm_vs_pos_p_v_z_10z_newcolumn/mesh1.txt",0);
  mesh->GenerateSurface(0);
  mesh->GenerateGlyphs();
  mesh->GenerateGlyphList(viewproperty);
  InitGL(str);
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(key);
  glutPassiveMotionFunc(movement);
  glutMainLoop();
  return 0;
}

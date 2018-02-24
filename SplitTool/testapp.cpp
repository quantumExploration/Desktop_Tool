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
#include "svQDOTData.h"
#include "svMesh.h"
#include "svLCArrow.h"
#include "svLTArrow.h"
#include "svImageList.h"
using namespace __svl_lib;

GLfloat m[16];

svMesh *meshRender;
svLCArrow *lc;
svLTArrow *lt;
svColors *myColor;
svImageList *mySlice;
//=====================
float alpha;
int g_numPasses = 4;
int g_imageWidth=1920;
int g_imageHeight=1080;

nv::Model *g_model;
GLuint g_quadDisplayList;
GLuint g_vboId;
GLuint g_eboId;

bool g_useOQ = false;
GLuint g_queryId;

static nv::SDKPath sdkPath;

GLSLProgramObject g_shaderDualInit;
GLSLProgramObject g_shaderDualPeel;
GLSLProgramObject g_shaderDualBlend;
GLSLProgramObject g_shaderDualFinal;

float g_opacity=0.75;
bool g_showOsd;
bool g_bShowUI;
unsigned g_numGeoPasses=0;

int g_oldX, g_oldY;
int g_newX, g_newY;


float g_white[3] = {1.0,1.0,1.0};
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

void MakeFullScreenQuad();
void InitDualPeelingRenderTargets();
void DeleteDualPeelingRenderTargets();
void BuildShaders(string SHADER_PATH);
void InitGL(string shader);
void RenderDualPeeling();

//==============
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
void DeleteDualPeelingRenderTargets()
{
        glDeleteFramebuffersEXT(1, &g_dualBackBlenderFboId);
        glDeleteFramebuffersEXT(1, &g_dualPeelingSingleFboId);
        glDeleteTextures(2, g_dualDepthTexId);
        glDeleteTextures(2, g_dualFrontBlenderTexId);
        glDeleteTextures(2, g_dualBackTempTexId);
        glDeleteTextures(1, &g_dualBackBlenderTexId);
}
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


}
void InitLight();
void InitGL(string shader)
{
     g_numPasses = 4;
     g_useOQ = false;
     g_showOsd = true;
     g_bShowUI = true;
     g_numGeoPasses = 0;

     g_white[0]=1;g_white[1]=1;g_white[2]=1;
     g_black[0]=0;g_black[1]=0;g_black[2]=1;

     g_drawBuffers[0] = GL_COLOR_ATTACHMENT0_EXT;
     g_drawBuffers[1] = GL_COLOR_ATTACHMENT1_EXT;
     g_drawBuffers[2] = GL_COLOR_ATTACHMENT2_EXT;
     g_drawBuffers[3] = GL_COLOR_ATTACHMENT3_EXT;
     g_drawBuffers[4] = GL_COLOR_ATTACHMENT4_EXT;
     g_drawBuffers[5] = GL_COLOR_ATTACHMENT5_EXT;
     g_drawBuffers[6] = GL_COLOR_ATTACHMENT6_EXT;
     g_backgroundColor = g_white;

        InitDualPeelingRenderTargets();
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        BuildShaders(shader);
        MakeFullScreenQuad();

        glDisable(GL_CULL_FACE);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glDisable(GL_NORMALIZE);


        glGenQueries(1, &g_queryId);
   InitLight();
}

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
        //lc->RenderVBO();
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
                g_shaderDualPeel.setUniform("Alpha", (float*)&alpha, 1);
                //lc->RenderVBO();
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

//             if (g_useOQ) {
                        glEndQuery(GL_SAMPLES_PASSED_ARB);
                        GLuint sample_count;
                        glGetQueryObjectuiv(g_queryId, GL_QUERY_RESULT_ARB, &sample_count);
                        if (sample_count == 0) {
                                break;
                        }
//              }
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

//=====================
int image_height =1080;
int image_width = 1920;
svVector3 eye;//(0,0,100);

svQDOT *rawData;
svRawSliceData *sliceData;
svRawSymmetryData *symmetryData;
svRawClusterData *clusterData;
svContourData *contourData;
svContourClusterData *contourClusterData;
svMeshData *meshData;
//svLCArrow *lc;
State *myState;

ViewProperty viewproperty;
Trackball trackball;
bool encode_type = 0;
int sample = 3;

int contourstep = 0;
bool contourratio = false;

float arrowradius=0.06;
float tuberadius=0.05;

void init(){
  rawData = new svQDOT();
  myState = new State();
  vector<int> num;num.push_back(40); //num.push_back(15);
  vector<bool> split; split.push_back(false); split.push_back(false);
  double **weights = new double*[2];weights[0]=new double[7];weights[1]=new double[7];
  for(int i=0;i<7;i++)weights[0][i] = 1; weights[0][6] = 1.;
  for(int i=0;i<7;i++)weights[1][i] = 1; weights[1][6] = 1.;
  ROIProperty *roiP1=new ROIProperty(); roiP1->maxMag =1; roiP1->minMag =0;
  roiP1->splitVisible.resize(109);
  for(int i=0;i<roiP1->splitVisible.size();i++) roiP1->splitVisible[i]=true;
  roiP1->maxX = 180; roiP1->minX = 0;
  roiP1->maxY = 180; roiP1->minY = 0;
  roiP1->maxZ = 180; roiP1->minZ = 0;
  vector<ROIProperty *>p; p.push_back(roiP1);
  ROIProperty *roiP2=new ROIProperty(); roiP2->maxMag =1; roiP2->minMag =1e-7;
  roiP2->splitVisible.resize(109);
  for(int i=0;i<roiP2->splitVisible.size();i++) roiP2->splitVisible[i]=true;
  roiP2->maxX = 180; roiP2->minX = 0;
  roiP2->maxY = 180; roiP2->minY = 0;
  roiP2->maxZ = 180; roiP2->minZ = 0;
  p.push_back(roiP2);
  myState->NewClusterProperty(num,split,weights,p);
  svVector3 pos(0,0,0);
  svVector3 dir(1,1,0); dir.normalize();
  svVector3 x(-1,1,0); x.normalize();
  svVector3 y(0,0,1); y.normalize();
  svScalar angle = 10;
  svScalar mag = 1e-7;
  svVector3 planepos(0,0,-10);
  svVector3 planedir(0,0,1);
  myState->NewSymmetryProperty(pos,dir,x,y,planepos, planedir, 0.25, angle,mag);
}

void setData(int type, char *filedir, char *tmpdir, char *file,
             svVector3 center, svVector3 dir, svScalar distance){
  char *str= new char[300];
  sprintf(str,"%s/%s/",tmpdir,file);
  rawData->SetData(type, filedir, str, file);

  meshData = new svMeshData(rawData);
  meshData->SetData(str);
  meshRender = new svMesh(meshData);
  meshRender->GenerateGlyph(viewproperty);
  meshRender->GenerateSurface();

  sliceData = new svRawSliceData(rawData);
  sliceData->Init(center, dir, distance);
  sliceData->SaveSlicetoVTK(tmpdir, file);
  sliceData->SaveSliceDensitytoFile(str);
  sliceData->state = myState;
  myState->NewROIProperty(1.,0.,180.,180.,180.,0.,0.,0.,sliceData->splitData.size());

  symmetryData = new svRawSymmetryData(rawData);
  symmetryData->state = myState;
  sliceData->SetSymmetry(symmetryData);
  symmetryData->Init(str);
  myState->NewVisible(rawData);
  //sliceData->GenerateSampling(sample);
  sliceData->UpdateVisibleBySplit();
  myState->UpdateVisible(rawData);

  clusterData = new svRawClusterData(rawData);
  sliceData->SetCluster(clusterData);
  clusterData->SetSlice(sliceData);
  clusterData->SetSymmetry(symmetryData);
  clusterData->state = myState;
  clusterData->Init(str);
  clusterData->GenerateClusterSampling(sample);
  myState->UpdateVisible(rawData);

  contourData = new svContourData(rawData, sliceData);
  contourData->GenerateContourTree(str);
  contourData->GenerateContoursByContourTree(str, contourstep, contourratio);
//  contourData->GenerateSampling(sample);

  contourClusterData = new svContourClusterData(rawData, contourData);
  contourData->SetCluster(contourClusterData);
  contourClusterData->state = myState;
  contourClusterData->Init(str);

  myColor = new svColors();
  myColor->SetAlpha(1.);
  myColor->SetColorBlind(false);
  myColor->SetColorType(c_magnitude);

  mySlice = new svImageList(sliceData);
  mySlice->SetImageType(cluster_image);
  mySlice->SetColors(myColor);
  mySlice->Reshape(image_width/3.,image_height-100,image_width*2./3.);
  mySlice->GenerateGroupingByColor(0.8);
  mySlice->AddHybridImage(contourData, string(str), 10);


  lc = new svLCArrow(sliceData);
  lc->SetRadius(arrowradius);//0.1
  lc->SetScale(tuberadius);
  lc->SetTubeRadius(tuberadius);//0.05
  lc->UpdateData();
  lc->UpdateIndex();
  lc->SetColors(myColor);
  lc->GenerateArrows();
  lc->GenerateTubes();
  lc->GenerateIndex();
  lc->UpdateArrowVBO();
  lc->UpdateTubeVBO();
  lc->UpdateIndexVBO();

  myColor->SetColorType(c_texture_cluster);
  lt = new svLTArrow(sliceData);
  lt->SetRadius(arrowradius);
  lt->SetScale(tuberadius);
  lt->SetTubeRadius(tuberadius);
  lt->UpdateData();
  lt->UpdateIndex();
  lt->SetColors(myColor);
  lt->GenerateArrows();
  lt->GenerateTubes();
  lt->GenerateIndex();
  lt->GenerateVBO();

  cerr<<"processing done...."<<endl;
// lt->UpdateArrowVBO();
//  lt->UpdateTubeVBO();
 // lt->UpdateIndexVBO();

  char *osgfile = new char[100];
//  l->SaveTubetoOSG("/home/henan/Documents/NIST/NIST_SPLIT/SPLIT_VIS2/lctube.osg");
//  lc->SaveArrowtoOSG("/home/henan/Documents/NIST/NIST_SPLIT/SPLIT_VIS2/lcarrow.osg");
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
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
   glEnable(GL_NORMALIZE);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8,1};
  GLfloat mat_specular[] = { 0.2,0.2,0.2,0.2 };
  GLfloat light_position[] = {eye[0], eye[1], eye[2],1};//100,100,200, 0 };
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

void display(){
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_LIGHTING);
   glEnable(GL_DEPTH_TEST);

   InitLight();

   trackball.getMatrix().getValue(m);
   for(int i=0;i<16;i++)viewproperty.tm[i]=m[i];
   glPushMatrix();
   glMultMatrixf(m);
   if(encode_type == 0)lt->RenderVBO();
   if(encode_type == 1)lc->RenderVBO();
   meshRender->Render();
   glPopMatrix();

   glGetDoublev (GL_MODELVIEW_MATRIX, viewproperty.mvmatrix);
   glGetDoublev (GL_PROJECTION_MATRIX, viewproperty.projmatrix);
   glGetIntegerv( GL_VIEWPORT, viewproperty.viewport );

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(0.0,(GLfloat) viewproperty.viewport[2], 0.0, (GLfloat) viewproperty.viewport[3]);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glDisable(GL_LIGHTING);

   mySlice->Render2D();

   glMatrixMode(GL_PROJECTION);
   glLoadMatrixd(viewproperty.projmatrix);
   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixd(viewproperty.mvmatrix);

   glutSwapBuffers();
   glFlush();
}

//--------------------------------------------------------------------------
void reshape(int w, int h){
   glClearColor(0.5,0.5,0.5, 1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   g_imageWidth=w;g_imageHeight=h;

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

   if (g_imageWidth != w || g_imageHeight != h)
  {// DeleteDualPeelingRenderTargets();
   //InitDualPeelingRenderTargets();
  }
}

void mouse(int button, int state, int x, int y){
        long s=0x00000000;
    s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
    s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
    s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;


    int key_state = glutGetModifiers();
    s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
//  s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
    s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;


        if (s & Trackball::BUTTON_DOWN){
          trackball.mouseDown(s, x, y);
          //glEnable(GL_CULL_FACE);
        }

        if (s & Trackball::BUTTON_UP){
          trackball.mouseUp(s, x, y);
          meshRender->GenerateGlyph(viewproperty);
          //glDisable(GL_CULL_FACE);
        }
        glutPostRedisplay();
}

void motion(int x, int y){
        trackball.mouseMotion(x, y);
        glutPostRedisplay();
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
               trackball.reset();
               break;
        case 's':{
               cout<<"input frequency"<<endl;
               cin>>sample;
               myState->NewVisible(rawData);
               clusterData->GenerateClusterSampling(sample);
               myState->UpdateVisible(rawData);
               lc->UpdateIndex();
               lc->GenerateIndex();
               lc->UpdateIndexVBO();
               lt->UpdateIndex();
               lt->GenerateIndex();
               lt->UpdateIndexVBO();
               break;}
         case 'S':{
                  cout<<"tube osg file name"<<endl;
                  string file; cin>>file;
                  if(encode_type==1)
                  lc->SaveTubetoOSG((char *)file.c_str());
                  else
                  lt->SaveTubetoOSG((char *)file.c_str());
                  cout<<"arrow osg file name"<<endl;
                  cin>>file;
                  if(encode_type==1)
                  lc->SaveArrowtoOSG((char *)file.c_str());
                  else
                  lt->SaveArrowtoOSG((char *)file.c_str());
                  cout<<"done!"<<endl;  break;
               }
	  case 'c':
               {
                 lt->UpdateData();
                 lt->UpdateIndex();
                 lt->GenerateArrows();
                 lt->GenerateTubes();
                 lt->GenerateIndex();
                  lt->GenerateVBO();break;
               }
          case 'C':
               {
  		lt->UpdateData();
  		lt->UpdateIndex();
  		lt->SetColors(myColor);
  		lt->GenerateArrows();
  		lt->GenerateTubes();
  		lt->GenerateIndex();
  		lt->GenerateVBO();
  		break;
               }
	  case 'l':
		{
		  int zmin, zmax;
		  cout<<"input the min and max z layer index"<<endl;
		  cin>>zmin; cin>>zmax;
                  clusterData->GenerateClusterSampling(sample);
                  myState->UpdateSplitVisible(zmin, zmax);
                  sliceData->UpdateVisibleBySplit();
                  //clusterData->GenerateClusterSampling(sample);
                  //myState->UpdateSplitVisible(zmin, zmax);
                  myState->UpdateVisible(rawData);
                  lc->UpdateIndex();
                  lc->GenerateIndex();
                  lc->UpdateIndexVBO();
                  lt->UpdateIndex();
                  lt->GenerateIndex();
                  lt->UpdateIndexVBO();
                  break;
		}
          case 'e':
               encode_type = 1- encode_type;break;
        }
        glutPostRedisplay();

}

//--------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  eye[0]=0;
  eye[1]=0;
  eye[2]=100;

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(g_imageWidth, g_imageHeight);
	glutInit(&argc, argv);
	glutCreateWindow("QDOT");
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


  glutReshapeFunc(reshape);

  init();
  cerr<<"=======init=========="<<endl;
  svVector3 center(0,0,0);
  svVector3 dir(0,0,1);
  svScalar distance = 0.25;

  viewproperty.eye = eye;
  trackball.getMatrix().getValue(m);
  for(int i=0;i<16;i++)viewproperty.tm[i]=m[i];
  setData(atoi(argv[1]), strdup(argv[2]), strdup(argv[3]), strdup(argv[4]),
          center, dir, distance);
  char *str = new char[200];
  sprintf(str,"%s/libs/dual_depth_peeling/shaders/",SRC_DIR);
  InitGL(str);
  delete [] str;

  glutDisplayFunc(display);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(key);

  glutMainLoop();
  return 0;
}

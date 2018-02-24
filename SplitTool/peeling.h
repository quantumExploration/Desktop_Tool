#include "dual_depth_peeling.h"
#define GL_RG32F                          0x8230

using namespace std;

extern int g_numPasses;
extern int g_imageWidth;
extern int g_imageHeight;

extern GLuint g_quadDisplayList;

extern bool g_useOQ;
extern GLuint g_queryId;


extern GLSLProgramObject g_shaderDualInit;
extern GLSLProgramObject g_shaderDualPeel;
extern GLSLProgramObject g_shaderDualBlend;
extern GLSLProgramObject g_shaderDualFinal;

extern unsigned g_numGeoPasses;


extern float *g_backgroundColor;

extern GLuint g_dualBackBlenderFboId;
extern GLuint g_dualPeelingSingleFboId;
extern GLuint g_dualDepthTexId[2];
extern GLuint g_dualFrontBlenderTexId[2];
extern GLuint g_dualBackTempTexId[2];
extern GLuint g_dualBackBlenderTexId;

extern GLenum g_drawBuffers[7];


void MakeFullScreenQuad();
void InitDualPeelingRenderTargets();
void DeleteDualPeelingRenderTargets();
void BuildShaders(string SHADER_PATH);
void InitGL(string shader);
void DestroyShaders();



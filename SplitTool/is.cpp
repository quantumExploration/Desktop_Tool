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
#include "svQDOT.h"
#include "svDirectArrow.h"
#include "svSplitArrow.h"
#include "svSummaryGlyph.h"
//#include "svGlyph.h"
#include "svUtil.h"
#include "ivTrackball.h"
#include "ivview.h"
#include "MGL.h"
#include "svOutline.h"
#include "svMesh.h"
#include "svConfig.h"
#include "svWidget.h"
#include "svDirectCylinder.h"
#include "svLTCylinder.h"
#include "svLCCylinder.h"
#include "svLLOCylinder.h"
#include "svLLPCylinder.h"
#include "svCCL.h"
#include "svIcons.h"
#include "svColors.h"
#include "svContourImageDrawing.h"
#include "parser.h"
#include "svBendBar.h"

//#include "svDirectCylinder.h"
#include <string.h>

#include <GL/glui.h>

#define ENCODE 0
#define SOLID_COLOR 1
#define SYMMETRY_COLOR 2
#define SPLITVECTORS_SCALE 0.25//0.5
#define LINEAR_SCALE  10000//50000
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
#define LLO 1
#define LC 2
#define LT 3
#define LCL 4
#define LLP 5
#define CC 6

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

#define ENCODE_LINEAR_TUBE_ID 123

using namespace __svl_lib;

void reshape(int w, int h);
void display(void);
void key(unsigned char key, int x, int y);

#define IMAGE_WIDTH 1900
#define IMAGE_HEIGHT 1000

int window;
int mouse_button;
int mouse_state;
int movement_x;
int movement_y;
int movement_stop_x;
int movement_stop_y;
int movement_times=-1;
//=======GLUI==============
//
//===========================
GLUI *glui;
GLUI_RadioGroup *group_data;
GLUI_Checkbox *box_gr_contour;
GLUI_EditText *text_gr_contour;
GLUI_Checkbox *box_contour;
GLUI_Checkbox *box_raw;
GLUI_Checkbox *symmetrybox;
GLUI_Checkbox *box_encode;
GLUI_Checkbox *box_layer;
GLUI_Scrollbar *sb_line;
GLUI_Listbox *list_widget_vis;
GLUI_Scrollbar *sb_arrow;
GLUI_Scrollbar *sb_tube;
GLUI_Scrollbar *sb_mag;
GLUI_Scrollbar *sb_layer;
GLUI_Scrollbar *sb_alpha;
GLUI_Listbox *list_mesh_type;
GLUI_Listbox *list_mesh_vis;
GLUI_StaticText *text_mag_min;
GLUI_StaticText *text_mag_max;
GLUI_StaticText *text_layer_min;
GLUI_StaticText *text_layer_max;
GLUI_EditText *text_picture;
GLUI_EditText *text_config;
GLUI_Checkbox *color_box1;
GLUI_Checkbox *color_box2;
GLUI_Checkbox *color_box3;
GLUI_Checkbox *encode_box1;
GLUI_Checkbox *encode_box2;
GLUI_Checkbox *encode_box3;
GLUI_Checkbox *encode_box4;
GLUI_Checkbox *encode_box5;
GLUI_Checkbox *encode_box6;
GLUI_Checkbox *encode_box7;
GLUI_Checkbox *encode_box8;
GLUI_Checkbox *contour_box;
//=========END================

view3d view_info;

//svPeeling *peeling;
svQDOT *flow_field;
svDirectArrow *directglyph;
svDirectArrow *rawdata;
svDirectCylinder *directcylinder;
svLLOCylinder *llocylinder;
svLLPCylinder *llpcylinder;
svLTCylinder *ltcylinder;
svLCCylinder *lccylinder;
svSplitArrow *splitglyph;
svBendBar *bendglyph;
svSummaryGlyph *summaryglyph;
svOutline *outline;
svMesh *mesh;
svMesh *symmetrymesh;
svWidget *widget;
svContourImageDrawing *contour;
svImage *image;
//svConnect *ccl;

char *configFile;

static GLuint *texture;//[4*3];
svVector3 *texture_pos;
svVector3 texture_lbbox;
svVector3 texture_rbbox;
svScalar texture_size;
svScalar texture_ratio;
svScalar texture_visual_ratio;
svScalar texture_visual_size;
svVector3 *texture_visual_pos;
int *texture_status;
int *texture_visual_status;
static GLuint *texture_visual;
int textureinit;
vector<int> symmetrytype;
svScalarArray *symmetrycount;
int symmetrytopvalue;
int symmetry_enable;
svVector4 buttoncolors[SYMMETRY_TYPE+1];
svScalar topmaglevel;

int layerVisible = 0;
int layer_repeat = 0;
//===============
float summary_arrow_scale;
float summary_tube_scale;

int data_contour;
int data_raw;

int show_data;
int enable_gr_contour = true;
float global_ratio;

svInt contour_layer;
svInt contour_index;
svScalar contour_mag;
int enable_contour_tree=0;
int contour_back_color = 0;
float imageCutoff = 50;


float length_scale;
float arrow_scale;
float tube_scale;
float glui_length_scale;
float glui_arrow_scale;
float glui_tube_scale;
bool length_scale_up=false;
bool arrow_scale_up=false;
bool tube_scale_up=false;
bool updatedata = false;
bool updatesymmetry = false;
bool updatecluster = false;
bool updatevisible = false;
bool updaterender = false;
bool updatecontour = false;
int length_vis = 0;

string picture_name;
string config_name;

int symmetrymesh_enable = 0;
int mesh_enable = 0;
int mesh_type = 0;
vector<int> unique_region;
int mesh_vis = 1;

int symmetrymesh_solid_list;
int  mesh_solid_list;
int  outline_display_list;
int  summary_list;
int  render_list;

float alpha = 0.75;

svScalar mag_show;
svScalar mag_max=1e-4;
svScalar mag_min=1e-16;
float glui_mag_show=0.5;

svScalar widget_tranx;
svScalar widget_trany;
svScalar widget_scalex;
svScalar widget_scaley;
int znum;
int layer_min=0;
int layer_max=108;
int layer_show=50;
float glui_layer_show=0.5;
int samplesize = 20;

int encode_visible = 0;
int encode_type = CC;//LINEAR;// LLO;//LINEAR;
int colorby = ENCODE;
int color_cluster = 1;
int color_symmetry = 0;
int color_magnitude = 0;
int encode_linear = 1;
int encode_linear_tube = 0;
int encode_split = 0;
///////////////////

int frequency =1;
int summaryVisible;
svScalar scale;
svScalar directradius;
svScalar summaryradius;
int regioncount = 0;

ViewProperty viewproperty;
/*struct ConfigProperty{
	char *rawDir;
	char *rawFile;
	char *storeDir;
        char *planename;
	
	svVector3 plane_center;
        svVector3 plane_vector;
        svScalar plane_distance;
	
	svBool isContour;
	
	KmeansProperty step1_kmeansproperty;
	KmeansProperty step2_kmeansproperty;
        SymmetryProperty symmetryproperty; 
        NeighborProperty neighborproperty;

        vector<string> contourname;	
	ContourProperty contourproperty;
        ContourTreeProperty contourtreeproperty;
	
	svScalarArray *magrange;

        int format;	
} configproperty;
*/
//ConfigProperty configproperty;


BendProperty bendproperty;
BarProperty barproperty;

Paser *paser;

void InitField();
int contourindex = 0;
void ReadConfig(char *configfname);//, ConfigProperty &property);
void Update();
void ReleaseMemory();
void WriteContours();
void LocateTexture();
void LoadTexture();

GLboolean bBoundingBox=true;
GLboolean bAxis;

GLuint image_width;
GLuint image_height;

GLint nx, ny, nz;
Trackball trackball;
GLfloat m[16];

//int  zmin;
//int  zmax;
//int  nonz;

svInt volumebound[2]={0,100};
svInt volumestep = 5;
svInt vbound = 0;
svScalar magthreshold = 0;

//=========================Depth Peeling============
int g_numPasses;
int g_imageWidth;
int g_imageHeight;


nv::Model *g_model;
GLuint g_quadDisplayList;
GLuint g_vboId;
GLuint g_eboId;

bool g_useOQ;
GLuint g_queryId;

static nv::SDKPath sdkPath;

GLSLProgramObject g_shaderDualInit;
GLSLProgramObject g_shaderDualPeel;
GLSLProgramObject g_shaderDualBlend;
GLSLProgramObject g_shaderDualFinal;

vector<float> g_opacity;
bool g_showOsd;
bool g_bShowUI;
unsigned g_numGeoPasses;

int g_oldX, g_oldY;
int g_newX, g_newY;

float g_white[3];
float g_black[3];
float *g_backgroundColor;

GLuint g_dualBackBlenderFboId;
GLuint g_dualPeelingSingleFboId;
GLuint g_dualDepthTexId[2];
GLuint g_dualFrontBlenderTexId[2];
GLuint g_dualBackTempTexId[2];
GLuint g_dualBackBlenderTexId;

GLenum g_drawBuffers[7];

void MakeFullScreenQuad();
void InitDualPeelingRenderTargets();
void DeleteDualPeelingRenderTargets();
void BuildShaders(string SHADER_PATH);
void InitGL(string shader);
void RenderDualPeeling();

//===================================================

void ArrowLight();
void glui_display();
void VisualMapping();


void InitLight()
{
  //setting of lighting
  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8,1};
  GLfloat mat_specular[] = { 0.2,0.2,0.2,0.2 };
  GLfloat light_position[] = { 100,100,200, 0 };
  GLfloat white_light[] = { 0.8, 0.8, 0.8, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

  GLfloat lmodel_ambient[] = {0.2,0.2,0.2,1.0};
  GLfloat local_view[] = {0.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,lmodel_ambient);
  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

  GLfloat light1_position[] = {0,0,100,0};
  GLfloat light1_ambient[] = {0.2,0.2,0.2,1};
  GLfloat light1_diffuse[] = {0.5,0.5,0.5,1.0};
  GLfloat light1_specular[] = {0.5,0.5,0.5,1.0};
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR,light1_specular);

  ArrowLight();
}


void ArrowLight()
{
  GLfloat mat_ambient[] = {1,1,1,1};
  GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, .8};
  GLfloat mat_specular[] = { 0.2,0.2,0.2,0.2 };
  GLfloat mat_shininess[] = { 50 };
  glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}


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


/* accFrustum()
 * The first 6 arguments are identical to the glFrustum() call.
 *  
 * pixdx and pixdy are anti-alias jitter in pixels. 
 * Set both equal to 0.0 for no anti-alias jitter.
 * eyedx and eyedy are depth-of field jitter in pixels. 
 * Set both equal to 0.0 for no depth of field effects.
 *
 * focus is distance from eye to plane in focus. 
 * focus must be greater than, but not equal to 0.0.
 *
 * Note that accFrustum() calls glTranslatef().  You will 
 * probably want to insure that your ModelView matrix has been 
 * initialized to identity before calling accFrustum().
 */
void accFrustum(GLdouble left, GLdouble right, GLdouble bottom, 
   GLdouble top, GLdouble near, GLdouble far, GLdouble pixdx, 
   GLdouble pixdy, GLdouble eyedx, GLdouble eyedy, GLdouble focus)
{
   GLdouble xwsize, ywsize; 
   GLdouble dx, dy;
   GLint viewport[4];

   glGetIntegerv (GL_VIEWPORT, viewport);
	
   xwsize = right - left;
   ywsize = top - bottom;
	
   dx = -(pixdx*xwsize/(GLdouble) viewport[2] + eyedx*near/focus);
   dy = -(pixdy*ywsize/(GLdouble) viewport[3] + eyedy*near/focus);
	
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum (left + dx, right + dx, bottom + dy, top + dy, near, far);
   gluLookAt(view_info.eye[0],view_info.eye[1],view_info.eye[2],
                                view_info.coi[0],view_info.coi[1],view_info.coi[2],
                                0, 1, 0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
 //  glTranslatef (-eyedx, -eyedy, 0.0);
}

/* accPerspective()
 * 
 * The first 4 arguments are identical to the gluPerspective() call.
 * pixdx and pixdy are anti-alias jitter in pixels. 
 * Set both equal to 0.0 for no anti-alias jitter.
 * eyedx and eyedy are depth-of field jitter in pixels. 
 * Set both equal to 0.0 for no depth of field effects.
 *
 * focus is distance from eye to plane in focus. 
 * focus must be greater than, but not equal to 0.0.
 *
 * Note that accPerspective() calls accFrustum().
 */
void accPerspective(GLdouble fovy, GLdouble aspect, 
   GLdouble near, GLdouble far, GLdouble pixdx, GLdouble pixdy, 
   GLdouble eyedx, GLdouble eyedy, GLdouble focus)
{
   GLdouble fov2,left,right,bottom,top;

   fov2 = ((fovy*M_PI) / 180.0) / 2.0;

   top = near / (cos(fov2) / sin(fov2));
   bottom = -top;

   right = top * aspect;
   left = -right;

   accFrustum (left, right, bottom, top, near, far,
               pixdx, pixdy, eyedx, eyedy, focus);
}

/*  Initialize lighting and other values.
 */
void Init(void)
{
   InitLight();

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);

   //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

   glEnable(GL_DEPTH_TEST);
  // glShadeModel (GL_FLAT);

   glEnable(GL_NORMALIZE);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);  
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 

}

void accDisplay(void)
{
   //InitLight();
//   glEnable(GL_LIGHTING);
//   glEnable(GL_LIGHT0);
//   glEnable(GL_LIGHT1);

   //glEnable(GL_MULTISAMPLE);

   //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   float aspect = view_info.aspect_ratio * float(image_width)/float(image_height);
   float GLfloat_fov;
   if ( aspect < 1 ){
                  // fovy is a misnomer.. we GLfloatly mean the fov applies to the
                  // smaller dimension
                  float fovx, fovy;
                  fovx = fovy = view_info.view_angle;
                  GLfloat_fov = svToDegree(2*atan(tan(svToRadian(fovx/2))/aspect));
   }
   else{
                GLfloat_fov = view_info.view_angle;
    }

     //  GLfloat m[16];

      if(mouse_state == GLUT_DOWN)
      {
          glEnable(GL_CULL_FACE);
      }

       trackball.getMatrix().getValue(m);

       int jitter;
       glClear(GL_ACCUM_BUFFER_BIT);
      // glClearAccum(0.5, 0.5, 0.5, 1.0);
       for (jitter = 0; jitter < ACSIZE; jitter++) {
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      // glClearColor(0.5, 0.5, 0.5, 1.0);
       accPerspective (GLfloat_fov,
                       aspect,
                      view_info.hither,
                      view_info.yon, 
                     j8[jitter].x, j8[jitter].y, 0, 0, 50.0);

        glPushMatrix();
      //  glMultMatrixf(m);
        RenderDualPeeling();
       // splitglyph->RenderVBO();
        //summaryglyph->Render();
        //directglyph->Render();
        //peeling->RenderDualPeeling();
        //glEnable(GL_DEPTH_TEST);
        //summaryglyph->RenderFont();
        glPopMatrix();
 
        glAccum(GL_ACCUM, 1.0/ACSIZE);
      }
      glAccum (GL_RETURN, 1.0);


//================2D Visualization======================================================
        glDisable(GL_CULL_FACE);

        //viewproperty.tm = m;

        for(int i=0;i<16;i++)viewproperty.tm[i]=m[i];

        glGetDoublev (GL_MODELVIEW_MATRIX, viewproperty.mvmatrix);
        glGetDoublev (GL_PROJECTION_MATRIX, viewproperty.projmatrix);
        glGetIntegerv( GL_VIEWPORT, viewproperty.viewport );

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0.0,(GLfloat) viewproperty.viewport[2], 0.0, (GLfloat) viewproperty.viewport[3]);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glDisable(GL_LIGHTING);

	glPushMatrix();
	glTranslatef(image_width-150,image_height-350,0);
        //color / texture legend
        glPopMatrix();

            glPushMatrix();
            glTranslatef(image_width-500,10.,0);
            glScalef(100,100,100);

            if(encode_type == CC)
              bendglyph->RenderLegend(bendproperty);

            glPopMatrix();


        //glEnable(GL_DEPTH_TEST);
      //  glDisable(GL_LIGHTING);
        //glEnable(GL_TEXTURE_2D);
        //glTranslatef(0,image_height-50,0);
       // else if(encode_visible)
       //     DrawVisibleButtons();

       if(encode_type == LC || encode_type == LCL)
       {
          
       }
       if(layerVisible)
       {
        //glDisable(GL_TEXTURE_2D);
       // glEnable(GL_LIGHTING);

           glPushMatrix();
           glTranslatef(widget_tranx, widget_trany,0);
           glScalef(widget_scalex,widget_scaley,1);
           widget->Render(topmaglevel);
           glPopMatrix();
        }
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixd(viewproperty.projmatrix);
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixd(viewproperty.mvmatrix);

      glFlush();
      glutSwapBuffers();
}

void Reshape(int w, int h)
{
   g_imageWidth=w;g_imageHeight=h;

   glClearColor(0., 0., 0., 1);
   glClearAccum(0, 0., 0., 1);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glClear(GL_ACCUM_BUFFER_BIT);

   glViewport(0, 0, (GLsizei) w, (GLsizei) h);

   image_width = w;
   image_height = h;

  widget_tranx=325;
  widget_trany=image_height-75;
  widget_scalex=4;
  widget_scaley=10.;

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   float aspect = view_info.aspect_ratio * float(w)/float(h);
   float GLfloat_fov;
   if ( aspect < 1 ){
                  // fovy is a misnomer.. we GLfloatly mean the fov applies to the
                  // smaller dimension
                  float fovx, fovy;
                  fovx = fovy = view_info.view_angle;
                  GLfloat_fov = svToDegree(2*atan(tan(svToRadian(fovx/2))/aspect));
   }
   else{
                GLfloat_fov = view_info.view_angle;
    }
//cerr<<aspect<<" "<<GLfloat_fov<<endl;
    gluPerspective(GLfloat_fov,
                                           aspect,
                                           view_info.hither,
                                           view_info.yon);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(view_info.eye[0],view_info.eye[1],view_info.eye[2],
                                view_info.coi[0],view_info.coi[1],view_info.coi[2],
                                0, 1, 0);

   //peeling->Reshape(w,h);
   DeleteDualPeelingRenderTargets();
   InitDualPeelingRenderTargets();

   int rx, ry, rw, rh;
   GLUI_Master.get_viewport_area(&rx, &ry, &rw, &rh);
}

void UpdateColor()
{
   if(encode_type == LINEAR || encode_type==LLO||encode_type == LT ||encode_type==LLP)
   {
      directcylinder->SetSolidColor();
      llocylinder->SetSolidColor();
      ltcylinder->SetSolidColor();
      llpcylinder->SetSolidColor();
   }
   else if(encode_type == LCL)
   {
       llocylinder->SetColorByPower();
   }
   else if(encode_type == LC)
   {
      lccylinder->SetColorByPower();
   }
   else if(encode_type == CC)
   {
     svVector4 barcolor;

     barcolor[0] = 76./255.;//248./255.;//76./255.;
     barcolor[1] = 28./255.;//170./255.;//28./255.;
     barcolor[2] = 95./255.;//161./255.;//95./255.;

      bendglyph->SetColor(barcolor);
   }
}
void UpdateCluster()
{
   directcylinder->ResetCluster();//cerr<<"done1"<<endl;
   directcylinder->SetROI(paser->GetMagRange00(),paser->GetMagRange01());
   directcylinder->GenerateClustersBySymmetry(paser->GetStep1Property(),
                      paser->GetSymmetryProperty(),
                      symmetrytype);
   directcylinder->SetROI(paser->GetMagRange10(),paser->GetMagRange11());
   directcylinder->GenerateClustersBySymmetry(paser->GetStep2Property(),
                      paser->GetSymmetryProperty(),
                      symmetrytype);

   llocylinder->ResetCluster();
   llocylinder->GenerateClusters(directcylinder->GetClusterLabels());

   ltcylinder->ResetCluster();
   ltcylinder->GenerateClusters(directcylinder->GetClusterLabels());

   lccylinder->ResetCluster();
   lccylinder->GenerateClusters(directcylinder->GetClusterLabels());


   llpcylinder->ResetCluster();
   llpcylinder->GenerateClusters(directcylinder->GetClusterLabels());
}
void UpdateRender()
{
    if(encode_type == LINEAR)
    {
        length_scale = (glui_length_scale * LINEAR_SCALE)*5;
        tube_scale =(glui_length_scale * SPLITVECTORS_SCALE);// (glui_tube_scale * TUBE_SCALE)*2.;

        cerr<<"length_scale "<<length_scale<<endl;
        cerr<<"tube_scale "<<tube_scale<<endl;

        directcylinder->SetScale(length_scale);
        directcylinder->SetTubeRadius(tube_scale);
        directcylinder->GenerateTubes();
        directcylinder->UpdateTubeVBO();
    }
    else if(encode_type ==LLO || encode_type == LCL)
    {
        cerr<<"LLO"<<endl;

        length_scale = (glui_length_scale * SPLITVECTORS_SCALE);
        tube_scale = length_scale;

        llocylinder->SetScale(length_scale);
        llocylinder->SetTubeRadius(tube_scale);
        llocylinder->GenerateTubes();
        llocylinder->UpdateTubeVBO();
        llocylinder->GenerateLegend();
    }
    else if(encode_type ==LT)
    {
        length_scale = (glui_length_scale * SPLITVECTORS_SCALE);
        tube_scale = length_scale;

        ltcylinder->SetScale(length_scale);
        ltcylinder->SetTubeRadius(tube_scale);
        ltcylinder->GenerateTubes();
        ltcylinder->UpdateTubeVBO();
        ltcylinder->GenerateLegend();
    }
    else if(encode_type ==LC)
    {
        length_scale = (glui_length_scale * SPLITVECTORS_SCALE);
        tube_scale = length_scale;

        lccylinder->SetScale(length_scale);
        lccylinder->SetTubeRadius(tube_scale);
        lccylinder->GenerateTubes();
        lccylinder->UpdateTubeVBO();
        lccylinder->GenerateLegend();
    }
    else if(encode_type == LLP)
    {
        cerr<<"LLP"<<endl;

        length_scale = (glui_length_scale * SPLITVECTORS_SCALE);
        tube_scale = length_scale;

        llpcylinder->SetScale(length_scale);
        llpcylinder->SetTubeRadius(tube_scale);
        llpcylinder->GenerateTubes();
        llpcylinder->UpdateTubeVBO();
        llpcylinder->GenerateLegend();
    }
    else if(encode_type == CC)
    {
         bendglyph->SetBarProperty(barproperty);
         bendglyph->SetAlpha(0.5);
         bendglyph->SetDisplayList(10);//cerr<<"done2"<<endl;
         bendglyph->SetLegendList(20);
        // svVector3 l;l[0]=-20;l[1]=-5;l[2]=-25;
        // svVector3 r; r[0]=-5;r[1]=5;r[2]=25;
        // bendglyph->SetVisible(l,r);
         bendglyph->Generate(bendproperty,viewproperty,
                 paser->GetPlaneVector(), 40);//cerr<<"done4"<<endl;

         svVector3 startPos = svVector3(10,-9.5,-1.5);
         svVector3 dir1= svVector3(1,0,0);
         svVector3 dir2=svVector3(0,1,0);
         svScalar stepD1 = 0.5;
         svScalar stepD2 = 0.5;
         svInt stepN1 = 17;
         svInt stepN2 = 19;
         bendglyph->GenerateLegend(bendproperty,startPos, dir1,  dir2,
                        stepD1,  stepD2,
                        stepN1,  stepN2,paser->GetPlaneVector());
         RenderProperty renderproperty;
         renderproperty.color[0] = 0.5;
         renderproperty.color[1] = 0.5;
         renderproperty.color[2] = 0.5;
         renderproperty.color[3] = 0.5;
         bendglyph->SetRenderProperty(renderproperty);
    }
}
void UpdateVBOData()
{
   directcylinder->UpdateData();
   llocylinder->UpdateData();
   ltcylinder->UpdateLTData();
   lccylinder->UpdateData();
   llpcylinder->UpdateLLPData();
}
void UpdateSymmetry()
{
   directcylinder->GenerateSymmetry(paser->GetSymmetryProperty());//, frequency);
   llocylinder->GenerateSymmetry(paser->GetSymmetryProperty());//, frequency);
   llpcylinder->GenerateSymmetry(paser->GetSymmetryProperty());
   ltcylinder->GenerateSymmetry(paser->GetSymmetryProperty());
   lccylinder->GenerateSymmetry(paser->GetSymmetryProperty());
   symmetrycount = directcylinder->GetSymmetryMagProb(symmetrytopvalue);
}
void UpdateVisible()
{
    cerr<<"start"<<endl;
    svIntArray visiblelayer = widget->GetVisible();
   if(symmetrytype.size()>0) symmetrytype.clear();

    for(int i=0;i<SYMMETRY_TYPE;i++)
        symmetrytype.push_back(i);
    symmetrytype.push_back(-1);

   directcylinder->ResetVisible();
   if(paser->GetIsContour())//configproperty.isContour)
   {
       directcylinder->SetContourSampling(frequency);
   }
   else
   {
       directcylinder->SetSampling(frequency);
   }cerr<<"sampling"<<endl;
   directcylinder->SetSymmetryVisible(symmetrytype);cerr<<"symmetrytype"<<endl;
   directcylinder->SetVisible(contourindex);cerr<<"visible"<<endl;
   directcylinder->SetLayerVisible(visiblelayer);
   directcylinder->SetVisible(1e-7, 1e-3);
   directcylinder->UpdateIndex();cerr<<"UpdateIndex()"<<endl;
   directcylinder->GenerateIndex();cerr<<"GenerateIndex()"<<endl;
   directcylinder->SetSampleData(samplesize);cerr<<"samplesize"<<endl;

   llocylinder->ResetVisible();
   if(paser->GetIsContour())//configproperty.isContour)
   {
      llocylinder->SetContourSampling(frequency);
   }
   else
   {
       llocylinder->SetSampling(frequency);
   }
   llocylinder->SetSymmetryVisible(symmetrytype);
   llocylinder->SetVisible(contourindex);
   llocylinder->SetLayerVisible(visiblelayer);
   llocylinder->UpdateIndex();
   llocylinder->GenerateIndex();
   llocylinder->SetSampleData(samplesize);

cerr<<"================================"<<endl;
   llpcylinder->ResetVisible();
   if(paser->GetIsContour())//configproperty.isContour)
   {
      llpcylinder->SetContourSampling(frequency);
   }
   else
   {
       llpcylinder->SetSampling(frequency);
   }
   llpcylinder->SetSymmetryVisible(symmetrytype);
   llpcylinder->SetVisible(contourindex);
   llpcylinder->SetLayerVisible(visiblelayer);
   llpcylinder->SetVisible(1e-7, 1e-3);
   llpcylinder->UpdateLLPIndex();
   llpcylinder->GenerateIndex();
   llpcylinder->SetSampleData(samplesize);

   cerr<<"llo"<<endl;
   ltcylinder->ResetVisible();cerr<<"visible"<<endl;
   if(paser->GetIsContour())//configproperty.isContour)
   {
      ltcylinder->SetContourSampling(frequency);
   }
   else
   {
       ltcylinder->SetSampling(frequency);
   }
   ltcylinder->SetSymmetryVisible(symmetrytype);
   ltcylinder->SetVisible(contourindex);
cerr<<"visible"<<endl;
   //ltcylinder->SetLayerVisible(visiblelayer);
cerr<<"layer"<<endl;
   ltcylinder->UpdateLTIndex();
cerr<<"UpdateIndex()"<<endl;
   ltcylinder->GenerateIndex();
cerr<<"GenerateIndex()"<<endl;
   ltcylinder->SetSampleData(samplesize);
cerr<<"lt"<<endl;

   lccylinder->ResetVisible();
   if(paser->GetIsContour())//configproperty.isContour)
   {
      lccylinder->SetContourSampling(frequency);
   }
   else
   {
       lccylinder->SetSampling(frequency);
   }
   lccylinder->SetSymmetryVisible(symmetrytype);
   lccylinder->SetVisible(contourindex);
   lccylinder->SetVisible(widget->GetVisible());
   lccylinder->UpdateIndex();
   lccylinder->GenerateIndex();
   lccylinder->SetSampleData(samplesize);

  cerr<<"lc"<<endl;

    if(encode_type == LINEAR)
    {
         directcylinder->UpdateIndexVBO();
     }
     else if(encode_type == LLO || encode_type == LCL)
     {
         llocylinder->UpdateIndexVBO();
     }
     else if(encode_type == LT)
     {
        ltcylinder->UpdateIndexVBO();
     }
     else if(encode_type == LC)
     {
        lccylinder->UpdateIndexVBO();
     }
     else if(encode_type == LLP)
     {
         llpcylinder->UpdateIndexVBO();
     }

//    visiblelayer.free();
    cerr<<"done udpatevisible"<<endl;
}

void control_cb(int control)
{
      glutSetWindow(window);
 
      if(control == ENCODE_ID)
      {
cerr<<"====================================================="<<endl;
          UpdateVisible();
cerr<<"Update visible"<<endl;
          UpdateColor();
cerr<<"Update color"<<endl;
          UpdateRender();
cerr<<"Update render"<<endl;
          UpdateVisible();
cerr<<"Update visible"<<endl;
      }
     else if(control == LAYER_ID)
     {
     }
     else if(control == LAYER_REPEAT_ID)
     {
        widget->Repeat(layer_repeat);
        if(layerVisible)
        {     svVector3 value[3];
             svInt *index=new svInt[3];  widget->GetIndex(index);
             for(int i=0;i<3;i++)
             {
               value[i]  = flow_field->GetPlanePosition(index[i]);
             }
             widget->SetValues(value);

             UpdateVisible();
        }
     }
     else if(control == UPDATE_ID)
     {
          cout<<"Please wait ...."<<endl;

               UpdateVisible();
               UpdateColor(); 
               UpdateRender(); 
     }
     glutPostRedisplay();
}

void key(unsigned char key, int x, int y)
{
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
      /*  case  'l':
               {cout<<"Please input the index of the min layer"<<endl;
               cin>>zmin;
               cout<<"Please input the index of the  max layer"<<endl;
               cin>>zmax;
               UpdateVisible();
               svVector3 p = flow_field->GetPlanePosition(zmin);
               cout<<"min plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
               p = flow_field->GetPlanePosition(zmax);
               cout<<"max plane: "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
                }break;
*/  
        case 't':
                {
                   svVector3 value[3];
                   widget->MoveRight();
                   svInt *index=new svInt[3];  widget->GetIndex(index);
                   for(int i=0;i<3;i++)
                   {
                     value[i]  = flow_field->GetPlanePosition(index[i]);
                   }
                   widget->SetValues(value);                   
                   UpdateVisible();
                } break;
         case 'T':
              {
                  svVector3 value[3];
                  widget->MoveLeft();
                  svInt *index=new svInt[3];  widget->GetIndex(index);
                  for(int i=0;i<3;i++)
                  {
                      value[i]  = flow_field->GetPlanePosition(index[i]);
                  }
                  widget->SetValues(value);
                  UpdateVisible();
               } break;
        }
 
        glutPostRedisplay();
}
void mouse(int button, int state, int x, int y)
{
    mouse_button = button;
    mouse_state = state;
  
//cerr<<mouse_button<<" "<<state<<endl;
        long s=0x00000000;
    s |= (button == GLUT_LEFT_BUTTON)   ? ((state == GLUT_DOWN) ? Trackball::LBUTTON_DOWN : Trackball::LBUTTON_UP) : 0;
    s |= (button == GLUT_MIDDLE_BUTTON) ? ((state == GLUT_DOWN) ? Trackball::MBUTTON_DOWN : Trackball::MBUTTON_UP) : 0;
    s |= (button == GLUT_RIGHT_BUTTON)  ? ((state == GLUT_DOWN) ? Trackball::RBUTTON_DOWN : Trackball::RBUTTON_UP) : 0;

    int key_state = glutGetModifiers();
    s |= (key_state & GLUT_ACTIVE_CTRL)  ? Trackball::CTRL_DOWN  : 0;
//    s |= (key_state & GLUT_ACTIVE_ALT)   ? Trackball::ALT_DOWN   : 0;
    s |= (key_state & GLUT_ACTIVE_SHIFT) ? Trackball::SHIFT_DOWN : 0;

     int tmplayer, tmpindex;

       if(layerVisible && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        {//cerr<<x<<" "<<y<<endl;
         //   int x1 = (x-widget_tranx)/widget_scalex;
         //   int y1 = (image_height-y-widget_trany)/widget_scaley;
            widget->Mouse(widget_tranx, widget_trany,
                          widget_scalex, widget_scaley,
                          x,image_height-y);
            updatevisible = true; 
        }

        if(button == GLUT_LEFT_BUTTON && state == GLUT_UP && widget->isSelect())
        {
           UpdateVisible();
           
           widget->Reset();
        }

        if (s & Trackball::BUTTON_DOWN){
          trackball.mouseDown(s, x, y);
          glEnable(GL_CULL_FACE);
          ACSIZE = 1;g_numPasses=2;
        }

        if (s & Trackball::BUTTON_UP){
          trackball.mouseUp(s, x, y);
          glDisable(GL_CULL_FACE);
          ACSIZE=4;g_numPasses=4;
        }
        glutPostRedisplay();

}

void motion(int x, int y)
{
        int x1 = (x-widget_tranx)/widget_scalex;
        int y1 = (image_height-y-widget_trany)/widget_scaley;

       if(layerVisible)
       {
             svVector3 value[3];
             widget->Move(x1, y1);
             svInt *index=new svInt[3];  widget->GetIndex(index);
             for(int i=0;i<3;i++)
             {
               value[i]  = flow_field->GetPlanePosition(index[i]);
             }
             widget->SetValues(value);
       }

       if(!widget->isSelect())
        trackball.mouseMotion(x, y);
        glutPostRedisplay();
}

void movement(int x, int y)
{
      movement_x = x;
      movement_y = y;

      glutPostRedisplay();
}
void InitField()
{
	char *qdot_format = new char[400];
	sprintf(qdot_format,"%s/%s/format.txt", 
                    paser->GetStoreDir(),
                    paser->GetRawFile());
        flow_field->SetVTK(
          paser->GetRawDir(), paser->GetRawFile(),
          paser->GetStoreDir(), 
          "sort.txt", "format.txt", "density.txt",
          paser->GetPlaneCenter(),
          paser->GetPlaneVector(),
          paser->GetPlaneDistance(),
          paser->GetFormat()
        );
        flow_field->New(qdot_format);	
        char *str = new char[200];

        sprintf(str, "%s/%s/", 
         paser->GetStoreDir(),paser->GetRawFile());
        flow_field->NewMesh(str);

      delete [] str;
      delete [] qdot_format;

}
void ReadConfig(char *inputfile)
{
    paser->ReadFiles(inputfile);

    InitField();

    paser->SetSeedNum(flow_field->GetPlaneNum());    
    paser->SetPlane(flow_field->GetPlaneDistance(),flow_field->GetMinPlane());

    char *str = new char[400];

    sprintf(str, "%s/%s/density.txt", paser->GetStoreDir(), paser->GetRawFile());
    paser->ProcessContour();
    paser->SetContourValues(flow_field->GetMaxDensity(str));
    paser->ProcessClusterLayers();
    paser->ResetBool();
cerr<<"done config"<<endl;	
    delete [] str;

//===============================================
   bendproperty.bendRange.add(1e-7);
   bendproperty.bendHeight.add(1);
   bendproperty.bendWidth.add(1);

   bendproperty.bendRange.add(1.5e-6);
   bendproperty.bendHeight.add(7.5);
   bendproperty.bendWidth.add(2);

   bendproperty.bendRange.add(4e-6);
   bendproperty.bendHeight.add(4);
   bendproperty.bendWidth.add(10);

   bendproperty.bendRange.add(1.5e-5);
   bendproperty.bendHeight.add(7.5);
   bendproperty.bendWidth.add(20);

   bendproperty.bendRange.add(4e-5);
   bendproperty.bendHeight.add(8);
   bendproperty.bendWidth.add(50);

   bendproperty.bendRange.add(1e-4);
   bendproperty.bendHeight.add(10);
   bendproperty.bendWidth.add(100);

   bendproperty.bendLegendRange.add(1e-7);
   bendproperty.bendLegendValue.add(10);
   bendproperty.bendLegendHeight.add(10);
   bendproperty.bendLegendWidth.add(1);

   bendproperty.bendLegendRange.add(1.5e-6);
   bendproperty.bendLegendValue.add(20);
   bendproperty.bendLegendHeight.add(10);
   bendproperty.bendLegendWidth.add(2);

   bendproperty.bendLegendRange.add(4e-6);
   bendproperty.bendLegendValue.add(100);
   bendproperty.bendLegendHeight.add(10);
   bendproperty.bendLegendWidth.add(10);

   bendproperty.bendLegendRange.add(1.5e-5);
   bendproperty.bendLegendValue.add(200);
   bendproperty.bendLegendHeight.add(10);
   bendproperty.bendLegendWidth.add(20);

   bendproperty.bendLegendRange.add(4e-5);
   bendproperty.bendLegendValue.add(500);
   bendproperty.bendLegendHeight.add(10);
   bendproperty.bendLegendWidth.add(50);

   bendproperty.bendLegendRange.add(1e-4);
   bendproperty.bendLegendValue.add(1000);
   bendproperty.bendLegendHeight.add(10);
   bendproperty.bendLegendWidth.add(100);


   barproperty.scalex = 0.006;
   barproperty.scaley = 0.06;

   barproperty.halowidth = barproperty.scalex*bendproperty.bendHeight[0]/2.;
   barproperty.legend =1e-6;//1.5e-6 ;
   barproperty.legendh = 10 * barproperty.scaley;
   barproperty.legendw = 2 * barproperty.scalex;
   barproperty.circleradius = barproperty.scalex*bendproperty.bendWidth[0]*2.;
   barproperty.legendcolor[0]=91/255.;//137./255.;//91/255.;
   barproperty.legendcolor[1]=168./255.;//70./255.;//168./255.;
   barproperty.legendcolor[2]=126./255.;//64./255.;//126./255.;
   barproperty.legendcolor[3]=1;
   barproperty.circlecolor[0]=1.;//227./255.;//118./255;//227./255.;
   barproperty.circlecolor[1]=0.;//218./255.;//206./255.;//218./255.;
   barproperty.circlecolor[2]=0.;//87./255.;//58./255.;//87./255.;
   barproperty.circlecolor[3]=1.;
   barproperty.dirbarh = 5 * barproperty.scaley;
}

void ComputeGLUIFactor()
{
    glui_length_scale = 1;
    glui_tube_scale = 1;
    glui_arrow_scale = 1;
/*
    if(encode_type == LINEAR )
    {
        glui_length_scale = (length_scale / LINEAR_SCALE)/5.;
    }
    else
    {
        glui_length_scale = (length_scale / SPLITVECTORS_SCALE)/5.;
    } 
    glui_arrow_scale = (arrow_scale / ARROW_SCALE)/2.;
    glui_tube_scale = (tube_scale / TUBE_SCALE)/2.;
*/
}

void InitDualPeelingRenderTargets()
{
        glGenTextures(2, g_dualDepthTexId);
        glGenTextures(2, g_dualFrontBlenderTexId);
        glGenTextures(2, g_dualBackTempTexId);

        glewExperimental = GL_TRUE;
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
typedef struct
{
 float location[4];
 float color[4];
} Vertex;
Vertex verts[6]; // triangle vertices
GLubyte tindices[6]; // triangle vertex indices
GLuint vboHandle[1]; // a VBO that contains interleaved positions and colors
GLuint indexVBO; 

void RenderDualPeeling()
{
/*   glEnable(GL_NORMALIZE);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   glEnable(GL_COLOR_MATERIAL);
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glDisable(GL_LIGHTING);
*/
 
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
 //===================================
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, g_dualPeelingSingleFboId);

        glDrawBuffers(2, &g_drawBuffers[1]);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawBuffer(g_drawBuffers[0]);
        glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBlendEquationEXT(GL_MAX_EXT);

        g_shaderDualInit.bind();
       // splitglyph->Render();
    
        glPushMatrix();
        glMultMatrixf(m);
        glEnable(GL_LIGHTING);
        if(encode_type == LINEAR)
        {
            directcylinder->RenderVBO();
        }
        else if(encode_type == LLO || encode_type == LCL)
        {
            llocylinder->RenderVBO();
         //   llocylinder->RenderLegend();
        }
        else if(encode_type == LT)
        {
            ltcylinder->RenderVBO();
         //   ltcylinder->RenderLegend();
        }
        else if(encode_type == LC)
        {
            lccylinder->RenderVBO();
         //   lccylinder->RenderLegend();
        }
        else if(encode_type == LLP)
        {
            llpcylinder->RenderVBO();
        //    llpcylinder->RenderLegend();
        }
        else if(encode_type == CC)
        {
            glCallList(10);
        }
        glDisable(GL_LIGHTING);
       // outline->DrawXYZ(directcylinder->GetLb(), directcylinder->GetRb(), viewproperty);
        //outline->DrawAxis(directcylinder->GetLb(), directcylinder->GetRb());
       // outline->DrawAxisSticks(directcylinder->GetLb(), directcylinder->GetRb(), viewproperty);
        glPopMatrix();
       // outline->DrawXYZFont(directcylinder->GetLb(), directcylinder->GetRb(), viewproperty);
       // outline->DrawAxisFont(directcylinder->GetLb(), directcylinder->GetRb(), viewproperty);

        g_numGeoPasses++;
        g_shaderDualInit.unbind();

        glEnable(GL_LIGHTING);

        CHECK_GL_ERRORS;

//=======================================

        glDrawBuffer(g_drawBuffers[6]);
        glClearColor(g_backgroundColor[0], g_backgroundColor[1], g_backgroundColor[2], 0);
        glClear(GL_COLOR_BUFFER_BIT);

        int currId = 0;

        for (int pass = 1; g_useOQ || pass < g_numPasses; pass++) {
                currId = pass % 2;
                int prevId = 1 - currId;
                int bufId = currId * 3;

                glDrawBuffers(2, &g_drawBuffers[bufId+1]);
                glClearColor(0, 0, 0, 0);
                glClear(GL_COLOR_BUFFER_BIT);

                glDrawBuffer(g_drawBuffers[bufId+0]);
                glClearColor(-MAX_DEPTH, -MAX_DEPTH, 0, 0);
                glClear(GL_COLOR_BUFFER_BIT);
                glDrawBuffers(3, &g_drawBuffers[bufId+0]);
                glBlendEquationEXT(GL_MAX_EXT);

                g_shaderDualPeel.bind();
                g_shaderDualPeel.bindTextureRECT("DepthBlenderTex", g_dualDepthTexId[prevId], 0);
                g_shaderDualPeel.bindTextureRECT("FrontBlenderTex", g_dualFrontBlenderTexId[prevId], 1);
              /* for(int j=0;j<display_list.size();j++)
               {
                        g_shaderDualPeel.setUniform("Alpha", (float*)&(g_opacity[j]), 1);
                        glCallList(display_list[j]);
               }
*/
                //splitglyph->Render();
                glPushMatrix();
                glMultMatrixf(m);
                 glEnable(GL_LIGHTING);
                g_shaderDualPeel.setUniform("Alpha", (float*)&(alpha),1);
                 if(encode_type == LINEAR)
                 {
                     directcylinder->RenderVBO();
                 }
                 else if(encode_type == LLO || encode_type == LCL)
                 {
                    llocylinder->RenderVBO();
               //     llocylinder->RenderLegend();
                 }
                 else if(encode_type == LT)
                 {
                   ltcylinder->RenderVBO();
             //      ltcylinder->RenderLegend();
                 }
                 else if(encode_type == LC)
                 {
                   lccylinder->RenderVBO();
           //        lccylinder->RenderLegend();
                 }
                 else if(encode_type == LLP)
                 {
                    llpcylinder->RenderVBO();
         //           llpcylinder->RenderLegend();
                 }
                 else if(encode_type == CC)
                 {
                      glCallList(10);
                 }

                 glDisable(GL_LIGHTING);
                 float solid = 1;
                g_shaderDualPeel.setUniform("Alpha", (float*)&(solid),1);
                // outline->DrawXYZ(directcylinder->GetLb(), directcylinder->GetRb(),viewproperty);
                 //outline->DrawAxis(directcylinder->GetLb(), directcylinder->GetRb());
                // outline->DrawAxisSticks(directcylinder->GetLb(), directcylinder->GetRb(), viewproperty);
                 glPopMatrix();
                // outline->DrawXYZFont(directcylinder->GetLb(), directcylinder->GetRb(), viewproperty);
                // outline->DrawAxisFont(directcylinder->GetLb(), directcylinder->GetRb(), viewproperty);
                 glEnable(GL_LIGHTING);
            //    glPopMatrix();
        //        display();
                g_numGeoPasses++;
                g_shaderDualPeel.unbind();
                CHECK_GL_ERRORS;
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

                if (g_useOQ) {
                        glEndQuery(GL_SAMPLES_PASSED_ARB);
                        GLuint sample_count;
                        glGetQueryObjectuiv(g_queryId, GL_QUERY_RESULT_ARB, &sample_count);
                        if (sample_count == 0) {
                                break;
                        }
                }
        }

        glDisable(GL_BLEND);

//==============================================
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
        glDisable(GL_LIGHTING);
        glDisable(GL_NORMALIZE);

        glGenQueries(1, &g_queryId);
}

void init(char *configfname)//rbfname, char *cpname)
{
//==============================
//======== init parameters=======
//===============================
  if(encode_type == LINEAR)
    length_scale = LINEAR_SCALE;
  else
    length_scale = SPLITVECTORS_SCALE;

  arrow_scale = ARROW_SCALE;
  tube_scale = TUBE_SCALE;
  summary_arrow_scale = SUMMARY_ARROW_SCALE;
  summary_tube_scale = SUMMARY_TUBE_SCALE;

  summaryVisible = 0;
  symmetry_enable = 0;
  textureinit = 0;

  widget_tranx=275;
  widget_trany=image_height-125;
  widget_scalex=4;
  widget_scaley=10.;

  svColors *color_object = new svColors();
  for(int i=0;i<SYMMETRY_TYPE;i++)
  {
      buttoncolors[i] = color_object->Get8Colors(i);
      buttoncolors[i][3] = 0.5;
  }

//===========display list=========
  symmetrymesh_solid_list = 50;
  mesh_solid_list =30;
  render_list = 10;
  summary_list = 20;
  outline_display_list = 40;

//===========dual peeling=========

   char *str = new char[200];

  sprintf(str,"%s/libs/dual_depth_peeling/shaders/",SRC_DIR);

  InitGL(str);

//===========variables=============
  outline = new svOutline();
  flow_field = new svQDOT();
  widget = new svWidget();

  configFile = strdup(configfname);
  paser = new Paser();
  ReadConfig(configFile);

  bendglyph = new svBendBar(flow_field);
  directcylinder = new svDirectCylinder(flow_field);
  llocylinder = new svLLOCylinder(flow_field);
  ltcylinder = new svLTCylinder(flow_field);
  lccylinder = new svLCCylinder(flow_field);
  llpcylinder = new svLLPCylinder(flow_field);

  delete [] str;
//===========operation============
  show_data = paser->GetIsContour();

  ComputeGLUIFactor();

  glui_display();

  Update();
  cerr<<"min "<<flow_field->GetMinValue()<<" "<<flow_field->GetMaxValue()<<endl;
}

void New()
{
cerr<<"start new"<<endl;
  bendglyph->New(flow_field, flow_field->GetPlaneNum());
cerr<<"Bend glyoh"<<endl;
  directcylinder->New(flow_field, flow_field->GetPlaneNum());
  llocylinder->New(flow_field, flow_field->GetPlaneNum());
  ltcylinder->New(flow_field, flow_field->GetPlaneNum());
  lccylinder->New(flow_field, flow_field->GetPlaneNum());
  llpcylinder->New(flow_field, flow_field->GetPlaneNum());
}

void InitData()
{
cerr<<paser->GetIsContour()<<endl;
  if(paser->GetIsContour())
  {
       directcylinder->GenerateContours(paser->GetContourProperty());
       llocylinder->GenerateContours(paser->GetContourProperty());//cerr<<"done"<<endl;
       ltcylinder->GenerateContours(paser->GetContourProperty());
       lccylinder->GenerateContours(paser->GetContourProperty());
       llpcylinder->GenerateContours(paser->GetContourProperty());
       bendglyph->GenerateContours(paser->GetContourProperty());

         directcylinder->GenerateEntropy();
         llocylinder->GenerateEntropy();
         ltcylinder->GenerateEntropy();
         lccylinder->GenerateEntropy();
         llpcylinder->GenerateEntropy();
         bendglyph->GenerateEntropy();

       widget->SetHistoValues(directcylinder->GetMagProb(topmaglevel));
       widget->SetValues(directcylinder->GetEntropyValues(),true);

       bendglyph->GenerateSymmetry(paser->GetSymmetryProperty());
       directcylinder->GenerateSymmetry(paser->GetSymmetryProperty());
       llocylinder->GenerateSymmetry(paser->GetSymmetryProperty());
       ltcylinder->GenerateSymmetry(paser->GetSymmetryProperty());//, frequency);
       lccylinder->GenerateSymmetry(paser->GetSymmetryProperty());//, frequency);
       llpcylinder->GenerateSymmetry(paser->GetSymmetryProperty());

       symmetrycount = directcylinder->GetSymmetryMagProb(symmetrytopvalue);
  }
  else
   {
         char *str = new char[400];
         for(int i=0;i<flow_field->GetPlaneNum();i++)
         {
               sprintf(str, "%s/%s/%s%d.txt", paser->GetStoreDir(),
                       paser->GetRawFile(), paser->GetPlaneName(),i);
               directcylinder->SetData(str, i);
               llocylinder->SetData(str, i);
               llpcylinder->SetData(str, i);
               ltcylinder->SetData(str, i);
               lccylinder->SetData(str, i);
               bendglyph->SetData(str, i);
         }
         delete [] str;
         char *format_fname = new char[400];
         sprintf(format_fname,"%s/%s/zformat.txt",paser->GetStoreDir(),
                paser->GetRawFile());
         char *index_fname = new char[400];
         sprintf(index_fname,"%s/%s/reverseindex.txt",
                      paser->GetStoreDir(),
                      paser->GetRawFile());
         directcylinder->SetFormat(format_fname, index_fname);
         llocylinder->SetFormat(format_fname, index_fname);
         llpcylinder->SetFormat(format_fname, index_fname);
         ltcylinder->SetFormat(format_fname, index_fname);
         lccylinder->SetFormat(format_fname, index_fname);
         bendglyph->SetFormat(format_fname, index_fname);
         delete [] format_fname;
         delete [] index_fname;

         bendglyph->GenerateEntropy();
         directcylinder->GenerateEntropy();
         llocylinder->GenerateEntropy();
         llpcylinder->GenerateEntropy();
         ltcylinder->GenerateEntropy();
         lccylinder->GenerateEntropy();

         widget->SetHistoValues(directcylinder->GetMagProb(topmaglevel));
         widget->SetValues(directcylinder->GetEntropyValues(),true);

         bendglyph->SetContourLabel();
         directcylinder->SetContourLabel();
         llocylinder->SetContourLabel();
         llpcylinder->SetContourLabel();
         ltcylinder->SetContourLabel();
         lccylinder->SetContourLabel();

         directcylinder->GenerateSymmetry(paser->GetSymmetryProperty());//, frequency);
         llocylinder->GenerateSymmetry(paser->GetSymmetryProperty());//, frequency);
         llpcylinder->GenerateSymmetry(paser->GetSymmetryProperty());
         ltcylinder->GenerateSymmetry(paser->GetSymmetryProperty());//, frequency);
         lccylinder->GenerateSymmetry(paser->GetSymmetryProperty());
         bendglyph->GenerateSymmetry(paser->GetSymmetryProperty());

         symmetrycount = directcylinder->GetSymmetryMagProb(symmetrytopvalue);
  }

}

void InitFactors()
{
   directcylinder->ResetCluster();//cerr<<"done1"<<endl;
   directcylinder->SetXdistance(flow_field->GetXdistance());//cerr<<"done1"<<endl;
   directcylinder->SetYdistance(flow_field->GetYdistance());//cerr<<"done1"<<endl;
   directcylinder->SetZdistance(flow_field->GetZdistance());//cerr<<"done1"<<endl;
//   directcylinder->SetROI(paser->GetMagRange00(),paser->GetMagRange01());
//   directcylinder->GenerateClustersBySymmetry(paser->GetStep1Property(),
//                      paser->GetSymmetryProperty(),
//                      symmetrytype);
//   directcylinder->SetROI(paser->GetMagRange10(),paser->GetMagRange11());
//   directcylinder->GenerateClustersBySymmetry(paser->GetStep2Property(),
//                         paser->GetSymmetryProperty(),
//                      symmetrytype);
   directcylinder->SetNumPower(flow_field->GetNumPower());
   directcylinder->SetScaling(flow_field->GetScaling());

   llocylinder->ResetCluster();
   llocylinder->SetXdistance(flow_field->GetXdistance());
   llocylinder->SetYdistance(flow_field->GetYdistance());
   llocylinder->SetZdistance(flow_field->GetZdistance());
//   llocylinder->GenerateClusters(directcylinder->GetClusterLabels());
   llocylinder->SetES(flow_field->GetScaling());
   llocylinder->SetNumPower(flow_field->GetNumPower());
   llocylinder->SetScaling(flow_field->GetScaling());

   ltcylinder->ResetCluster();
   ltcylinder->SetXdistance(flow_field->GetXdistance());
   ltcylinder->SetYdistance(flow_field->GetYdistance());
   ltcylinder->SetZdistance(flow_field->GetZdistance());
//   ltcylinder->GenerateClusters(directcylinder->GetClusterLabels());
   ltcylinder->SetES(flow_field->GetScaling());
   ltcylinder->SetNumPower(flow_field->GetNumPower());
   ltcylinder->SetScaling(flow_field->GetScaling());

   lccylinder->ResetCluster();
   lccylinder->SetXdistance(flow_field->GetXdistance());
   lccylinder->SetYdistance(flow_field->GetYdistance());
   lccylinder->SetZdistance(flow_field->GetZdistance());
//   lccylinder->GenerateClusters(directcylinder->GetClusterLabels());
   lccylinder->SetES(flow_field->GetScaling());
   lccylinder->SetNumPower(flow_field->GetNumPower());
   lccylinder->SetScaling(flow_field->GetScaling());

   llpcylinder->ResetCluster();
   llpcylinder->SetXdistance(flow_field->GetXdistance());
   llpcylinder->SetYdistance(flow_field->GetYdistance());
   llpcylinder->SetZdistance(flow_field->GetZdistance());
   llpcylinder->SetES(7);//flow_field->GetScaling());
   llpcylinder->SetNumPower(flow_field->GetNumPower());
   llpcylinder->SetScaling(flow_field->GetScaling());
}

void Update()
{
  cerr<<"update"<<endl;

  widget->SetLevel(flow_field->GetPlaneNum());
  widget->Init();
cerr<<"widget"<<endl;
cerr<<"paser "<<paser->GetZmin()<<" "<<paser->GetZmax()<<" "<<paser->GetNonz()<<endl;
 widget->SetIndex(paser->GetZmin(), paser->GetZmax(), paser->GetNonz());//zmin, zmax, nonz);
 //widget->Repeat(paser->GetRepeated());

cerr<<"widget"<<endl;
cerr<<"widget"<<endl;
  svVector3 value[3];
  svInt *index=new svInt[3];  widget->GetIndex(index);
  for(int i=0;i<3;i++)
  {
         value[i]  = flow_field->GetPlanePosition(index[i]);
  }
   widget->SetValues(value);

  New();cerr<<"New"<<endl;

  InitData();cerr<<"InitData"<<endl;
  InitFactors();cerr<<"InitFactor"<<endl;

  UpdateVBOData();cerr<<"UpdateVBOData()"<<endl;
  UpdateVisible();cerr<<"UpdateVisible()"<<endl;
  UpdateCluster();cerr<<"UpdateCluster()"<<endl;
  UpdateColor();cerr<<"UpdateColor()"<<endl;
  UpdateRender();cerr<<"UpdateRender()"<<endl;

  svVector3 center = flow_field->GetCenter();//cerr<<"done"<<endl;
  center.getValue(view_info.coi);

  GLfloat x, y, z;
  flow_field->GetPhysicalDimension(&x,&y, &z);

  view_info.eye[0] = 0;//x/2.0;
  view_info.eye[1] = 0;// y/2.0;
  view_info.eye[2] = z*2.0;

  viewproperty.eye[0] = view_info.eye[0];
  viewproperty.eye[1] = view_info.eye[1];
  viewproperty.eye[2] = view_info.eye[2];

  trackball.setEye(view_info.eye);
  trackball.setFocus(center);
  trackball.setWindowSize(image_width, image_height);
  trackball.reset();
}

//**********************
//// GLUI
////**********************
void glui_display()
{
  char str[50];
  glui = GLUI_Master.create_glui_subwindow( window,
                                            GLUI_SUBWINDOW_LEFT );
  new GLUI_StaticText(glui, "Show me");

  GLUI_RadioGroup *group_encode = new GLUI_RadioGroup(glui, &encode_type, ENCODE_ID, control_cb);
  glui->add_radiobutton_to_group(group_encode, "Linear");
  glui->add_radiobutton_to_group(group_encode, "LLo");
  glui->add_radiobutton_to_group(group_encode, "LC");
  glui->add_radiobutton_to_group(group_encode, "LT");
  glui->add_radiobutton_to_group(group_encode, "LCL");
  glui->add_radiobutton_to_group(group_encode, "LLp");
  glui->add_radiobutton_to_group(group_encode, "CC");

  GLUI_Panel *panel_layer= glui->add_panel("ROI selection");
  panel_layer->set_alignment(GLUI_ALIGN_LEFT);
  box_layer = glui->add_checkbox_to_panel(panel_layer,"Layers", &layerVisible, LAYER_ID, control_cb);
  glui->add_column_to_panel(panel_layer, true);
  new GLUI_Checkbox(panel_layer, "Repeat", &layer_repeat, LAYER_REPEAT_ID, control_cb);

  new GLUI_Button(glui, "Update", UPDATE_ID, control_cb);
}

//**********************
// program entry
//**********************

int main(int argc, char** argv)
{
   image_width  = IMAGE_WIDTH;
   image_height = IMAGE_HEIGHT;
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA
                          | GLUT_DEPTH | GLUT_ACCUM | GLUT_MULTISAMPLE);
        glutInitWindowSize(image_width, image_height);
           glutInitWindowPosition(0, 0);
        window = glutCreateWindow("QDOT");
        if (glewInit() != GLEW_OK)
        {
                printf("glewInit failed. Exiting...\n");
                exit(1);
        }

   GLUI_Master.set_glutReshapeFunc(Reshape);
   init(argv[1]);
   Init();
   glutDisplayFunc(accDisplay);

   GLUI_Master.set_glutKeyboardFunc(key);
   GLUI_Master.set_glutSpecialFunc(NULL);
   GLUI_Master.set_glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutPassiveMotionFunc(movement);  

 //  glui_display();

   glutMainLoop();
   return 0;


}

#include "renderApi.h"

void renderApi::initParameter(){
 alpha = 0.75;
 blackcolor[0]=0;blackcolor[1]=0;blackcolor[2]=0;blackcolor[3]=0;

 magthreashold = 0;
 volumebound[0]=0; volumebound[1]=100;
 vbound=0;
 imageCutoff = 10;
 image_inside = false;
 
 g_numPasses = 4;
 g_imageWidth = 1024;
 g_imageHeight = 768;
 g_useOQ = false;
 g_opacity = 0.6;
 g_showOsd = true;
 g_bShowUI = true;
 g_numGeoPasses = 0;
 g_rotating = 0;
 g_panning = 0;
 g_scaling = 0;
 g_bbScale = 1.0;
 g_bbTrans[0]=0; g_bbTrans[1]=0; g_bbTrans[2]=0;
 g_rot[0] = 0; g_rot[1] = 45;
 g_pos[0]=0;g_pos[1]=0;g_pos[2]=2;
 g_white[0]=1;g_white[1]=1;g_white[2]=1;
 g_black[0]=0;g_black[1]=0;g_black[2]=0;
 g_backgroundColor = g_white;
}

void renderApi::init(dataApi *raw, dataApi *data){
  outline = new svOutline();
  image = new svContourImageDrawing(raw->getData());
  lt = new svLTArrow(data->getData());
  lc = new svLCArrow(data->getData());
}

void renderApi::Reshape(int w, int h){
  image_width = w;
  image_height = h;
}

void renderApi:: 

/*=============lighting==============*/
void renderApi::ArrowLight()
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

void renderApi::InitLight()
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
/*silver material
void InitLight(){
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
/*==============lighting end===========*/

/*==============glyph======================*/
void renderApi::UpdateVis(){
  if(renderType == RENDER_TYPE_LC){
    lc->UpdateIndex();cerr<<"update index"<<endl;
    lc->GenerateIndex();cerr<<"index"<<endl;
    lc->UpdateIndexVBO();cerr<<"ivbo"<<endl;
  }
  else if(renderType == RENDER_TYPE_LT){
    lt->UpdateIndex();cerr<<"update index"<<endl;
    lt->GenerateIndex();cerr<<"index"<<endl;
    lt->UpdateIndexVBO();cerr<<"ivbo"<<endl;
  }
}
void renderApi::UpdateRender(){
 if(renderType == RENDER_TYPE_LT){
  lt->UpdateData();
  lt->UpdateColor();
  lt->SetColorByClusterMag();
  lt->UpdateIndex();//cerr<<"update index"<<endl;
  lt->GenerateTubes();//cerr<<"tube"<<endl;
  lt->GenerateArrows();//cerr<<"arrow"<<endl;
  lt->GenerateLegend();//cerr<<"legend"<<endl;
  lt->GenerateIndex();//cerr<<"index"<<endl;
  lt->UpdateTubeVBO();//cerr<<"tvbo"<<endl;
  lt->UpdateArrowVBO();//cerr<<"avbo"<<endl;
  lt->UpdateIndexVBO();//cerr<<"ivbo"<<endl;
 }
}
void renderApi::UpdateVisible(){
} 
/*==============glyph end==================*/


/*==============hyperslice==================*/

void renderApi::Update2DContour(){
   image->SetSideX(50.);//hard code!!!
   image->ResetContourTree();//cerr<<"1"<<endl;
   image->New();//cerr<<"1"<<endl;
   image->GeneratePixels();//cerr<<"1"<<endl;
   image->SetBound(volumebound[0], volumebound[1], volumestep);//cerr<<"1"<<endl;
   image->GenerateContourTree(paser->GetContourTreeProperty());//cerr<<"2"<<endl;
   image->GenerateContours(paser->GetContourTreeDir(),
                             paser->GetPlaneName());//cerr<<"3"<<endl;
   image->GenerateSelectedContours(paser->GetContourProperty());//cerr<<"4"<<endl;
   image->GenerateVisibility(vbound, magthreshold);//cerr<<"5"<<endl;
   Reshape2DContour();//cerr<<"6"<<endl;
   image->SetContourPosition();//cerr<<"7"<<endl;

   char *str = new char[400];
   sprintf(str, "%s/%s/2dcontour.txt", paser->GetStoreDir(),paser->GetRawFile());
   image->SaveContourtoFile(str);cerr<<"8"<<endl;
  delete [] str;

  Reshape2DContour();
}
void renderApi::UpdateSingleContour(){
  image->GenerateContour(paser->GetContourTreeDir(),
                            paser->GetPlaneName());
   char *str = new char[400];
   sprintf(str, "%s/%s/2dcontour.txt", paser->GetStoreDir(),paser->GetRawFile());
   image->SaveContourtoFile(str);cerr<<"8"<<endl;
  delete [] str;
  Update2D();
}
void renderApi::Reshape2DContour(){
   int rx, ry, rw, rh;
   rx = 0;
   ry = 0;
   rw = image_width;
   rh = image_height;
   rx = image_width - 300;
   rw = 300.;
   image->Reshape(50,rw,rh,rx,ry);//hard code!!!!!!!!!
   image->SetMinD();
   image->SetContourPosition();
   image->getImageSize(image_lbx, image_lby, image_rbx, image_rby);
}

void renderApi::Update2DColor(){
  char *str = new char[400];
  sprintf(str, "%s/%s/2dcolor.txt", paser->GetStoreDir(),paser->GetRawFile());
  cerr<<str<<endl;
  if(paser->GetIsContour())//hard code!!
  image->SaveColortoFile(str, lc->GetColors());
  else
  image->SaveColortoFile(str, lt->GetColors());
  delete [] str;
}

void renderApi::Update2D(){
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

void renderApi::imageMovement(int movement_x){
            if(image_inside)
             image->Mouse(movement_x, image_height-movement_y,
                         contour_layer, contour_index, contour_mag);
}
/*==============hyperslice end==============*/


/*===================peeling==================*/

void renderApi::InitDualPeelingRenderTargets()
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
void renderApi::InitAccumulationRenderTargets()
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

void renderApi::DeleteDualPeelingRenderTargets()
{
        glDeleteFramebuffersEXT(1, &g_dualBackBlenderFboId);
        glDeleteFramebuffersEXT(1, &g_dualPeelingSingleFboId);
        glDeleteTextures(2, g_dualDepthTexId);
        glDeleteTextures(2, g_dualFrontBlenderTexId);
        glDeleteTextures(2, g_dualBackTempTexId);
        glDeleteTextures(1, &g_dualBackBlenderTexId);
}

void renderApi::MakeFullScreenQuad()
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

void renderApi::BuildShaders(string SHADER_PATH)
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

void renderApi::DestroyShaders()
{
        g_shaderDualInit.destroy();
        g_shaderDualPeel.destroy();
        g_shaderDualBlend.destroy();
        g_shaderDualFinal.destroy();
}

void renderApi::DeleteAccumulationRenderTargets()
{
        glDeleteFramebuffersEXT(1, &g_accumulationFboId);
        glDeleteTextures(2, g_accumulationTexId);
}

void renderApi::InitGL(string shader)
{
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        InitDualPeelingRenderTargets();
        InitAccumulationRenderTargets();
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        BuildShaders(shader);
        MakeFullScreenQuad();
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glGenQueries(1, &g_queryId);     
} 



void renderApi::render(){
  
}
void renderApi::RenderDualPeeling()
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
        render();
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
                render();
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

//              if (g_useOQ) {
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

void renderApi::RenderAverageColors()
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
//              g_shaderAverageInit.setUniform("Alpha", (float*)&alphainside, 1);
        g_shaderAverageInit.setUniform("Alpha", (float*)&alpha, 1);
        render();
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


/*=============peeling end=================*/

~renderApi(){
   delete lcglyph;
   delete ltglyph;
}

void renderApi::loadData(char *dir, 
                         char *infile1, char *infile2){
   if(renderType == RENDER_TYPE_ATOM){
      ltglyph->SetData(dir, infile1);
      lcglyph->SetData(dir, infile2);
   }
}

void renderApi::updateVBO(){
   if(renderType = RRENDER_TYPE_ATOM){
      ltglyph->UpdateLTData();
      lcglyph->UpdateData();
   }
}

void renderApi::updateVisible(int *layer){
   if(renderType == RENDER_TYPE_ATOM){
     ltglyph->ResetVisible();
     lcglyph->ResetVisible();
     ltglyph->SetVisible(layer);
     lcglyph->SetVisible(layer);
     ltglyph->UpdateLTIndex();
     lcglyph->UpdateIndex();
     ltglyph->GenerateIndex();
     lcglyph->GenerateIndex();
     ltglyph->UpdateIndexVBO();
     lcglyph->UpdateIndexVBO();
   }
}

void renderApi::updateColor(){
   if(renderType == RENDER_TYPE_ATOM){
     ltglyph->SetSolidColor();
     lcglyph->SetColorByPower();
   }
}

void renderApi::updateRender(int level, 
                             svScalar lscale, svScalar tscale){
   if(renderType == RENDER_TYPE_ATOM){
     ltglyph->SetScale(scale);
     ltglyph->SetTubeRadius(tscale);
     ltglyph->GenerateTubes();
     ltglyph->UpdateTubeVBO();
     ltglyph->GenerateLegend();
     lcglyph->SetScale(scale);
     lcglyph->SetTubeRadius(tscale);
     lcglyph->GenerateTubes();
     lcglyph->UpdateTubeVBO();
     lcglyph->GenerateLegend();
   }
}

void rendreApi::show(){
   if(renderType == RENDER_TYPE_ATOM){
     ltglyph->RenderVBO();
     lcglyph->RenderVBO();
   }
}
